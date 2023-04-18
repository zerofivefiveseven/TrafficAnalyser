#!/bin/bash
set -eu


# shellcheck disable=SC2039
declare -a NECESSARY=("curl" "git" "build-essential" "cmake" "libpcap-dev" "libstdc++6" )
# shellcheck disable=SC2112
function installOrUpdate() {
  echo ------Start Shell Script------
    for value in "$@" # Используйте здесь "$@", а не "$*" !!!!!
        do
            echo Checking for $value
            sudo apt install -y $value
        done
}

# shellcheck disable=SC2039
sudo apt update
installOrUpdate "${NECESSARY[@]}"

cmake -S . -B cmake-build-release
cd ./cmake-build-release
cmake --build .
cd ../
sudo chmod a+x Analyser