#! /bin/sh

#cd out/build ; make
# shellcheck disable=SC2164
cd out/build ; cmake --build . -- -lboost_log
