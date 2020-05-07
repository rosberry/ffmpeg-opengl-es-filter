#include "libavutil/opt.h"
#include "internal.h"
#include "framesync.h"
#include "filters.h"

#include "libavcodec/avcodec.h"
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>


#ifndef __APPLE__
//# define GL_TRANSITION_USING_EGL //remove this line if you don't want to use EGL
#endif

//#ifdef __APPLE__
//# define __gl_h_
//# define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
//# include <OpenGL/gl3.h>
//#else

//#include <GL/glew.h>
//#include <GLES2/gl2ext.h>


//#endif

//#ifdef GL_TRANSITION_USING_EGL

#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <android/log.h>


//#else
//# include <GLFW/glfw3.h>
//#endif

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#define BASE (0)
#define BLEND (1)
#define ADJ_LUT (2)
#define STATIC_LUT (3)

#define PIXEL_FORMAT GL_RGB

#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(55, 28, 1)
#define av_frame_alloc  avcodec_alloc_frame
#endif
//#ifdef GL_TRANSITION_USING_EGL

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
"  vTextureCoord = vec2(uv.x, 1.0 - uv.y);\n"
"}\n";

static const GLchar *f_shader_source =
"varying mediump vec2 vTextureCoord;\n"
"uniform mediump sampler2D u_baseTexUnit;\n"
"uniform mediump sampler2D u_blendTexUnit;\n"
"uniform mediump sampler2D u_adjustmentsLookupTexUnit;\n"
"uniform mediump sampler2D u_staticLutTexUnit;\n"
"uniform mediump float u_baseMix;\n"
"uniform mediump float u_blendMix;\n"

// lut
"const mediump float stepDis = 1.0 / 8.0;\n"
"const mediump float perPixel = 1.0 / 512.0;\n"
"const mediump float halfPixel = 0.5 / 512.0;\n"

"mediump vec2 firstLookupPosition(mediump vec3 base) {\n"
"   mediump float blueColor = base.b * 63.0;\n"

"   mediump vec2 firstQuad = vec2(0.0);\n"
"   firstQuad.y = floor(floor(blueColor) / 8.0);\n"
"   firstQuad.x = floor(blueColor) - (firstQuad.y * 8.0);\n"

"   return firstQuad * stepDis + halfPixel + (stepDis - perPixel) * base.xy;\n"
"}\n"

"mediump vec2 secondLookupPosition(mediump vec3 base) {\n"
"   mediump float blueColor = base.b * 63.0;\n"

"   mediump vec2 secondQuad = vec2(0.0);\n"
"   secondQuad.y = floor(ceil(blueColor) / 8.0);\n"
"   secondQuad.x = ceil(blueColor) - (secondQuad.y * 8.0);\n"

"   return secondQuad * stepDis + halfPixel + (stepDis - perPixel) * base.xy;\n"
"}\n"

"mediump vec3 newLut(mediump vec3 base, mediump sampler2D lutTexUnit) {\n"
"   mediump float blueColor = base.b * 63.0;\n"

"   mediump vec3 firstLookup = texture2D(lutTexUnit, firstLookupPosition(base)).rgb;\n"
"   mediump vec3 secondLookup = texture2D(lutTexUnit, secondLookupPosition(base)).rgb;\n"

"   return mix(firstLookup, secondLookup, 1.0);\n"
"}\n"

// luminance
"mediump float luminance(mediump vec3 color) {\n"
"   mediump float fmin = min(min(color.r, color.g), color.b);\n"
"   mediump float fmax = max(max(color.r, color.g), color.b);\n"

"   return (fmax + fmin) / 2.0;\n"
"}\n"

// screen
"mediump vec3 screen(mediump vec3 src1, mediump vec3 src2, mediump float alpha) {\n"
"   return mix(src1, src1 + src2 - src1 * src2, alpha);\n"
"}\n"

"void main() {\n"
"   mediump vec3 base = mix(vec3(0.0), texture2D(u_baseTexUnit, vec2(vTextureCoord.x, 1.0 - vTextureCoord.y)).rgb, u_baseMix);\n"
"   mediump float baseLuminance = luminance(base);\n"
"   base = newLut(base, u_adjustmentsLookupTexUnit);\n"
"   base = mix(base, newLut(base, u_staticLutTexUnit), u_blendMix) * baseLuminance;\n"
"   mediump vec3 blend = texture2D(u_blendTexUnit, vec2(vTextureCoord.x, 1.0 - vTextureCoord.y)).rgb;\n"
"   gl_FragColor = vec4(screen(base, blend, u_blendMix), 1.0);\n"
"}\n";

