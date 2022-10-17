BUILD_DIR="$(pwd)/make_build"

if [[ -e ${BUILD_DIR} ]];
then
  rm -rf "${BUILD_DIR}"
fi

mkdir "${BUILD_DIR}"

function build_openssl {
  echo "Setting up openssl in ${BUILD_DIR}"
  ./scripts/clone.openssl.sh \
    "${BUILD_DIR}" \
    "https://github.com/openssl/openssl.git" \
    "openssl"

  ./scripts/configure.openssl.sh \
    "${BUILD_DIR}" \
    "Debug" \
    "openssl"

  ./scripts/build.openssl.sh \
    "${BUILD_DIR}" \
    "openssl"

  ./scripts/install.openssl.sh \
    "${BUILD_DIR}" \
    "openssl"
}
build_openssl

function build_boostbeast {
  echo "Setting up boostbeast in ${BUILD_DIR}"
  ./scripts/clone.boostbeast.sh \
    "${BUILD_DIR}" \
    "https://github.com/boostorg/boost.git" \
    "boostbeast"

  ./scripts/configure.boostbeast.sh \
    "${BUILD_DIR}" \
    "boostbeast"

  ./scripts/build.boostbeast.sh \
    "${BUILD_DIR}" \
    "boostbeast"

  ./scripts/install.boostbeast.sh \
    "${BUILD_DIR}" \
    "boostbeast"
}
build_boostbeast
