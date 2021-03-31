#! /usr/bin/sh

# GOAL: stampo tutti i file regolari (non directory) nella working path corrente

for i in *; do

if  [ ! -f $i ]; then
continue
fi

echo $i file

done