typedef struct {
    const AVClass *class;
    FFFrameSync fs;
    
    // input options
    double baseMix;
    double blendMix;
    
    // timestamp of the first frame in the output, in the timebase units
    int64_t first_pts;
    
    // uniforms
    GLuint u_base;
    GLuint u_blend;
    GLuint u_adjLut;
    GLuint u_staticLut;
    GLint u_baseMix;
    GLint u_blendMix;
    
    // internal state
    GLuint posBuf;
    GLuint program;
    //#ifdef GL_TRANSITION_USING_EGL
    EGLDisplay eglDpy;
    EGLConfig eglCfg;
    EGLSurface eglSurf;
    EGLContext eglCtx;
    
    GLchar *f_shader_source;
    AVFrame *adjLut_frame;
    AVFrame *staticLut_frame;
    
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
    
    LOGI("build_shader");
    
    while((err = glGetError()) != GL_NO_ERROR) {
        LOGI("build_shader -1 error: %d", err);
        av_log(ctx, AV_LOG_ERROR, "error glLinkProgram %d", err);
    }
    
    if (!shader || !glIsShader(shader)) {
        return 0;
    }
    
    while((err = glGetError()) != GL_NO_ERROR) {
        LOGI("build_shader error: %d", err);
        av_log(ctx, AV_LOG_ERROR, "error glLinkProgram %d", err);
    }
    
    LOGI("build_shader 1");
    
    while((err = glGetError()) != GL_NO_ERROR) {
        LOGI("build_shader 1 error: %d", err);
        av_log(ctx, AV_LOG_ERROR, "error glLinkProgram %d", err);
    }
    
    glShaderSource(shader, 1, &shader_source, 0);
    glCompileShader(shader);
    
    LOGI("build_shader 2");
    
    while((err = glGetError()) != GL_NO_ERROR) {
        LOGI("build_shader 2 error: %d", err);
        av_log(ctx, AV_LOG_ERROR, "error glLinkProgram %d", err);
    }
    
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    
    while((err = glGetError()) != GL_NO_ERROR) {
        LOGI("build_shader 3 error: %d", err);
        av_log(ctx, AV_LOG_ERROR, "error glLinkProgram %d", err);
    }
    
    if (status == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        
        // The maxLength includes the NULL character
        GLchar* strInfoLog = malloc(maxLength + 1);
        glGetShaderInfoLog(shader, maxLength, &maxLength, strInfoLog);
        
        LOGI("build_shader info error: %s", strInfoLog);
        
        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(shader); // Don't leak the shader.
    }
    
    LOGI("build_shader 3");
    
    return (status == GL_TRUE ? shader : 0);
}

static int build_program(AVFilterContext *ctx) {
    GLuint v_shader, f_shader;
    GLTransitionContext *c = ctx->priv;
    
    LOGI("build_program");
    
    if (!(v_shader = build_shader(ctx, v_shader_source, GL_VERTEX_SHADER))) {
        av_log(ctx, AV_LOG_ERROR, "invalid vertex shader\n");
        return -1;
    }
    
    if (!(f_shader = build_shader(ctx, f_shader_source, GL_FRAGMENT_SHADER))) {
        av_log(ctx, AV_LOG_ERROR, "invalid fragment shader\n");
        return -1;
    }
    
    LOGI("build_program 1");
    
    c->program = glCreateProgram();
    glAttachShader(c->program, v_shader);
    glAttachShader(c->program, f_shader);
    glLinkProgram(c->program);
    
    LOGI("build_program 2");
    
    GLint status;
    glGetProgramiv(c->program, GL_LINK_STATUS, &status);
    
    LOGI("build_program 3");
    
    return status == GL_TRUE ? 0 : -1;
}

static void setup_vbo(GLTransitionContext *c) {
    LOGI("setup_vbo");
    
    glGenBuffers(1, &c->posBuf);
    glBindBuffer(GL_ARRAY_BUFFER, c->posBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);
    
    LOGI("setup_vbo 1");
    
    GLint loc = glGetAttribLocation(c->program, "position");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    LOGI("setup_vbo 2");
    
}

