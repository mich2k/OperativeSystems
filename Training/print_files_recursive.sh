#!/usr/bin/bash

# accettato un parametro $1, ovvero directory


cd $1


for file in *; do

if [ -f $file ]; then
    echo $file è un file in $(pwd)
fi
done

for i in *;do   # ricorsione
    if [ -d $i -a -x $i ];then
            $0 $(pwd)/$i $2 $3
        fi
done
