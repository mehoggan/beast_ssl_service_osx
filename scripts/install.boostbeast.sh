BUILD_DIR=${1}
SUB_FOLDER=${2}

echo "Going to install boostbeast..."

PYTHON_INCLUDES="/Library/Frameworks/Python.framework/Versions/3.8"
PYTHON_INCLUDES="${PYTHON_INCLUDES}/include/python3.8"
cd ${BUILD_DIR}/${SUB_FOLDER} &&
  export CC="/usr/bin/clang" && \
  export AR="/usr/bin/ar" && \
  export RANLIB="/usr/bin/ranlib" && \
  export LD="/usr/bin/ld" && \
  export LIBTOOL="/usr/bin/libtool" && \
  export MAKE="/usr/bin/make" && \
  export C_INCLUDE_PATH="${PYTHON_INCLUDES}:${C_INCLUDE_PATH}" && \
  export CPLUS_INCLUDE_PATH="${PYTHON_INCLUDES}:${CPLUS_INCLUDE_PATH}" && \
  ./b2 install
