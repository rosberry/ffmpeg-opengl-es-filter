#!/bin/bash

if [[ -z ${ARCH} ]]; then
    echo -e "(*) ARCH not defined\n"
    exit 1
fi

if [[ -z ${TARGET_SDK} ]]; then
    echo -e "(*) TARGET_SDK not defined\n"
    exit 1
fi

if [[ -z ${SDK_PATH} ]]; then
    echo -e "(*) SDK_PATH not defined\n"
    exit 1
fi

if [[ -z ${BASEDIR} ]]; then
    echo -e "(*) BASEDIR not defined\n"
    exit 1
fi

# ENABLE COMMON FUNCTIONS
if [[ ${APPLE_TVOS_BUILD} -eq 1 ]]; then
    . ${BASEDIR}/build/tvos-common.sh
else
    . ${BASEDIR}/build/ios-common.sh
fi

# PREPARING PATHS & DEFINING ${INSTALL_PKG_CONFIG_DIR}
LIB_NAME="libwebp"
set_toolchain_clang_paths ${LIB_NAME}

# PREPARING FLAGS
TARGET_HOST=$(get_target_host)
CFLAGS=$(get_cflags ${LIB_NAME})
CXXFLAGS=$(get_cxxflags ${LIB_NAME})
LDFLAGS=$(get_ldflags ${LIB_NAME})

cd ${BASEDIR}/src/${LIB_NAME} || exit 1

if [ -d "build" ]; then
    rm -rf build
fi

mkdir build;
cd build

# OVERRIDING INCLUDE PATH ORDER
CFLAGS="-I${BASEDIR}/prebuilt/$(get_target_build_directory)/giflib/include \
-I${BASEDIR}/prebuilt/$(get_target_build_directory)/jpeg/include \
-I${BASEDIR}/prebuilt/$(get_target_build_directory)/libpng/include \
-I${BASEDIR}/prebuilt/$(get_target_build_directory)/tiff/include $CFLAGS"

cmake -Wno-dev \
    -DCMAKE_VERBOSE_MAKEFILE=0 \
    -DCMAKE_C_FLAGS="${CFLAGS}" \
    -DCMAKE_CXX_FLAGS="${CXXFLAGS}" \
    -DCMAKE_EXE_LINKER_FLAGS="${LDFLAGS}" \
    -DCMAKE_SYSROOT="${SDK_PATH}" \
    -DCMAKE_FIND_ROOT_PATH="${SDK_PATH}" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="${BASEDIR}/prebuilt/$(get_target_build_directory)/${LIB_NAME}" \
    -DCMAKE_SYSTEM_NAME=Darwin \
    -DCMAKE_OSX_SYSROOT="" \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_LINKER="$LD" \
    -DCMAKE_AR="$(xcrun --sdk $(get_sdk_name) -f ar)" \
    -DCMAKE_AS="$AS" \
    -DGIF_INCLUDE_DIR=${BASEDIR}/prebuilt/$(get_target_build_directory)/giflib/include \
    -DGIF_LIBRARY= \
    -DJPEG_INCLUDE_DIR=${BASEDIR}/prebuilt/$(get_target_build_directory)/jpeg/include \
    -DJPEG_LIBRARY=${BASEDIR}/prebuilt/$(get_target_build_directory)/jpeg/lib \
    -DPNG_PNG_INCLUDE_DIR=${BASEDIR}/prebuilt/$(get_target_build_directory)/libpng/include \
    -DPNG_LIBRARY=${BASEDIR}/prebuilt/$(get_target_build_directory)/libpng/lib \
    -DTIFF_INCLUDE_DIR=${BASEDIR}/prebuilt/$(get_target_build_directory)/tiff/include \
    -DTIFF_LIBRARY=${BASEDIR}/prebuilt/$(get_target_build_directory)/tiff/lib \
    -DZLIB_INCLUDE_DIR=${SDK_PATH}/usr/include \
    -DZLIB_LIBRARY=${SDK_PATH}/usr/lib \
    -DGLUT_INCLUDE_DIR= \
    -DGLUT_cocoa_LIBRARY= \
    -DGLUT_glut_LIBRARY= \
    -DOPENGL_INCLUDE_DIR= \
    -DSDLMAIN_LIBRARY= \
    -DSDL_INCLUDE_DIR= \
    -DWEBP_BUILD_CWEBP=0 \
    -DWEBP_BUILD_DWEBP=0 \
    -DWEBP_BUILD_EXTRAS=0 \
    -DWEBP_BUILD_GIF2WEBP=0 \
    -DWEBP_BUILD_IMG2WEBP=0 \
    -DWEBP_BUILD_WEBPMUX=0 \
    -DWEBP_BUILD_WEBPINFO=0 \
    -DWEBP_BUILD_ANIM_UTILS=0 \
    -DCMAKE_SYSTEM_PROCESSOR=$(get_target_arch) \
    -DBUILD_SHARED_LIBS=0 .. || exit 1

make -j$(get_cpu_count) || exit 1

# CREATE PACKAGE CONFIG MANUALLY
create_libwebp_package_config "1.0.2"

make install || exit 1
