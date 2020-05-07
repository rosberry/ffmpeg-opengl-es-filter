---
layout: default
---
FFmpeg for Android, iOS and tvOS

<img src="https://github.com/tanersener/mobile-ffmpeg/raw/dev-v3.x/docs/assets/mobile-ffmpeg-logo-v7.png" width="240">

### 1. Features
- Supports FFmpeg `v3.4.x`, `v4.0.x`, `v4.1` and `v4.2-dev` releases
- Use prebuilt binaries available under `Github`/`JCenter`/`CocoaPods` or build your own version with external libraries you need
- Includes 28 external libraries, 4 GPL libraries and 13 architectures in total
- Exposes both FFmpeg library and MobileFFmpeg wrapper library capabilities
- Includes cross-compile instructions for 44 open-source libraries

   `chromaprint`, `expat`, `ffmpeg`, `fontconfig`, `freetype`, `fribidi`, `giflib`, `gmp`, `gnutls`, `kvazaar`, `lame`, `leptonica`, `libaom`, `libass`, `libiconv`, `libilbc`, `libjpeg`, `libjpeg-turbo`, `libogg`, `libpng`, `libsndfile`, `libtheora`, `libuuid`, `libvorbis`, `libvpx`, `libwebp`, `libxml2`, `nettle`, `opencore-amr`, `openh264`, `opus`, `sdl`, `shine`, `snappy`, `soxr`, `speex`, `tesseract`, `tiff`, `twolame`, `vid.stab`, `wavpack`, `x264`, `x265`, `xvidcore`

