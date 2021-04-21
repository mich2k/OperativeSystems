#!/usr/bin/sh

i=0
for elem in $1; do  #in filename param

if [ -d $elem ]; then
    echo "[D] $elem"
elif [ -f $elem ]; then
    echo "[F] $elem"
fi
i=$((i+1))

done

echo sono stati trovati $i file + dirs
