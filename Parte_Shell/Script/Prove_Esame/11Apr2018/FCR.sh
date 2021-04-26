#!/bin/sh

cd $1
Y=$2
tempfile=$3

for f in *; do
    if [ -f $f -a -r $f ];then
        type=''
        curr_lines=$( wc -l < $f )
        if [ $curr_lines -ge $Y -a $curr_lines -ge 5 ];then
            > $f.quinta
            echo $(awk 'NR==5' $f) > $f.quinta # head -5 | tail -1
            type=quinta
        else
            >$f.NOquinta
            type=NOquinta
        fi
        echo creato $f.$type!
        echo $(pwd)/$f.$type >> $tempfile

    fi
done



for dir in *;do   # ricorsione
    if [ -d $dir -a -x $dir ];then
        $0 $(pwd)/$dir $Y $tempfile
    fi
done
