set -x

BUILD_DIR=${1}

MAKE_CMD=$(which make)
echo "Going to build openssl in ${BUILD_DIR} with ${MAKE_CMD}..."

cd ${BUILD_DIR}/openssl && \
  export CC="/usr/bin/clang" && \
  export CXX="/usr/bin/clang++" && \
  export AR="/usr/bin/ar" && \
  export RANLIB="/usr/bin/ranlib" && \
  export LD="/usr/bin/ld" && \
  export LIBTOOL="/usr/bin/libtool" && \
  export MAKE="/usr/bin/make" && \
  export CFLAGS="-Wno-unknown-warning-option" && \
  /usr/bin/make clean all -j $(sysctl -n hw.ncpu)
