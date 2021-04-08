#!/usr/bin/sh


cd $1
H=$2
K=$3
cont=0

>/tmp/tmp$$ # ulteriore tmp file per la verifica con cont
subtemp=/tmp/tmp$$

for curr_file in *;do
    if [ -f $curr_file -a -r $curr_file ];then
        if [ $(wc -l < $curr_file) -eq $K ]; then
            cont=$(expr $cont + 1)
            echo $(pwd)/$curr_file >> $subtemp
        fi
    fi
done

#cat $subtemp

if [ $cont = $H ]; then
    cat $subtemp >> $4
fi
if [ -f $subtemp ]; then
    rm $subtemp
fi



for i in *;do   # ricorsione
    if [ -d $i ];then
        if [ -x $i ];then
                $0 $(pwd)/$i $2 $3 $4
        fi
    fi
done
