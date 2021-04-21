#!/usr/bin/sh


for elem in .* ; do
    echo $elem
    ls -dlA $elem
done
