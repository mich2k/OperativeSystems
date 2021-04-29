#!/usr/bin/sh

cd $1



for curr_file in *;do
    if [ -f $curr_file -a -r $curr_file ];then
        echo $curr_file
        lines=$(wc -l < $curr_file)
        if [ "$lines" = "$2" ]; then
            echo $(pwd) compatibile
            echo $(pwd)/$curr_file >> $3
        fi
    fi
done


for i in *;do   # ricorsione
    if [ -d $i -a -x $i ];then
        $0 $(pwd)/$i $2 $3
    fi
done
