#!/usr/bin/sh

cd $1
# $2 = K    byte
# $3 = Y    
# ls -dl $i | cut -d ' ' -f 5
files=
count=0

for i in *; do
    if [ -f $i ]; then
        size=$(ls -dl $i | cut -d ' ' -f 5)    # verfico la dimensione
        if [ $size -eq $3 ]; then
            n_not_numbers=$(grep -v [0-9] $i | wc -c)
            if [ $n_not_numbers -eq 0 ]; then  # non ha non numerici
                files="$files $i"
                count=$(expr $count + 1)
            fi
        fi
    fi
done

if [ $count -eq $2 ]; then
    echo directory compatibile: $(pwd)
    echo invoco la parte in C
    main $files
fi

for i in *;do   # ricorsione
    if [ -d $i -a -x $i ]; then
        $0 $(pwd)/$i $2 $3
    fi
done
