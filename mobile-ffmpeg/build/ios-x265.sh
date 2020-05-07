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
LIB_NAME="x265"
set_toolchain_clang_paths ${LIB_NAME}

# PREPARING FLAGS
TARGET_HOST=$(get_target_host)
CFLAGS=$(get_cflags ${LIB_NAME})
CXXFLAGS=$(get_cxxflags ${LIB_NAME})
LDFLAGS=$(get_ldflags ${LIB_NAME})

# USE CLEAN SOURCE ON EACH BUILD
cd ${BASEDIR}/src || exit 1
rm -rf ${LIB_NAME} || exit 1
DOWNLOAD_RESULT=$(download_gpl_library_source ${LIB_NAME})
if [[ ${DOWNLOAD_RESULT} -ne 0 ]]; then
    exit 1
fi
cd ${BASEDIR}/src/${LIB_NAME} || exit 1

ASM_OPTIONS=""
case ${ARCH} in
    armv7 | armv7s)
        ASM_OPTIONS="-DENABLE_ASSEMBLY=1 -DCROSS_COMPILE_ARM=1"
    ;;
    arm64 | arm64e)
        ASM_OPTIONS="-DENABLE_ASSEMBLY=0 -DCROSS_COMPILE_ARM=1"
    ;;
    *)
        ASM_OPTIONS="-DENABLE_ASSEMBLY=1 -DCROSS_COMPILE_ARM=0"
    ;;
esac

if [ -d "cmake-build" ]; then
    rm -rf cmake-build
fi

mkdir cmake-build || exit 1
cd cmake-build || exit 1

# fix x86 and x86_64 assembly
${SED_INLINE} 's/win64/macho64 -DPREFIX/g' ${BASEDIR}/src/x265/source/cmake/CMakeASM_NASMInformation.cmake
${SED_INLINE} 's/win/macho/g' ${BASEDIR}/src/x265/source/cmake/CMakeASM_NASMInformation.cmake

# fix pointer array assignments
${SED_INLINE} '/addAvg/s/ p.pu/ *p.pu/g' ${BASEDIR}/src/x265/source/common/arm/asm-primitives.cpp
${SED_INLINE} '/convert_p2s/s/ p.pu/ *p.pu/g' ${BASEDIR}/src/x265/source/common/arm/asm-primitives.cpp
${SED_INLINE} '/pixelavg_pp/s/ p.pu/ *p.pu/g' ${BASEDIR}/src/x265/source/common/arm/asm-primitives.cpp
${SED_INLINE} '/addAvg/s/ p.chroma/ *p.chroma/g' ${BASEDIR}/src/x265/source/common/arm/asm-primitives.cpp
${SED_INLINE} '/add_ps/s/ p.chroma/ *p.chroma/g' ${BASEDIR}/src/x265/source/common/arm/asm-primitives.cpp
${SED_INLINE} '/add_ps/s/ p.cu/ *p.cu/g' ${BASEDIR}/src/x265/source/common/arm/asm-primitives.cpp
${SED_INLINE} '/blockfill_s/s/ p.cu/ *p.cu/g' ${BASEDIR}/src/x265/source/common/arm/asm-primitives.cpp
${SED_INLINE} '/ssd_s/s/ p.cu/ *p.cu/g' ${BASEDIR}/src/x265/source/common/arm/asm-primitives.cpp
${SED_INLINE} '/calcresidual/s/ p.cu/ *p.cu/g' ${BASEDIR}/src/x265/source/common/arm/asm-primitives.cpp
${SED_INLINE} '/scale1D_128to64_neon/s/ p.scale/ *p.scale/g' ${BASEDIR}/src/x265/source/common/arm/asm-primitives.cpp

# fixing constant shift
${SED_INLINE} 's/lsr 16/lsr #16/g' ${BASEDIR}/src/x265/source/common/arm/blockcopy8.S

# fixing leading underscores
${SED_INLINE} 's/function x265_/function _x265_/g' ${BASEDIR}/src/x265/source/common/arm/*.S
${SED_INLINE} 's/ x265_/ _x265_/g' ${BASEDIR}/src/x265/source/common/arm/pixel-util.S

# fixing relocation errors
${SED_INLINE} 's/sad12_mask:/sad12_mask_bytes:/g' ${BASEDIR}/src/x265/source/common/arm/sad-a.S
${SED_INLINE} 's/g_lumaFilter:/g_lumaFilter_bytes:/g' ${BASEDIR}/src/x265/source/common/arm/ipfilter8.S
${SED_INLINE} 's/g_chromaFilter:/g_chromaFilter_bytes:/g' ${BASEDIR}/src/x265/source/common/arm/ipfilter8.S
${SED_INLINE} 's/\.text/.equ sad12_mask, .-sad12_mask_bytes\
\
.text/g' ${BASEDIR}/src/x265/source/common/arm/sad-a.S
${SED_INLINE} 's/\.text/.equ g_lumaFilter, .-g_lumaFilter_bytes\
.equ g_chromaFilter, .-g_chromaFilter_bytes\
\
.text/g' ${BASEDIR}/src/x265/source/common/arm/ipfilter8.S

# using customized build file
rm -f ${BASEDIR}/src/${LIB_NAME}/source/CMakeLists.txt || exit 1
cp ${BASEDIR}/tools/cmake/CMakeLists.x265.ios.txt ${BASEDIR}/src/${LIB_NAME}/source/CMakeLists.txt || exit 1

cmake -Wno-dev \
    -DCMAKE_VERBOSE_MAKEFILE=0 \
    -DCMAKE_C_FLAGS="${CFLAGS}" \
    -DCMAKE_CXX_FLAGS="${CXXFLAGS}" \
    -DCMAKE_EXE_LINKER_FLAGS="${LDFLAGS}" \
    -DCMAKE_SYSROOT="${SDK_PATH}" \
    -DCMAKE_FIND_ROOT_PATH="${SDK_PATH}" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="${BASEDIR}/prebuilt/$(get_target_build_directory)/${LIB_NAME}" \
    -DCMAKE_SYSTEM_NAME=Generic \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_LINKER="$LD" \
    -DCMAKE_AR="$(xcrun --sdk $(get_sdk_name) -f ar)" \
    -DCMAKE_AS="$AS" \
    -DSTATIC_LINK_CRT=1 \
    -DENABLE_PIC=1 \
    -DENABLE_CLI=0 \
    ${ASM_OPTIONS} \
    -DCMAKE_SYSTEM_PROCESSOR=$(get_target_arch) \
    -DENABLE_SHARED=0 ../source || exit 1

make -j$(get_cpu_count) || exit 1

# MANUALLY COPY PKG-CONFIG FILES
cp x265.pc ${INSTALL_PKG_CONFIG_DIR} || exit 1

make install || exit 1
