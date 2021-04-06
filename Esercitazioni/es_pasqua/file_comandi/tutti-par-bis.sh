#!/usr/bin/sh

# non ho idea di cosa questo esercizio voglia

i=1

if [ ! $# -ge 1 ]; then
    echo error, params not ge than 1
    exit 1
fi

while [ $i -ne $# ]; do
echo ${@:$i:1} - $i
i=$(expr $i + 1)
done
