#include "libavutil/opt.h"
#include "internal.h"
#include "framesync.h"
#include "filters.h"

#include "libavcodec/avcodec.h"
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>

#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <android/log.h>


#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#define BASE (0)
#define BLEND (1)

#define PIXEL_FORMAT GL_RGB

#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(55, 28, 1)
#define av_frame_alloc  avcodec_alloc_frame
#endif

#define LOG_TAG "GLBUFEX"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

static const EGLint configAttribs[] = {
    EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,
    EGL_RED_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_BLUE_SIZE, 8,
    EGL_DEPTH_SIZE, 16,
    EGL_NONE};

static const EGLint contextAttribs[] = {
    EGL_CONTEXT_CLIENT_VERSION, 3,
    EGL_NONE};

static GLenum err;

static const float position[12] = {
    -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f
};

static const GLchar *v_shader_source =
"attribute mediump vec2 position;\n"
"varying mediump vec2 vTextureCoord;\n"
"void main(void) {\n"
"  gl_Position = vec4(position, 0, 1);\n"
"  lowp vec2 uv = position * 0.5 + 0.5;\n"
"  vTextureCoord = vec2(uv.x, uv.y);\n"
"}\n";

static const GLchar *f_shader_source =
"varying mediump vec2 vTextureCoord;\n"
"uniform mediump sampler2D u_baseTexUnit;\n"
"uniform mediump sampler2D u_blendTexUnit;\n"
"uniform mediump float u_baseMix;\n"
"uniform mediump float u_blendMix;\n"

"mediump vec3 screen(mediump vec3 src1, mediump vec3 src2, mediump float alpha) {\n"
"   return mix(src1, src1 + src2 - src1 * src2, alpha);\n"
"}\n"

"void main() {\n"

"   mediump vec3 base = mix(vec3(0.0), texture2D(u_baseTexUnit, vTextureCoord).rgb, u_baseMix);\n"
"   mediump vec3 blend = texture2D(u_blendTexUnit, vTextureCoord).rgb;\n"
"   gl_FragColor = vec4(screen(base, blend, u_blendMix), 1.0);\n"
"}\n";

typedef struct {
    const AVClass *class;
    FFFrameSync fs;

    double baseMix;
    double blendMix;

    int64_t first_pts;

    GLuint u_base;
    GLuint u_blend;
    GLint u_baseMix;
    GLint u_blendMix;

    GLuint posBuf;
    GLuint program;

    EGLDisplay eglDpy;
    EGLConfig eglCfg;
    EGLSurface eglSurf;
    EGLContext eglCtx;

    GLchar *f_shader_source;

} GLTransitionContext;

#define OFFSET(x) offsetof(GLTransitionContext, x)
#define FLAGS AV_OPT_FLAG_FILTERING_PARAM|AV_OPT_FLAG_VIDEO_PARAM

static const AVOption gltransition_options[] = {
    {"baseMix",  "base intensity",  OFFSET(baseMix),  AV_OPT_TYPE_DOUBLE, {.dbl=1.0}, 0, DBL_MAX,
        FLAGS},
    {"blendMix", "blend intensity", OFFSET(blendMix), AV_OPT_TYPE_DOUBLE, {.dbl=1.0}, 0, DBL_MAX,
        FLAGS},
    {NULL}
};

FRAMESYNC_DEFINE_CLASS(gltransition, GLTransitionContext, fs);

static GLuint build_shader(AVFilterContext *ctx, const GLchar *shader_source, GLenum type) {
    GLuint shader = glCreateShader(type);

    if (!shader || !glIsShader(shader)) {
        return 0;
    }

    glShaderSource(shader, 1, &shader_source, 0);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    
    if (status == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        GLchar* strInfoLog = malloc(maxLength + 1);
        glGetShaderInfoLog(shader, maxLength, &maxLength, strInfoLog);

        LOGI("build_shader info error: %s", strInfoLog);

        glDeleteShader(shader); // Don't leak the shader.
    }

    return (status == GL_TRUE ? shader : 0);
}

static int build_program(AVFilterContext *ctx) {
    GLuint v_shader, f_shader;
    GLTransitionContext *c = ctx->priv;

    if (!(v_shader = build_shader(ctx, v_shader_source, GL_VERTEX_SHADER))) {
        av_log(ctx, AV_LOG_ERROR, "invalid vertex shader\n");
        return -1;
    }

    if (!(f_shader = build_shader(ctx, f_shader_source, GL_FRAGMENT_SHADER))) {
        av_log(ctx, AV_LOG_ERROR, "invalid fragment shader\n");
        return -1;
    }

    c->program = glCreateProgram();
    glAttachShader(c->program, v_shader);
    glAttachShader(c->program, f_shader);
    glLinkProgram(c->program);

    GLint status;
    glGetProgramiv(c->program, GL_LINK_STATUS, &status);

    return status == GL_TRUE ? 0 : -1;
}

