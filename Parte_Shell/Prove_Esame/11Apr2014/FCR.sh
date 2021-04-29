#!/usr/bin/sh

cd $1

temp=$2

for curr in *;do
    if [ -f $curr -a -r $curr ]; then #mi assicuro sia un file leggibile
        compat_lines=$(grep -c -v '^a' $curr)
        if [ $(wc -l < $curr) -eq 0 ];then
            continue
        fi
        echo $compat_lines per $curr
        if [ $compat_lines -eq 0 ];then
            echo $(pwd)/$curr >> $temp
        fi
    fi
done


for dir in *;do   # ricorsione
    if [ -d $dir -a -x $dir ];then
        $0 $(pwd)/$dir $temp
    fi
done
