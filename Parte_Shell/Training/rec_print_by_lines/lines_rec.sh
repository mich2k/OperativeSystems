#!/usr/bin/sh

cd $1

for file in *; do
    if [ -f $file -a -r $file ];then
        case $file in
            FCR.sh)
                lines=$(grep -c -v '^$' $file)
                if [ $lines -le $2 ];then
                    echo $file in $(pwd)
                fi;;
            *);;
                esac
    fi
done




for dir in *;do
    if [ -d $dir -a -x $dir ];then
        #echo $dir
        #sleep 5
        $0 $(pwd)/$dir $2
    fi
done
