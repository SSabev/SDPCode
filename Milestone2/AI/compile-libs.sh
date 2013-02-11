#! /bin/bash

# Compile cpptest
pushd libs/cpptest > /dev/null

unzip cpptest
./configure
make

popd > /dev/null

# Copy the libs to a higher-level location
pushd libs > /dev/null

mkdir include

cp cpptest/src/*.h include
cp cpptest/src/.libs/libcpptest.so* ./

popd > /dev/null
