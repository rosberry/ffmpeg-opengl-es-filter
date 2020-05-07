**Вступление**

Этот репозиторий создан для добавления кастомного фильтра, написанного изначально на OpenGL и портированного затем на OpenGL ES.

**Добавление в проект**

[MobileFFmpeg]((https://github.com/tanersener/mobile-ffmpeg)) позволяет собирать ffmpeg для Android, iOS и tvOS.

Собирается проект при помощи скрипта `android.sh`, который лежит в директории `mobile-ffmpeg`.

Для сборки необходимо следующее:

- **Android SDK 4.1 Jelly Bean (API Level 16)** или позднее
- **Android NDK r20** or later with LLDB and CMake


**Процесс сборки**

В текущем проекте уже добавлен кастомный фильтр `vf_gltransition.c`
Инструкция по добавлению находится [здесь](https://github.com/transitive-bullshit/ffmpeg-gl-transition#building-ffmpeg).

1. Для начала необхидимо экспортировать переменные: 
```
export ANDROID_HOME=<Android SDK Path>
export ANDROID_NDK_ROOT=<Android NDK Path> 
```

2. Сборка FFmpeg представлена в двух вариантах: `Main Release` и `LTS Release`. Если minSdKVersion проекта равна 24 и более, то стоит использовать `Main Release`. Подробнее [тут.](https://github.com/transitive-bullshit/ffmpeg-gl-transition#building-ffmpeg)

3. По умолчанию сборка ffmpeg происходит для всех платформ (`arm-v7a`, `arm-v7a-neon`, `arm64-v8a`, `x86`, `x86-64`), что существенно увеличивает время сборки. Для тестирования я выбрать одну платформу и отключить остальные, прописав при вызове скрипта `disable-<название>` для каждой из них.

4. MobileFFMpeg поддерживает множество дополнительных библиотек, прописав в параметрах, например, `enable-x264` для подключения libx264.

5. Для сборки ffmpeg использовался следующий скрипт: `./android.sh --lts --enable-gpl --enable-x264 --enable-libpng`

6. После завершения сборки ffmpeg вcе сгенерированные so-шники находятся в директории `prebuilt`. Также для Android генерируется `.aar`-файл, который в последствии будет использован для подключения `MobileFFmpeg` в наш проект.

7. Для добавления FFMpeg необходимо сделать следующее: 
    - Добавить проект `android` из директории `mobile-ffmpeg` как модуль в свой проект
    - Добавить архив `mobile-ffmpeg.aar` в свой проект, используя `File` -> `New` -> `New Module` -> `Import .JAR/.AAR Package menu`.
    - Добавить зависимость к модулю `mobile-ffmpeg` через `Project structure`.
    
8. При любом изменении в файле `vf_gltransition` необходимо заново собрать `MobileFFmpeg` и заменить текущий `mobile-ffmpeg.aar` на новый.
