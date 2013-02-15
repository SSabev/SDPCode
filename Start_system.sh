#!/bin/bash

# vision
source venv/env/bin/activate
./SDPCode/vision/run_vision.sh &

#Java server
export NXJ_HOME=/group/teaching/sdp/archive/SDP2012/sdp3/secure/lejo_nxj
export LD_LIBRARY_PATH=/group/teaching/sdp/archive/SDP2012/sdp3/secure/bluez/lib:$LD_LIBRARY_PATH
nxjpcc SDPCode/Comms/Server.java
nxjpc SDPCode/Comms/Server &

#Compile main app
mkdir ~/Infinity && cd ~/Infinity
qmake-qt4 SDPCode/Infinity.pro -r -spec  linux-g++
make
./Infinity