static void setup_tex(AVFilterLink *baseLink) {
    AVFilterContext *ctx = baseLink->dst;
    GLTransitionContext *c = ctx->priv;
    
    LOGI("setup_tex");
    
    { // base
        glGenTextures(1, &c->u_base);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, c->u_base);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        
        glTexImage2D(GL_TEXTURE_2D, 0, PIXEL_FORMAT, baseLink->w, baseLink->h, 0, PIXEL_FORMAT, GL_UNSIGNED_BYTE, NULL);
        
        glUniform1i(glGetUniformLocation(c->program, "u_baseTexUnit"), 0);
    }
    
    LOGI("setup_tex 2");
    
    { // blend
        glGenTextures(1, &c->u_blend);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, c->u_blend);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        
        glTexImage2D(GL_TEXTURE_2D, 0, PIXEL_FORMAT, baseLink->w, baseLink->h, 0, PIXEL_FORMAT, GL_UNSIGNED_BYTE, NULL);
        
        glUniform1i(glGetUniformLocation(c->program, "u_blendTexUnit"), 1);
    }
    
    LOGI("setup_tex 3");
    
    { // adjustments LUT
        glGenTextures(1, &c->u_adjLut);
        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, c->u_adjLut);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        
        glTexImage2D(GL_TEXTURE_2D, 0, PIXEL_FORMAT, 512, 512, 0, PIXEL_FORMAT, GL_UNSIGNED_BYTE, NULL);
        
        glUniform1i(glGetUniformLocation(c->program, "u_adjustmentsLookupTexUnit"), 2);
    }
    
    LOGI("setup_tex 4");
    
    { // static LUT
        glGenTextures(1, &c->u_staticLut);
        glActiveTexture(GL_TEXTURE0 + 3);
        glBindTexture(GL_TEXTURE_2D, c->u_staticLut);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        
        glTexImage2D(GL_TEXTURE_2D, 0, PIXEL_FORMAT, 512, 512, 0, PIXEL_FORMAT, GL_UNSIGNED_BYTE, NULL);
        
        glUniform1i(glGetUniformLocation(c->program, "u_staticLutTexUnit"), 3);
    }
    
    LOGI("setup_tex 5");
    
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
    
    LOGI("setup_gl");
    
    //init EGL
    // 1. Initialize EGL
    c->eglDpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    EGLint major, minor;
    eglInitialize(c->eglDpy, &major, &minor);
    av_log(ctx, AV_LOG_DEBUG, "%d%d", major, minor);
    // 2. Select an appropriate configuration
    EGLint numConfigs;
    EGLint pbufferAttribs[] = {
        EGL_WIDTH,
        inLink->w,
        EGL_HEIGHT,
        inLink->h,
        EGL_NONE,
    };
    
    LOGI("setup_gl 1");
    
    eglChooseConfig(c->eglDpy, configAttribs, &c->eglCfg, 1, &numConfigs);
    // 3. Create a surface
    c->eglSurf = eglCreatePbufferSurface(c->eglDpy, c->eglCfg,
                                         pbufferAttribs);
    
    LOGI("setup_gl 2");
    
    // 4. Bind the API
    eglBindAPI(EGL_OPENGL_ES_API);
    // 5. Create a context and make it current
    c->eglCtx = eglCreateContext(c->eglDpy, c->eglCfg, EGL_NO_CONTEXT, contextAttribs);
    eglMakeCurrent(c->eglDpy, c->eglSurf, c->eglSurf, c->eglCtx);
    
    LOGI("setup_gl 3");
    
    glViewport(0, 0, inLink->w, inLink->h);
    
    int ret;
    if ((ret = build_program(ctx)) < 0) {
        return ret;
    }
    
    LOGI("setup_gl 4");
    
    glUseProgram(c->program);
    setup_vbo(c);
    setup_uniforms(inLink);
    setup_tex(inLink);
    
    LOGI("setup_gl 5");
    
    return 0;
}

