#! /bin/bash

# Compile cpptest
pushd libs/cpptest > /dev/null

unzip cpptest
./configure
make

popd > /dev/null
