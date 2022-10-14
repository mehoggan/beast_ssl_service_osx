BUILD_DIR=${1}
CONFIG_MODE=${2}

echo "Going to configure openssl in ${BUILD_DIR}..."

if [[ "${CONFIG_MODE}" -eq "Release" ]];
then
  echo "Going to configure openssl in ${CONFIG_MODE} mode..."
  cd ${BUILD_DIR}/openssl && \
    export CC="/usr/bin/clang" && \
    export AR="/usr/bin/ar" && \
    export RANLIB="/usr/bin/ranlib" && \
    export LD="/usr/bin/ld" && \
    export LIBTOOL="/usr/bin/libtool" && \
    export MAKE="/usr/bin/make" && \
    export CFLAGS="-Wno-unknown-warning-option" && \
    ./Configure no-asm -g3 -O0 -fno-omit-frame-pointer -fno-inline-functions \
      shared \
      --prefix=${BUILD_DIR}/openssl \
      --openssldir=${BUILD_DIR}/openssl
else
  echo "Going to configure openssl in ${CONFIG_MODE} mode..."
  cd ${BUILD_DIR}/openssl && \
    export CC="/usr/bin/clang" && \
    export AR="/usr/bin/ar" && \
    export RANLIB="/usr/bin/ranlib" && \
    export LD="/usr/bin/ld" && \
    export LIBTOOL="/usr/bin/libtool" && \
    export MAKE="/usr/bin/make" && \
    export CFLAGS="-Wno-unknown-warning-option" && \
    ./Configure -d no-asm -g3 -O0 -fno-omit-frame-pointer \
      -fno-inline-functions \
      shared \
      --prefix=${BUILD_DIR}/openssl \
      --openssldir=${BUILD_DIR}/openssl
fi
