Pod::Spec.new do |s|  
    s.name              = "mobile-ffmpeg-tvos-https"
    s.version           = "VERSION"
    s.summary           = "Mobile FFmpeg tvOS Https Static Framework"
    s.description       = <<-DESC
    Includes FFmpeg v4.2-dev-1824 with gmp v6.1.2 and gnutls v3.6.8 library enabled.
    DESC

    s.homepage          = "https://github.com/tanersener/mobile-ffmpeg"

    s.author            = { "Taner Sener" => "tanersener@gmail.com" }
    s.license           = { :type => "LGPL-3.0", :file => "mobileffmpeg.framework/LICENSE" }

    s.platform          = :tvos
    s.requires_arc      = true
    s.libraries         = 'z', 'bz2', 'c++'

    s.source            = { :http => "https://github.com/tanersener/mobile-ffmpeg/releases/download/vVERSION/mobile-ffmpeg-https-VERSION-tvos-framework.zip" }

    s.tvos.deployment_target = '9.2'
    s.tvos.frameworks   = 'AudioToolbox','VideoToolbox','CoreMedia'
    s.tvos.vendored_frameworks = 'mobileffmpeg.framework', 'libavcodec.framework', 'libavdevice.framework', 'libavfilter.framework', 'libavformat.framework', 'libavutil.framework', 'libswresample.framework', 'libswscale.framework', 'gmp.framework', 'gnutls.framework', 'libcharset.framework', 'libhogweed.framework', 'libiconv.framework', 'libnettle.framework'

end  
