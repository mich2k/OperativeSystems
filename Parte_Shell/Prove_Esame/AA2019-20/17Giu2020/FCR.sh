#!/usr/bin/sh

cd $1
B=$2
found_dir=false
found_char=false
files=

for curr_file in *;do
    if [ -f $curr_file -a -r $curr_file ];then
        n_chars=$(wc -c < $curr_file)
        if [ $(expr $n_chars % $B) = 0 ]; then
            #echo $(pwd)/$curr_file
            found_char=true
            files="$files $curr_file"
            
        fi
    fi
    if [ -d $curr_file ];then
        found_dir=true
    fi
done

if [ "$found_dir" = "true" -a "$found_char" = "true" ];then
    echo dir compatibile in $1
    for compat_file in $(echo $files); do
        > $compat_file.Chiara
        #main.c $compat_file $(wc -c < $compat_file) $B
    done
fi

for i in *;do   # ricorsione
    if [ -d $i -a -x $i ];then
        $0 $(pwd)/$i $B
    fi
done
