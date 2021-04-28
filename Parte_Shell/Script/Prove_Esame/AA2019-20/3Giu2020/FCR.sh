#!/usr/bin/sh

cd $1


for D in *;do
    case $D in
        $2?$2)
            if [ -d $D -a -x $D ];then
                echo $D trovata!
                echo $(pwd)/$D >> $3
            fi;;
        *);;
    esac
done


for dir in *;do   # ricorsione
    if [ -d $dir -a -x $dir ];then
        $0 $(pwd)/$dir $C $3
    fi
done
