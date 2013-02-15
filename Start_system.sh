#!/bin/bash

# vision
source venv/env/bin/activate
./SDPCode/vision/runvision.sh &

#Java server
export NXJ_HOME=/group/teaching/sdp/archive/SDP2012/sdp3/secure/lejos_nxj
export PATH=/group/teaching/sdp/archive/SDP2012/sdp3/secure/lejos_nxj/bin:$PATH
export LD_LIBRARY_PATH=/group/teaching/sdp/archive/SDP2012/sdp3/secure/bluez/lib:$LD_LIBRARY_PATH

pushd SDPCode/Comms > /dev/null

#nxjpcc Server.java
nxjpc Server &

popd > /dev/null

#Compile main app
mkdir ~/Infinity
cd ~/Infinity
qmake-qt4 /group/teaching/sdp/sdp8/SDPCode/Infinity.pro -r -spec  linux-g++
make
# Start main app
./Infinity

