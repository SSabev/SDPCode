#! /bin/bash
# This script downloads and sets up the premake4 program.

if [ -f premake4 ]
	then
		echo "premake4 appears to be already present."
		exit 0
else
	wget http://pankoi.com/sdp/premake4 -O premake4

	chmod +x premake4
fi
