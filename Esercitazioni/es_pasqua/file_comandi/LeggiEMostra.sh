#!/usr/bin/sh

inp=

for param in $@; do
    echo "visualizzo $param (si/no)?" > /dev/tty 
    read inp
    case $inp in
    si | Si) ls -la $param; less $param;;
    *) echo $param non verra\' considerato;;
    esac
done