static AVFrame *draw(FFFrameSync *fs,
                     AVFilterContext *ctx,
                     AVFrame *baseFrame,
                     const AVFrame *blendFrame,
                     const AVFrame *adjLutFrame,
                     const AVFrame *staticLutFrame
                     ) {
    
    LOGI("draw");
    
    GLTransitionContext *c = ctx->priv;
    AVFilterLink *baseLink = ctx->inputs[BASE];
    AVFilterLink *blendLink = ctx->inputs[BLEND];
    AVFilterLink *adjLutLink = ctx->inputs[ADJ_LUT];
    AVFilterLink *staticLutLink = ctx->inputs[STATIC_LUT];
    AVFilterLink *outLink = ctx->outputs[0];
    AVFrame *outFrame;
    
    LOGI("draw 2");
    
    outFrame = ff_get_video_buffer(outLink, outLink->w, outLink->h);
    if (!outFrame) {
        return NULL;
    }
    
    LOGI("draw 3");
    
    av_frame_copy_props(outFrame, baseFrame);
    
    eglMakeCurrent(c->eglDpy, c->eglSurf, c->eglSurf, c->eglCtx);
    
    glUseProgram(c->program);
    
    glUniform1f(c->u_baseMix, c->baseMix);
    glUniform1f(c->u_blendMix, c->blendMix);
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    LOGI("draw 4");
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, c->u_base);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, baseFrame->linesize[0] / 3);
    glTexImage2D(GL_TEXTURE_2D, 0, PIXEL_FORMAT, baseLink->w, baseLink->h, 0, PIXEL_FORMAT, GL_UNSIGNED_BYTE,
                 baseFrame->data[0]);
    
    LOGI("draw 5");
    
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, c->u_blend);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, blendFrame->linesize[0] / 3);
    glTexImage2D(GL_TEXTURE_2D, 0, PIXEL_FORMAT, blendLink->w, blendLink->h, 0, PIXEL_FORMAT, GL_UNSIGNED_BYTE,
                 blendFrame->data[0]);
    
    LOGI("draw 6");
    
    glActiveTexture(GL_TEXTURE0 + 2);
    LOGI("draw 6_1");
    glBindTexture(GL_TEXTURE_2D, c->u_adjLut);
    LOGI("draw 6_2");
    glPixelStorei(GL_UNPACK_ROW_LENGTH, adjLutFrame->linesize[0] / 3);
    LOGI("draw 6_3");
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 512, 512, PIXEL_FORMAT, GL_UNSIGNED_BYTE, adjLutFrame->data[0]);
    
    LOGI("draw 7");
    
    glActiveTexture(GL_TEXTURE0 + 3);
    LOGI("draw 7_1");
    glBindTexture(GL_TEXTURE_2D, c->u_staticLut);
    LOGI("draw 7_2");
    glPixelStorei(GL_UNPACK_ROW_LENGTH, staticLutFrame->linesize[0] / 3);
    LOGI("draw 7_3");
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 512, 512, PIXEL_FORMAT, GL_UNSIGNED_BYTE, staticLutFrame->data[0]);
    
    LOGI("draw 7_4");
    glDrawArrays(GL_TRIANGLES, 0, 6);
    LOGI("draw 7_5");
    glPixelStorei(GL_PACK_ROW_LENGTH, outFrame->linesize[0] / 3);
    LOGI("draw 7_6");
    glReadPixels(0, 0, outLink->w, outLink->h, PIXEL_FORMAT, GL_UNSIGNED_BYTE, (GLvoid *) outFrame->data[0]);
    LOGI("draw 7_7");
    
    LOGI("draw 8");
    
    glPixelStorei(GL_PACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    
    av_frame_free(&baseFrame);
    
    LOGI("draw 9");
    
    return outFrame;
}

static int process_frame(FFFrameSync *fs) {
    AVFilterContext *ctx = fs->parent;
    GLTransitionContext *c = ctx->priv;
    
    LOGI("process_frame");
    
    AVFrame *baseFrame, *blendFrame, *outFrame;
    int ret;
    
    if (!c->adjLut_frame) {
        ret = ff_inlink_consume_frame(ctx->inputs[ADJ_LUT], &c->adjLut_frame);
        if (ret < 0) {
            return ret;
        }
    }
    
    LOGI("process_frame 1");
    
    if (!c->staticLut_frame) {
        ret = ff_inlink_consume_frame(ctx->inputs[STATIC_LUT], &c->staticLut_frame);
        if (ret < 0) {
            return ret;
        }
    }
    
    LOGI("process_frame 2");
    
    ret = ff_framesync_dualinput_get(fs, &baseFrame, &blendFrame);
    if (ret < 0) {
        return ret;
    }
    
    LOGI("process_frame 3");
    
    if (c->first_pts == AV_NOPTS_VALUE && baseFrame && baseFrame->pts != AV_NOPTS_VALUE) {
        c->first_pts = baseFrame->pts;
    }
    
    LOGI("process_frame 4");
    
    if (!blendFrame) {
        return ff_filter_frame(ctx->outputs[0], baseFrame);
    }
    
    outFrame = draw(fs, ctx, baseFrame, blendFrame, c->adjLut_frame, c->staticLut_frame);
    if (!outFrame) {
        return AVERROR(ENOMEM);
    }
    
    LOGI("process_frame 5");
    
    return ff_filter_frame(ctx->outputs[0], outFrame);
}

static av_cold int init(AVFilterContext *ctx) {
    GLTransitionContext *c = ctx->priv;
    c->fs.on_event = process_frame;
    c->first_pts = AV_NOPTS_VALUE;
    
    
    //#ifndef GL_TRANSITION_USING_EGL
    //if (!glfwInit())
    //{
    //    return -1;
    //}
    //#endif
    
    return 0;
}

