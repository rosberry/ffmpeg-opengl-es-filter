Pod::Spec.new do |s|  
    s.name              = "mobile-ffmpeg-tvos-video"
    s.version           = "VERSION"
    s.summary           = "Mobile FFmpeg tvOS Video Static Framework"
    s.description       = <<-DESC
    Includes FFmpeg v4.2-dev-1824 with fontconfig v2.13.91, freetype v2.10.1, fribidi v1.0.5, kvazaar v1.2.0, libaom v1.0.0-dev-2018, libass v0.14.0, libiconv v1.16, libtheora v1.1.1, libvpx v1.8.0, snappy v1.1.7 and libwebp v1.0.2 libraries enabled.
    DESC

    s.homepage          = "https://github.com/tanersener/mobile-ffmpeg"

    s.author            = { "Taner Sener" => "tanersener@gmail.com" }
    s.license           = { :type => "LGPL-3.0", :file => "mobileffmpeg.framework/LICENSE" }

    s.platform          = :tvos
    s.requires_arc      = true
    s.libraries         = 'z', 'bz2', 'c++'

    s.source            = { :http => "https://github.com/tanersener/mobile-ffmpeg/releases/download/vVERSION/mobile-ffmpeg-video-VERSION-tvos-framework.zip" }

    s.tvos.deployment_target = '9.2'
    s.tvos.frameworks   = 'AudioToolbox','VideoToolbox','CoreMedia'
    s.tvos.vendored_frameworks = 'mobileffmpeg.framework', 'libavcodec.framework', 'libavdevice.framework', 'libavfilter.framework', 'libavformat.framework', 'libavutil.framework', 'libswresample.framework', 'libswscale.framework', 'expat.framework', 'fontconfig.framework', 'freetype.framework', 'fribidi.framework', 'giflib.framework', 'jpeg.framework', 'kvazaar.framework', 'libaom.framework', 'libass.framework', 'libcharset.framework', 'libiconv.framework', 'libogg.framework', 'libpng.framework', 'libtheora.framework', 'libtheoradec.framework', 'libtheoraenc.framework', 'libuuid.framework', 'libvorbis.framework', 'libvorbisenc.framework', 'libvorbisfile.framework', 'libvpx.framework', 'libwebp.framework', 'libwebpdecoder.framework', 'libwebpdemux.framework', 'snappy.framework', 'tiff.framework'

end  
