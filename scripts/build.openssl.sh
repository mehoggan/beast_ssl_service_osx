set -x

BUILD_DIR=${1}
SUB_FOLDER=${2}

MAKE_CMD=$(which make)
echo "Going to build openssl in ${BUILD_DIR}/${SUB_FOLDER} with ${MAKE_CMD}..."
echo "PWD=${PWD}"

cd ${BUILD_DIR}/${SUB_FOLDER} && \
  export CC="/usr/bin/clang" && \
  export CXX="/usr/bin/clang++" && \
  export AR="/usr/bin/ar" && \
  export RANLIB="/usr/bin/ranlib" && \
  export LD="/usr/bin/ld" && \
  export LIBTOOL="/usr/bin/libtool" && \
  export MAKE="/usr/bin/make" && \
  export CFLAGS="-Wno-unknown-warning-option" && \
  ${MAKE_CMD} -j $(sysctl -n hw.ncpu)