- Supports `API Level 16+` on Android
- Builds `arm-v7a`, `arm-v7a-neon`, `arm64-v8a`, `x86` and `x86_64` Android architectures
- Supports `zlib` and `MediaCodec` Android system libraries
- Creates Android archive with .aar extension
- Supports `iOS SDK 9.3` or later
- Builds `armv7`, `armv7s`, `arm64`, `arm64e`, `i386` and `x86_64` iOS architectures
- Supports `tvOS SDK 9.2` or later
- Builds `arm64` and `x86_64` tvOS architectures
- Supports `bzip2`, `zlib` iOS system libraries and `AudioToolbox`, `CoreImage`, `VideoToolbox`, `AVFoundation` iOS system frameworks
- Android Camera access on [supported devices](https://developer.android.com/ndk/guides/stable_apis#camera)
- iOS Camera access
- `ARC` enabled iOS library
- Creates static framework and static universal (fat) library (.a) on iOS / tvOS
- Licensed under LGPL 3.0, can be customized to support GPL v3.0

### 2. Using
Binaries are available at [Github](https://github.com/tanersener/mobile-ffmpeg/releases), [JCenter](https://bintray.com/bintray/jcenter) and [CocoaPods](https://cocoapods.org).

There are eight different prebuilt packages. Below you can see which external libraries are enabled in each of them.

Please remember that some parts of `FFmpeg` are licensed under the `GPL` and only `GPL` licensed `mobile-ffmpeg` packages include them.

<table>
<thead>
<tr>
<th align="center">min</th>
<th align="center">min-gpl</th>
<th align="center">https</th>
<th align="center">https-gpl</th>
<th align="center">audio</th>
<th align="center">video</th>
<th align="center">full</th>
<th align="center">full-gpl</th>
</tr>
</thead>
<tbody>
<tr>
<td align="center">-</td>
<td align="center"><sup>vid.stab</sup><br><sup>x264</sup><br><sup>x265</sup><br><sup>xvidcore</sup></td>
<td align="center"><sup>gmp</sup><br><sup>gnutls</sup></td>
<td align="center"><sup>gmp</sup><br><sup>gnutls</sup><br><sup>vid.stab</sup><br><sup>x264</sup><br><sup>x265</sup><br><sup>xvidcore</sup></td>
<td align="center"><sup>lame</sup><br><sup>libilbc</sup><br><sup>libvorbis</sup><br><sup>opencore-amr</sup><br><sup>opus</sup><br><sup>shine</sup><br><sup>soxr</sup><br><sup>speex</sup><br><sup>twolame</sup><br><sup>wavpack</sup></td>
<td align="center"><sup>fontconfig</sup><br><sup>freetype</sup><br><sup>fribidi</sup><br><sup>kvazaar</sup><br><sup>libaom</sup><br><sup>libass</sup><br><sup>libiconv</sup><br><sup>libtheora</sup><br><sup>libvpx</sup><br><sup>libwebp</sup><br><sup>snappy</sup></td>
<td align="center"><sup>fontconfig</sup><br><sup>freetype</sup><br><sup>fribidi</sup><br><sup>gmp</sup><br><sup>gnutls</sup><br><sup>kvazaar</sup><br><sup>lame</sup><br><sup>libaom</sup><br><sup>libass</sup><br><sup>libiconv</sup><br><sup>libilbc</sup><br><sup>libtheora</sup><br><sup>libvorbis</sup><br><sup>libvpx</sup><br><sup>libwebp</sup><br><sup>libxml2</sup><br><sup>opencore-amr</sup><br><sup>opus</sup><br><sup>shine</sup><br><sup>snappy</sup><br><sup>soxr</sup><br><sup>speex</sup><br><sup>twolame</sup><br><sup>wavpack</sup></td>
<td align="center"><sup>fontconfig</sup><br><sup>freetype</sup><br><sup>fribidi</sup><br><sup>gmp</sup><br><sup>gnutls</sup><br><sup>kvazaar</sup><br><sup>lame</sup><br><sup>libaom</sup><br><sup>libass</sup><br><sup>libiconv</sup><br><sup>libilbc</sup><br><sup>libtheora</sup><br><sup>libvorbis</sup><br><sup>libvpx</sup><br><sup>libwebp</sup><br><sup>libxml2</sup><br><sup>opencore-amr</sup><br><sup>opus</sup><br><sup>shine</sup><br><sup>snappy</sup><br><sup>soxr</sup><br><sup>speex</sup><br><sup>twolame</sup><br><sup>vid.stab</sup><br><sup>wavpack</sup><br><sup>x264</sup><br><sup>x265</sup><br><sup>xvidcore</sup></td>
</tr>
</tbody>
</table>

 - `libilbc`, `opus`, `snappy`, `x264` and `xvidcore` are supported since `v1.1`

 - `libaom` and `soxr` are supported since `v2.0`

 - `chromaprint`, `vid.stab` and `x265` are supported since `v2.1`

 - `sdl`, `tesseract`, `twolame` external libraries; `zlib`, `MediaCodec` Android system libraries; `bzip2`, `zlib` iOS system libraries and `AudioToolbox`, `CoreImage`, `VideoToolbox`, `AVFoundation` iOS system frameworks are supported since `v3.0`
 
 - Since `v4.2`, `chromaprint`, `sdl` and `tesseract` libraries are not included in binary releases. You can still build them and include in your releases
 
 - `AVFoundation` is not available on `tvOS`, `VideoToolbox` is not available on `tvOS` LTS releases

#### 2.1 Android
1. Add MobileFFmpeg dependency to your `build.gradle` in `mobile-ffmpeg-<package name>` format
    ```
    dependencies {
        implementation 'com.arthenica:mobile-ffmpeg-min:4.2.2'
    }
    ```

2. Execute commands.
    ```
    import com.arthenica.mobileffmpeg.FFmpeg;

    FFmpeg.execute("-i file1.mp4 -c:v mpeg4 file2.mp4");
    ```

3. Check execution output.
    ```
    int rc = FFmpeg.getLastReturnCode();
    String output = FFmpeg.getLastCommandOutput();
 
    if (rc == RETURN_CODE_SUCCESS) {
        Log.i(Config.TAG, "Command execution completed successfully.");
    } else if (rc == RETURN_CODE_CANCEL) {
        Log.i(Config.TAG, "Command execution cancelled by user.");
    } else {
        Log.i(Config.TAG, String.format("Command execution failed with rc=%d and output=%s.", rc, output));
    }
    ```

4. Stop an ongoing operation.
    ```
    FFmpeg.cancel();
    ```

5. Get media information for a file.
    ```
    MediaInformation info = FFmpeg.getMediaInformation("<file path or uri>");
    ```

6. Record video and audio using Android camera.
    ```
    FFmpeg.execute("-f android_camera -i 0:0 -r 30 -pixel_format bgr0 -t 00:00:05 <record file path>");
    ```

7. List enabled external libraries.
    ```
    List<String> externalLibraries = Config.getExternalLibraries();
    ```

8. Enable log callback.
    ```
    Config.enableLogCallback(new LogCallback() {
        public void apply(LogMessage message) {
            Log.d(Config.TAG, message.getText());
        }
    });
    ```

9. Enable statistics callback.
    ```
    Config.enableStatisticsCallback(new StatisticsCallback() {
        public void apply(Statistics newStatistics) {
            Log.d(Config.TAG, String.format("frame: %d, time: %d", newStatistics.getVideoFrameNumber(), newStatistics.getTime()));
        }
    });
    ```

10. Set log level.
    ```
    Config.setLogLevel(Level.AV_LOG_FATAL);
    ```

11. Register custom fonts directory.
    ```
    Config.setFontDirectory(this, "<folder with fonts>", Collections.EMPTY_MAP);
    ```

#### 2.2 iOS / tvOS
1. Add MobileFFmpeg dependency to your `Podfile` in `mobile-ffmpeg-<package name>` format

    - iOS
    ```
    pod 'mobile-ffmpeg-min', '~> 4.2.2'
    ```

    - tvOS
    ```
    pod 'mobile-ffmpeg-tv-min', '~> 4.2.2'
    ```

2. Execute commands.
    ```
    #import <mobileffmpeg/MobileFFmpeg.h>

    [MobileFFmpeg execute: @"-i file1.mp4 -c:v mpeg4 file2.mp4"];
    ```
    
3. Check execution output.
    ```
    int rc = [MobileFFmpeg getLastReturnCode];
    NSString *output = [MobileFFmpeg getLastCommandOutput];

    if (rc == RETURN_CODE_SUCCESS) {
        NSLog(@"Command execution completed successfully.\n");
    } else if (rc == RETURN_CODE_CANCEL) {
        NSLog(@"Command execution cancelled by user.\n");
    } else {
        NSLog(@"Command execution failed with rc=%d and output=%@.\n", rc, output);
    }
    ```

4. Stop an ongoing operation.
    ```
    [MobileFFmpeg cancel];
    ```

5. Get media information for a file.
    ```
    MediaInformation *mediaInformation = [MobileFFmpeg getMediaInformation:@"<file path or uri>"];
    ```

6. Record video and audio using iOS camera. This operation is not supported on `tvOS` since `AVFoundation` is not available on `tvOS`.

    ```
    [MobileFFmpeg execute: @"-f avfoundation -r 30 -video_size 1280x720 -pixel_format bgr0 -i 0:0 -vcodec h264_videotoolbox -vsync 2 -f h264 -t 00:00:05 %@", recordFilePath];
    ```

7. List enabled external libraries.
    ```
    NSArray *externalLibraries = [MobileFFmpegConfig getExternalLibraries];
    ```

8. Enable log callback.
    ```
    [MobileFFmpegConfig setLogDelegate:self];

    - (void)logCallback: (int)level :(NSString*)message {
        dispatch_async(dispatch_get_main_queue(), ^{
            NSLog(@"%@", message);
        });
    }
    ```

9. Enable statistics callback.
    ```
    [MobileFFmpegConfig setStatisticsDelegate:self];

    - (void)statisticsCallback:(Statistics *)newStatistics {
        dispatch_async(dispatch_get_main_queue(), ^{
            NSLog(@"frame: %d, time: %d\n", newStatistics.getVideoFrameNumber, newStatistics.getTime);
        });
    }
    ```

10. Set log level.
    ```
    [MobileFFmpegConfig setLogLevel:AV_LOG_FATAL];
    ```

11. Register custom fonts directory.
    ```
    [MobileFFmpegConfig setFontDirectory:@"<folder with fonts>" with:nil];
    ```    

### 3. Versions

`MobileFFmpeg` version number is aligned with `FFmpeg` since version `4.2`. 

In previous versions, `MobileFFmpeg` version of a release and `FFmpeg` version included in that release was different. 
The following table lists `FFmpeg` versions used in `MobileFFmpeg` releases.
  
- `dev` part in `FFmpeg` version number indicates that `FFmpeg` source is pulled from the `FFmpeg` `master` branch. 
Exact version number is obtained using `git describe --tags`. 

|  MobileFFmpeg Version | FFmpeg Version | Release Date |
| :----: | :----: |:----: |
| [4.2.2](https://github.com/tanersener/mobile-ffmpeg/releases/tag/v4.2.2) | 4.2-dev-1824 | July 3, 2019 |
| [4.2.2.LTS](https://github.com/tanersener/mobile-ffmpeg/releases/tag/v4.2.2.LTS) | 4.2-dev-1824 | July 3, 2019 |
| [4.2.1](https://github.com/tanersener/mobile-ffmpeg/releases/tag/v4.2.1) | 4.2-dev-1156 | Apr 2, 2019 |
| [4.2](https://github.com/tanersener/mobile-ffmpeg/releases/tag/v4.2) | 4.2-dev-480 | Jan 3, 2019 |
| [4.2.LTS](https://github.com/tanersener/mobile-ffmpeg/releases/tag/v4.2.LTS) | 4.2-dev-480 | Jan 3, 2019 |
| [3.1](https://github.com/tanersener/mobile-ffmpeg/releases/tag/v3.1) | 4.1-10 | Dec 11, 2018 |
| [3.0](https://github.com/tanersener/mobile-ffmpeg/releases/tag/v3.0) | 4.1-dev-1517 | Oct 25, 2018 |
| [2.2](https://github.com/tanersener/mobile-ffmpeg/releases/tag/v2.2) | 4.0.3 | Nov 10, 2018 |
| [2.1.1](https://github.com/tanersener/mobile-ffmpeg/releases/tag/v2.1.1) | 4.0.2 | Sep 19, 2018 |
| [2.1](https://github.com/tanersener/mobile-ffmpeg/releases/tag/v2.1) | 4.0.2 | Sep 5, 2018 |
| [2.0](https://github.com/tanersener/mobile-ffmpeg/releases/tag/v2.0) | 4.0.1 | Jun 30, 2018 |
| [1.2](https://github.com/tanersener/mobile-ffmpeg/releases/tag/v1.2) | 3.4.4 | Aug 30, 2018|
| [1.1](https://github.com/tanersener/mobile-ffmpeg/releases/tag/v1.1) | 3.4.2 | Jun 18, 2018 |
| [1.0](https://github.com/tanersener/mobile-ffmpeg/releases/tag/v1.0) | 3.4.2 | Jun 6, 2018 |

### 4. LTS Releases

Starting from `v4.2`, `MobileFFmpeg` binaries are published in two different variants: `Main Release` and `LTS Release`. 

- Main releases include complete functionality of the library and support the latest SDK/API features.

- LTS releases are customized to support a wider range of devices. They are built using older API/SDK versions, so some features are not available on them.

This table shows the differences between two variants.

|        | Main Release | LTS Release |
| :----: | :----: | :----: |
| Android API Level | 24 | 16 | 
| Android Camera Access | Yes | - |
| Android Architectures | arm-v7a-neon<br/>arm64-v8a<br/>x86<br/>x86-64 | arm-v7a<br/>arm-v7a-neon<br/>arm64-v8a<br/>x86<br/>x86-64 |
| Xcode Support | 10.1 | 7.3.1 |
| iOS SDK | 12.1 | 9.3 |
| iOS Architectures | arm64<br/>arm64e<br/>x86-64 | armv7<br/>arm64<br/>i386<br/>x86-64 |
| tvOS SDK | 10.2 | 9.2 |
| tvOS Architectures | arm64<br/>x86-64 | arm64<br/>x86-64 |
    
### 5. Building
#### 5.1 Prerequisites
1. Use your package manager (apt, yum, dnf, brew, etc.) to install the following packages.

    ```
    autoconf automake libtool pkg-config curl cmake gcc gperf texinfo yasm nasm bison autogen patch git
    ```

2. Android builds require these additional packages.
    - **Android SDK 4.1 Jelly Bean (API Level 16)** or later
    - **Android NDK r20** or later with LLDB and CMake

3. iOS builds need these extra packages and tools.
    - **Xcode 7.3.1** or later
    - **iOS SDK 9.3** or later
    - **Command Line Tools**

4. tvOS builds need these extra packages and tools.
    - **Xcode 7.3.1** or later
    - **tvOS SDK 9.2** or later
    - **Command Line Tools**

#### 5.2 Build Scripts
Use `android.sh`, `ios.sh` and `tvos.sh` to build MobileFFmpeg for each platform.

After a successful build, compiled FFmpeg and MobileFFmpeg libraries can be found under `prebuilt` directory.

##### 5.2.1 Android
```
export ANDROID_HOME=<Android SDK Path>
export ANDROID_NDK_ROOT=<Android NDK Path>
./android.sh
```

<img src="https://github.com/tanersener/mobile-ffmpeg/raw/master/docs/assets/android_custom.gif" width="600">

##### 5.2.2 iOS
```
./ios.sh
```

<img src="https://github.com/tanersener/mobile-ffmpeg/raw/master/docs/assets/ios_custom.gif" width="600">

##### 5.2.3 tvOS
```
./tvos.sh
```

<img src="https://github.com/tanersener/mobile-ffmpeg/raw/master/docs/assets/tvos_custom.gif" width="600">

#### 5.3 GPL Support
It is possible to enable GPL licensed libraries `x264`, `xvidcore` since `v1.1` and `vid.stab`, `x265` since `v2.1` 
from the top level build scripts. Their source code is not included in the repository and downloaded when enabled.

#### 5.4 External Libraries
`build` directory includes build scripts of all external libraries. Two scripts exist for each external library, 
one for `Android` and one for `iOS / tvOS`. Each of these two scripts contains options/flags used to cross-compile the 
library on the specified mobile platform. 

### 6. Documentation

A more detailed documentation is available at [Wiki](https://github.com/tanersener/mobile-ffmpeg/wiki).

### 7. License

This project is licensed under the LGPL v3.0. However, if source code is built using optional `--enable-gpl` flag or 
prebuilt binaries with `-gpl` postfix are used then MobileFFmpeg is subject to the GPL v3.0 license.

Source code of FFmpeg and external libraries is included in compliance with their individual licenses.

`openh264` source code included in this repository is licensed under the 2-clause BSD License but this license does 
not cover the `MPEG LA` licensing fees. If you build `mobile-ffmpeg` with `openh264` and distribute that library, then 
you are subject to pay `MPEG LA` licensing fees. Refer to [OpenH264 FAQ](https://www.openh264.org/faq.html) page for 
the details. Please note that `mobile-ffmpeg` does not publish a binary with `openh264` inside.

`strip-frameworks.sh` script included and distributed (until v4.x) is published under the [Apache License version 2.0](https://www.apache.org/licenses/LICENSE-2.0).

In test applications; embedded fonts are licensed under the [SIL Open Font License](https://opensource.org/licenses/OFL-1.1), other digital assets are published in the public domain.

Please visit [License](https://github.com/tanersener/mobile-ffmpeg/wiki/License) page for the details.

### 8. See Also

- [libav gas-preprocessor](https://github.com/libav/gas-preprocessor/raw/master/gas-preprocessor.pl)
- [FFmpeg API Documentation](https://ffmpeg.org/doxygen/4.0/index.html)
- [FFmpeg Wiki](https://trac.ffmpeg.org/wiki/WikiStart)
- [FFmpeg License and Legal Considerations](https://ffmpeg.org/legal.html)
- [FFmpeg External Library Licenses](https://www.ffmpeg.org/doxygen/4.0/md_LICENSE.html)