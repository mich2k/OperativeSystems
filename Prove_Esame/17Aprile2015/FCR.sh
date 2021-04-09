#!/usr/bin/sh

cd $1
tempfile=$3
X=$2
files=

for curr_file in *;do
    if [ -f $curr_file -a -r $curr_file ];then
        #echo $curr_file
        compat_lines=$(grep 't$' < $curr_file | wc -l)
        if [ "$compat_lines" = "$X" ]; then
            #echo $(pwd)/$curr_file
            echo inserire la K-esima linea da mostrare per $curr_file
            read K
            expr $K + 0 > /dev/null 2>&1
            if [ ! $? -eq 2 -a ! $? -eq 3 ]; then
            #echo $1 e\' numerico
            if [ ! $K -ge 0 ]; then
                echo $K non e\' strettamente positivo
                rm $tempfile
                exit 5
            elif [ $K -ge $X ]; then
                echo $K deve essere strettamente minore di $X
                rm $tempfile
                exit 6
            fi
            else
                echo $1 non numerico
                exit 7
            fi
            echo $K linea: $(awk NR==$K < $curr_file)
            echo $(pwd)/$curr_file >> $3
        fi
    fi
done


for i in *;do   # ricorsione
    if [ -d $i -a -x $i ];then
        $0 $(pwd)/$i $2 $tempfile
    fi
done
