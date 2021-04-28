#!/usr/bin/sh

cd $1
# $2 = dir_to_find
# $3 = lines_req
count=0
files=

#case $i in:

#esac
for i in *; do
        if [ -r $i ]
            curr_lines=$(wc -l < $i)
            if [ $curr_lines -eq $3 ]; then
                files="$files $i"
                count=$(expr $count + 1)
            fi
        fi
    fi
done

if [ $count -ge 1 ]; then
    echo directory compatibile: $(pwd) con $files
    echo invoco la parte in C
    main $files $3
fi

for i in *;do   # ricorsione
    if [ -d $i ];then
        if [ -x $i ];then
            if [ "$i" = "$2" ];then
                $0 $(pwd)/$i $2 $3
            fi
        fi
    fi
done
