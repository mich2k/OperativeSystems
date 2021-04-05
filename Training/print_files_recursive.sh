#!/usr/bin/bash

# accettato un parametro $1, ovvero directory

PATH=$PATH:$(pwd)
export PATH
echo $1
cd $1


for file in *; do

if [ -f $file ]; then
    echo $file è un file
fi

if [ -d $file ]; then
    echo $(pwd)/$file
    $0 $(pwd)/$file
fi


done
