#! /usr/bin/sh

# GOAL: stampo tutti i file regolari (non directory) nella working path corrente

#todo: controllo input

cd $1
for i in *; do

if  [ ! -f $i ]; then
continue
fi

echo $i file

done
