#!/usr/bin/sh


for elem in p* ; do
    echo $elem
    if [ -r $elem ]; then
    cat < $elem
    fi
done
