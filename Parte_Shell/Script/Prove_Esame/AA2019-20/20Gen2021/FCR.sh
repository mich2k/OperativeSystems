#!/usr/bin/sh

cd $1
cont=0
files=

for param in $*;do
    found=false
    for F in *; do
        case $param in
            $F) 
                echo trovato $F
                found=true
                cont=$(expr $cont + 1)
                echo $cont
                files="$files $F"
                break
            ;;
            *);;
        esac
    done
   

done

compare=0
for param in $*; do
    case $param in
    /*);;
    *)  
    compare=$(expr $compare + 1);;
    esac
done

echo COMPARE: $compare CONT: $cont
if [ $cont -eq $compare ]; then
    echo DIR compatibile in $(pwd), sono stati trovati:
    echo $files
    echo chiamo la parte in C..
    #main.c $files
fi


for dir in *;do   # ricorsione
    if [ -d $dir -a -x $dir ];then
         check_num=$(echo $dir | grep -c [0-9] )
         if [ $check_num -ge 1 ]; then
            echo trovata $dir
            $0 $(pwd)/$dir $*
         fi
    fi
done