static void setup_vbo(GLTransitionContext *c) {
    glGenBuffers(1, &c->posBuf);
    glBindBuffer(GL_ARRAY_BUFFER, c->posBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);

    GLint loc = glGetAttribLocation(c->program, "position");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

static void setup_tex(AVFilterLink *baseLink) {
    AVFilterContext *ctx = baseLink->dst;
    GLTransitionContext *c = ctx->priv;

    { // base
        glGenTextures(1, &c->u_base);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, c->u_base);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, PIXEL_FORMAT, baseLink->w, baseLink->h, 0, PIXEL_FORMAT, GL_UNSIGNED_BYTE, NULL);

        glUniform1i(glGetUniformLocation(c->program, "u_baseTexUnit"), 0);
    }

    { // blend
        glGenTextures(1, &c->u_blend);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, c->u_blend);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, PIXEL_FORMAT, baseLink->w, baseLink->h, 0, PIXEL_FORMAT, GL_UNSIGNED_BYTE, NULL);

        glUniform1i(glGetUniformLocation(c->program, "u_blendTexUnit"), 1);
    }
}

static void setup_uniforms(AVFilterLink *baseLink) {
    AVFilterContext *ctx = baseLink->dst;
    GLTransitionContext *c = ctx->priv;

    c->u_baseMix = glGetUniformLocation(c->program, "u_baseMix");
    glUniform1f(c->u_baseMix, 1.0f);

    c->u_blendMix = glGetUniformLocation(c->program, "u_blendMix");
    glUniform1f(c->u_blendMix, 1.0f);
}

static int setup_gl(AVFilterLink *inLink) {
    AVFilterContext *ctx = inLink->dst;
    GLTransitionContext *c = ctx->priv;

    c->eglDpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    EGLint major, minor;
    eglInitialize(c->eglDpy, &major, &minor);
    av_log(ctx, AV_LOG_DEBUG, "%d%d", major, minor);

    EGLint numConfigs;
    EGLint pbufferAttribs[] = {
        EGL_WIDTH,
        inLink->w,
        EGL_HEIGHT,
        inLink->h,
        EGL_NONE,
    };

    eglChooseConfig(c->eglDpy, configAttribs, &c->eglCfg, 1, &numConfigs);
    c->eglSurf = eglCreatePbufferSurface(c->eglDpy, c->eglCfg,
                                         pbufferAttribs);

    eglBindAPI(EGL_OPENGL_ES_API);
    c->eglCtx = eglCreateContext(c->eglDpy, c->eglCfg, EGL_NO_CONTEXT, contextAttribs);
    eglMakeCurrent(c->eglDpy, c->eglSurf, c->eglSurf, c->eglCtx);

    glViewport(0, 0, inLink->w, inLink->h);

    int ret;
    if ((ret = build_program(ctx)) < 0) {
        return ret;
    }

    glUseProgram(c->program);
    setup_vbo(c);
    setup_uniforms(inLink);
    setup_tex(inLink);
    
    return 0;
}

static AVFrame *draw(FFFrameSync *fs,
                     AVFilterContext *ctx,
                     AVFrame *baseFrame,
                     const AVFrame *blendFrame
                     ) {

    GLTransitionContext *c = ctx->priv;
    AVFilterLink *baseLink = ctx->inputs[BASE];
    AVFilterLink *blendLink = ctx->inputs[BLEND];
    AVFilterLink *outLink = ctx->outputs[0];
    AVFrame *outFrame;

    outFrame = ff_get_video_buffer(outLink, outLink->w, outLink->h);
    if (!outFrame) {
        return NULL;
    }

    av_frame_copy_props(outFrame, baseFrame);

    eglMakeCurrent(c->eglDpy, c->eglSurf, c->eglSurf, c->eglCtx);

    glUseProgram(c->program);

    glUniform1f(c->u_baseMix, c->baseMix);
    glUniform1f(c->u_blendMix, c->blendMix);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, c->u_base);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, baseFrame->linesize[0] / 3);
    glTexImage2D(GL_TEXTURE_2D, 0, PIXEL_FORMAT, baseLink->w, baseLink->h, 0, PIXEL_FORMAT, GL_UNSIGNED_BYTE,
                 baseFrame->data[0]);

    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, c->u_blend);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, blendFrame->linesize[0] / 3);
    glTexImage2D(GL_TEXTURE_2D, 0, PIXEL_FORMAT, blendLink->w, blendLink->h, 0, PIXEL_FORMAT, GL_UNSIGNED_BYTE,
                 blendFrame->data[0]);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glPixelStorei(GL_PACK_ROW_LENGTH, outFrame->linesize[0] / 3);
    clock_t begin = clock();

    glReadPixels(0, 0, outLink->w, outLink->h, PIXEL_FORMAT, GL_UNSIGNED_BYTE, (GLvoid *) outFrame->data[0]);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    LOGI("draw time_spent %f", time_spent);

    glPixelStorei(GL_PACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

    av_frame_free(&baseFrame);

    return outFrame;
}

