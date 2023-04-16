#!/bin/bash
set -eu


# shellcheck disable=SC2039
declare -a NECESSARY=( "build-essential cmake" "libpcap-dev" "libstdc++6" )
# shellcheck disable=SC2112
function installOrUpdate() {
    for value in "$@" # Используйте здесь "$@", а не "$*" !!!!!
        do
            PKG_OK=$(dpkg-query -W --showformat='${Status}\n' $value|grep "install ok installed")
            echo Checking for $value: $PKG_OK
            if [ "" = "$PKG_OK" ]; then
              echo "No $value. Setting up $value."
              sudo apt-get --yes install $value
            fi

        done
}

# shellcheck disable=SC2039
installOrUpdate "${NECESSARY[@]}"

cmake -S . -B cmake-build-debug
cd ./cmake-build-debug
cmake --build .
cd ../
sudo chmod a+x Analyser