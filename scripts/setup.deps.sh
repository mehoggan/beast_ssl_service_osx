BUILD_DIR="$(pwd)/build"

if [[ -e ${BUILD_DIR} ]];
then
  rm -rf "${BUILD_DIR}"
fi

mkdir "${BUILD_DIR}"

function openssl {
  echo "Setting up openssl in ${BUILD_DIR}"
  ./scripts/clone.openssl.sh \
    "${BUILD_DIR}" \
    "https://github.com/openssl/openssl.git"

  ./scripts/configure.openssl.sh \
    "${BUILD_DIR}" \
    "Debug"

  ./scripts/build.openssl.sh \
    "${BUILD_DIR}"

  ./scripts/install.openssl.sh \
    "${BUILD_DIR}"
}
openssl

function boostbeast {
  echo "Setting up boostbeast in ${BUILD_DIR}"
  ./scripts/clone.boostbeast.sh \
    "${BUILD_DIR}" \
    "https://github.com/boostorg/boost.git"

  ./scripts/configure.boostbeast.sh \
    "${BUILD_DIR}"

  ./scripts/build.boostbeast.sh \
    "${BUILD_DIR}"

  ./scripts/install.boostbeast.sh \
    "${BUILD_DIR}"
}
boostbeast