static av_cold void uninit(AVFilterContext *ctx) {
    GLTransitionContext *c = ctx->priv;
    ff_framesync_uninit(&c->fs);
    
    //#ifdef GL_TRANSITION_USING_EGL
    if (c->eglDpy) {
        glDeleteTextures(1, &c->u_base);
        glDeleteTextures(1, &c->u_blend);
        glDeleteTextures(1, &c->u_blend);
        glDeleteBuffers(1, &c->posBuf);
        glDeleteProgram(c->program);
        eglTerminate(c->eglDpy);
    }
    //#else
    //    if (c->window) {
    //        glDeleteTextures(1, &c->u_base);
    //        glDeleteTextures(1, &c->u_blend);
    //        glDeleteBuffers(1, &c->posBuf);
    //        glDeleteProgram(c->program);
    //        glfwDestroyWindow(c->window);
    //    }
    //#endif
    
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
    int ret;
    
    /* if (!c->adjLut_frame) {
     AVFrame *origFrame;
     ret = ff_inlink_consume_frame(ctx->inputs[ADJ_LUT], &origFrame);
     
     if (ret < 0) {
     return ret;
     }
     
     c->adjLut_frame = av_frame_alloc();
     swsContext = sws_getContext(512, 512, *//*origFrame->format*//*AV_PIX_FMT_RGBA, 512, 512, AV_PIX_FMT_RGB24, SWS_BILINEAR, NULL, NULL, NULL);
                                                                   int num_bytes = avpicture_get_size(AV_PIX_FMT_RGB24, 512, 512);
                                                                   uint8_t* frame2_buffer = (uint8_t *)av_malloc(num_bytes*sizeof(uint8_t));
                                                                   avpicture_fill((AVPicture*) c->adjLut_frame, frame2_buffer, AV_PIX_FMT_RGB24, 512, 512);
                                                                   
                                                                   sws_scale(swsContext, (const uint8_t *const *)origFrame->data, origFrame->linesize, 0, 512, c->adjLut_frame->data, c->adjLut_frame->linesize);
                                                                   }*/
    
    /*if (!c->adjLut_frame) {
     AVFrame *origFrame;
     ret = ff_inlink_consume_frame(ctx->inputs[ADJ_LUT], &origFrame);
     
     if (ret < 0) {
     return ret;
     }
     
     swsContext = sws_getContext(origFrame->width, origFrame->height, AV_PIX_FMT_RGBA, origFrame->width,
     origFrame->height, AV_PIX_FMT_RGB24, SWS_BILINEAR, NULL, NULL, NULL);
     uint8_t *data[AV_NUM_DATA_POINTERS];
     int linesize[AV_NUM_DATA_POINTERS];
     av_image_alloc(data, linesize, origFrame->width, origFrame->height, AV_PIX_FMT_RGB24, 1);
     AVFrame *tempFrame = origFrame;
     sws_scale(swsContext, (const uint8_t **) tempFrame->data, origFrame->linesize, 0, origFrame->height, data,
     linesize);
     free(data[0]);
     
     c->adjLut_frame = origFrame;
     }*/
    
    if (!c->adjLut_frame) {
        ret = ff_inlink_consume_frame(ctx->inputs[ADJ_LUT], &c->adjLut_frame);
        if (ret < 0) {
            return ret;
        }
    }
    
    if (!c->staticLut_frame) {
        ret = ff_inlink_consume_frame(ctx->inputs[STATIC_LUT], &c->staticLut_frame);
        if (ret < 0) {
            return ret;
        }
    }
    
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
    
    if (baseLink->w != blendLink->w || baseLink->h != blendLink->h) {
        av_log(ctx, AV_LOG_ERROR, "First input link %s parameters "
               "(size %dx%d) do not match the corresponding "
               "second input link %s parameters (size %dx%d)\n",
               ctx->input_pads[BASE].name, baseLink->w, baseLink->h,
               ctx->input_pads[BLEND].name, blendLink->w, blendLink->h);
        return AVERROR(EINVAL);
    }
    
    outLink->w = baseLink->w;
    outLink->h = baseLink->h;
    // outLink->time_base = baseLink->time_base;
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
    {
        .name = "adjLut",
        .type = AVMEDIA_TYPE_VIDEO,
    },
    {
        .name = "staticLut",
        .type = AVMEDIA_TYPE_VIDEO,
    },
    {NULL}
};

static const AVFilterPad gltransition_outputs[] = {
    {
        .name = "default",
        .type = AVMEDIA_TYPE_VIDEO,
        .config_props = config_output,
    },
    {NULL}
};

AVFilter ff_vf_gltransition = {
    .name          = "gltransition",
    .description   = NULL_IF_CONFIG_SMALL("OpenGL blend transitions"),
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

