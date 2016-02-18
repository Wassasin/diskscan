#!/bin/bash
set -e
set +x

rm -rf build
mkdir -p build
pushd build

CC="clang" CXX="clang++" cmake -DCMAKE_BUILD_TYPE=RELEASE ..
popd

