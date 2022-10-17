BUILD_DIR=${1}
OPENSSL_REPO=${2}
SUB_FOLDER=${3}

echo "Going to clone openssl to ${BUILD_DIR}..."

git clone --recursive ${OPENSSL_REPO} ${BUILD_DIR}/${SUB_FOLDER}

submodules=(
  'gost-engine_b2b4d629f100eaee9f5942a106b1ccefe85b8808'
  'krb5_aa9b4a2a64046afd2fab7cb49c346295874a5fb6'
  'oqs-provider_3f3d8a8cf331c73a26fe93a598273997cb54eb4f'
  'pyca-cryptography_fa84d185c0fefa7d612759e02a26def2f948fe34'
  'python-ecdsa_4de8d5bf89089d1140eb99aa5d7eb2dc8e6337b6'
  'tlsfuzzer_dbd56c149072e656ca8d6a43a59588f3e7513da2'
  'tlslite-ng_771e9f59d639dbb0e2fa8e646c8e588405d3903e'
  'wycheproof_2196000605e45d91097147c9c71f26b72af58003')

function do_checkout {
  dir=${1}
  sha=${2}
  echo "Going to checkout ${sha} in ${dir}..."
  pushd "${BUILD_DIR}/${SUB_FOLDER}/${dir}"
  git checkout ${sha}
  popd
}

for submodule in "${submodules[@]}";
do
  parts=($(echo ${submodule} | tr "_" " "))
  dir_part=${parts[0]}
  sha_part=${parts[1]}
  echo "Extracted dir=${dir_part} sha=${sha_part}."
  do_checkout ${dir_part} ${sha_part}
done
do_checkout "" "8c7d847e2e6ac6bfded210c19fd8461254bb2be3"
