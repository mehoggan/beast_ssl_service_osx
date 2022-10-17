BUILD_DIR=${1}
SUB_FOLDER=${2}

echo "Going to configure boostbeast in ${BUILD_DIR}..."

cd ${BUILD_DIR}/${SUB_FOLDER} && \
  export CC="/usr/bin/clang" && \
  export AR="/usr/bin/ar" && \
  export RANLIB="/usr/bin/ranlib" && \
  export LD="/usr/bin/ld" && \
  export LIBTOOL="/usr/bin/libtool" && \
  export MAKE="/usr/bin/make" && \
  export CFLAGS="-Wno-unknown-warning-option" && \
  export OPENSSL_ROOT="${BUILD_DIR}/openssl" && \
  export OPENSSL_ROOTDIR="${BUILD_DIR}/openssl" && \
  export OPENSSL_INCLUDE_DIR="${BUILD_DIR}/openssl/include" && \
  export OPENSSL_LIBRARIES="${BUILD_DIR}/openssl/lib" && \
  ./bootstrap.sh \
    --prefix=${BUILD_DIR}/boostbeast \
    --with-toolset=clang
