#! /usr/bin/env sh

BASEDIR=$(dirname $0)

# source "$BASEDIR/../env"

python "$BASEDIR/forker.py" $@
chmod ug+x /tmp/vision_sock
rm /tmp/vision_sock
echo "Vision quit" >&2