static int process_frame(FFFrameSync *fs) {
    AVFilterContext *ctx = fs->parent;
    GLTransitionContext *c = ctx->priv;

    AVFrame *baseFrame, *blendFrame, *outFrame;
    int ret;

    ret = ff_framesync_dualinput_get(fs, &baseFrame, &blendFrame);
    if (ret < 0) {
        return ret;
    }

    if (c->first_pts == AV_NOPTS_VALUE && baseFrame && baseFrame->pts != AV_NOPTS_VALUE) {
        c->first_pts = baseFrame->pts;
    }

    if (!blendFrame) {
        return ff_filter_frame(ctx->outputs[0], baseFrame);
    }

    outFrame = draw(fs, ctx, baseFrame, blendFrame);
    if (!outFrame) {
        return AVERROR(ENOMEM);
    }

    return ff_filter_frame(ctx->outputs[0], outFrame);
}

static av_cold int init(AVFilterContext *ctx) {
    GLTransitionContext *c = ctx->priv;
    c->fs.on_event = process_frame;
    c->first_pts = AV_NOPTS_VALUE;

    return 0;
}

static av_cold void uninit(AVFilterContext *ctx) {
    GLTransitionContext *c = ctx->priv;
    ff_framesync_uninit(&c->fs);

    if (c->eglDpy) {
        glDeleteTextures(1, &c->u_base);
        glDeleteTextures(1, &c->u_blend);
        glDeleteBuffers(1, &c->posBuf);
        glDeleteProgram(c->program);
        eglTerminate(c->eglDpy);
    }

    if (c->f_shader_source) {
        av_freep(&c->f_shader_source);
    }
}

static int query_formats(AVFilterContext *ctx) {
    static const enum AVPixelFormat formats[] = {
        AV_PIX_FMT_RGB24,
        AV_PIX_FMT_NONE
    };

    return ff_set_common_formats(ctx, ff_make_format_list(formats));
}

struct SwsContext *swsContext;

static int activate(AVFilterContext *ctx) {
    GLTransitionContext *c = ctx->priv;
    return ff_framesync_activate(&c->fs);
}

static int config_output(AVFilterLink *outLink) {
    AVFilterContext *ctx = outLink->src;
    GLTransitionContext *c = ctx->priv;
    AVFilterLink *baseLink = ctx->inputs[BASE];
    AVFilterLink *blendLink = ctx->inputs[BLEND];
    int ret;

    if (baseLink->format != blendLink->format) {
        av_log(ctx, AV_LOG_ERROR, "inputs must be of same pixel format\n");
        return AVERROR(EINVAL);
    }

    //if (baseLink->w != blendLink->w || baseLink->h != blendLink->h) {
    //    av_log(ctx, AV_LOG_ERROR, "First input link %s parameters "
    //           "(size %dx%d) do not match the corresponding "
    //           "second input link %s parameters (size %dx%d)\n",
    //           ctx->input_pads[BASE].name, baseLink->w, baseLink->h,
    //           ctx->input_pads[BLEND].name, blendLink->w, blendLink->h);
    //    return AVERROR(EINVAL);
    //}

    outLink->w = baseLink->w;
    outLink->h = baseLink->h;

    outLink->frame_rate = baseLink->frame_rate;

    if ((ret = ff_framesync_init_dualinput(&c->fs, ctx)) < 0) {
        return ret;
    }

    return ff_framesync_configure(&c->fs);
}

static const AVFilterPad gltransition_inputs[] = {
    {
        .name = "base",
        .type = AVMEDIA_TYPE_VIDEO,
        .config_props = setup_gl
    },
    {
        .name = "blend",
        .type = AVMEDIA_TYPE_VIDEO
    },
    {NULL}
};

static const AVFilterPad gltransition_outputs[] = {
    {
        .name = "outputx",
        .type = AVMEDIA_TYPE_VIDEO,
        .config_props = config_output,
    },
    {NULL}
};

AVFilter ff_vf_gltransition = {
    .name          = "gltransition",
    .description   = NULL_IF_CONFIG_SMALL("OpenGL blend videos"),
    .priv_size     = sizeof(GLTransitionContext),
    .preinit       = gltransition_framesync_preinit,
    .init          = init,
    .uninit        = uninit,
    .query_formats = query_formats,
    .activate      = activate,
    .inputs        = gltransition_inputs,
    .outputs       = gltransition_outputs,
    .priv_class    = &gltransition_class,
    .flags         = AVFILTER_FLAG_SUPPORT_TIMELINE_GENERIC
};

