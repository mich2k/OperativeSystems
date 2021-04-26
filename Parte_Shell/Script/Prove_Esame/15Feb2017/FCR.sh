#!/usr/bin/sh

X=$2
cd $1
files=' '
found=false

for curr_file in *;do
    if [ -f $curr_file -a -r $curr_file ];then
        echo $curr_file
        if [ $(wc -l < $curr_file) -eq $X ]; then
            n_lines=$(wc -l < $curr_file)
            n_lines_numbers=$(grep -c [0-9] < $curr_file)
            #echo $n_lines in $curr_file
            #echo $n_lines_numbers in $curr_file
            #echo $n_numbers
            if [ "$n_lines_numbers" = "$n_lines" ];then
                #echo $curr_file compatibile
                #if [ ! $found = "true" ]; then
                    echo $(pwd) path compatibile
                #fi
                files="$files $curr_file"
                found=true
            fi
        fi
    fi
done

if [ $found = "true" ]; then
    echo $files
    echo "si vuole passare alla parte in C (s/n)"
    read input
    case $input in
    s | S)
        echo chiamo la parte in C
        #main.c $X $files
        ;;
    *)
        echo non chiamo la parte in C;;
    esac
fi

for i in *;do   # ricorsione
    if [ -d $i -a -x $i ];then
        echo espoloro $i
        $0 $(pwd)/$i $X
    fi
done
