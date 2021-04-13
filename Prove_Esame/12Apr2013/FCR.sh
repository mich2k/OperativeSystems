#!/usr/bin/sh

FASE=$4
tempfile=$3
S=$2
G=$1
cd $G
found=false


for curr_file in *;do
    if [ -f $curr_file -a -r $curr_file ];then
    case $curr_file in
        *.$S)
            if [ $FASE = A ];then
                echo $(pwd) >> $tempfile
                found=true
            else
                echo -n "file: \"$curr_file\", prima linea: "
                echo $(head -1 $curr_file)
            fi
            ;;
    esac
    fi
    if [ $found = true -a $FASE = A ];then
        break
    fi
done



for i in *;do   # ricorsione
    if [ -d $i -a -x $i ];then
        #echo $0 $(pwd)/$i $G $S $tempfile
        if [ $FASE = A ];then
            $0 $(pwd)/$i $S $tempfile $FASE
        fi
    fi
done
