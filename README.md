# FFmpeg OpenGL ES filter
OpenGL ES filter for FFmpeg is a combination of two great libraries: [ffmpeg-gl-transition
](https://github.com/transitive-bullshit/ffmpeg-gl-transition) and [mobile-ffmpeg](https://github.com/tanersener/mobile-ffmpeg). This filter allows you to use *ffmpeg-gl-transition* in your Android application.
The main cause of making this library is using OpenGL shader in Android application. So, we took the idea from *ffmpeg-gl-transition* and ported it OpenGL ES, then we builded FFmpeg with this filter included by using *mobile-ffmpeg* and afterwards we had our dummy shader included in great FFmpeg library.

This shader just blends two videos into one.

![](https://media.giphy.com/media/iJJKKoAQxqACOszX4D/giphy.gif)

You can find the instructions of how to add a custom filter to FFmpeg *ffmpeg.diff* in *ffmpeg-gl-transition* library. Next we added some changes to *config.h* file in *mobile-ffmpeg/src/ffmpeg* directory: 
```sh
--extra-libs='-lGLESv3 -lEGL -lgcc'
```
Then, after following instructions for building ffmpeg in *moblie-ffmpeg* library, we had the .aar file containing all needed FFmpeg binaries. After adding this file to our project we're going to use our OpenGL filter with this command: 
```sh
-i path-to-video1 -i path-to-video2 -filter_complex "gltransition=baseMix=1:blendMix=1" -y out.mp4
```
### Contribution
After starting the execution of OpenGL filter on Android we found out that the speed of execution of this command leaves something to be desired on some Chinese devices. We added some changes to our filter:
```
Replaced GL_RGB with GL_RGBA
Removed any calls of the method glPixelStorei() 
```
So, these changes improved the speed of execution on Chineses devices, but it drastically slowed down the execution of this command on Samsung devices. As, FFmpeg uses CPU for encoding the video on Android, we are sure that this issue depends on CPU. If you have any ideas related to this, please let us know. 