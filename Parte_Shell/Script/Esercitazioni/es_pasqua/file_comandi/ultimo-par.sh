#!/usr/bin/sh

echo ${@:$#}
echo ${@:1:$#-1}
