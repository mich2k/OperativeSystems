#!/usr/bin/sh

cd $1
# $2
# $3 = Y    
lines=
count=0

# ALGORITMO DEL TIPO: trovare dir chiamata $dir_name in una directory O più gerarchie/directory
# E' NECESSARIO USARE IL CASE!

case $1 in
    */$2)   #echo dir con nome compatibile in $(pwd), verifico se esiste un file compatibile..
        for current_file_in_found_dir in *; do 
            if [ -f $current_file_in_found_dir -a -r $current_file_in_found_dir ]; then     # -r NON verifica che è anche un file, verifica solo se si hanno i perm. di read (al contrario di quanto scritto sul man.)
                lines=$(wc -l < $current_file_in_found_dir)
                if [ $lines = $3 ]; then
                    echo \'$current_file_in_found_dir\' in $(pwd) ha $lines linee, richieste: $3
                    count=$(expr $count + 1)
                    echo $(pwd)/$current_file_in_found_dir >> $4    # echo, non cat, ci interessa la path per conteggio non il contenuto!
                fi
            fi
        done
esac

if [ $count = $2 ]; then
    echo directory compatibile: $(pwd) con $files
    echo invoco la parte in C
    main $files
fi

for i in *;do   # ricorsione
    if [ -d $i -a -x $i ]; then
        $0 $(pwd)/$i $2 $3 $4
    fi
done
