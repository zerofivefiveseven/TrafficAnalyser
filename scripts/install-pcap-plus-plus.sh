#!/usr/bin/env bash
set -eu


REQUIRED_PKG="libpcap-dev"
PKG_OK=$(dpkg-query -W --showformat='${Status}\n' $REQUIRED_PKG|grep "install ok installed")
echo Checking for $REQUIRED_PKG: $PKG_OK
if [ "" = "$PKG_OK" ]; then
  echo "No $REQUIRED_PKG. Setting up $REQUIRED_PKG."
  sudo apt-get --yes install $REQUIRED_PKG
fi
REQUIRED_PKG2="libstdc++6"
PKG_OK=$(dpkg-query -W --showformat='${Status}\n' $REQUIRED_PKG2|grep "install ok installed")
echo Checking for $REQUIRED_PKG: $PKG_OK
if [ "" = "$PKG_OK" ]; then
  echo "No $REQUIRED_PKG. Setting up $REQUIRED_PKG."
  sudo apt-get --yes install $REQUIRED_PKG
fi



SAVED_PWD="$(pwd -P)"
# shellcheck disable=SC2046
HERE=$(dirname $(readlink -e $0))
source "${HERE}/install-helpers.sh"

URL='https://github.com/seladb/PcapPlusPlus.git'

COMMIT='f132cdab061626a3e583fa0ce52e249501e63dfb'

pkgdir="${LOCAL_REPO_PATH}/PcapPlusPlus"
clone_or_update_git_repo "${URL}" "${pkgdir}" "${COMMIT}"

cd "${pkgdir}"

mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX:PATH=${PREFIX} ..
make all install

cd "${SAVED_PWD}"