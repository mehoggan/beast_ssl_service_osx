set -x

BUILD_DIR=${1}
SUB_FOLDER=${2}

MAKE_CMD=$(which make)
echo "Going to build boostbeast in ${BUILD_DIR} with ${MAKE_CMD}..."

PYTHON_INCLUDES="/Library/Frameworks/Python.framework/Versions/3.8"
PYTHON_INCLUDES="${PYTHON_INCLUDES}/include/python3.8"
cd ${BUILD_DIR}/${SUB_FOLDER} && \
  export CC="/usr/bin/clang" && \
  export CXX="/usr/bin/clang++" && \
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
  export C_INCLUDE_PATH="${PYTHON_INCLUDES}:${C_INCLUDE_PATH}" && \
  export CPLUS_INCLUDE_PATH="${PYTHON_INCLUDES}:${CPLUS_INCLUDE_PATH}" && \
  ./b2 -j$(sysctl -n hw.ncpu) cxxstd=14
