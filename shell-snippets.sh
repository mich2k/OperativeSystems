#!/bin/sh
#!/bin/bash

# CONTROLLO VALIDITA' NUMERO

expr $1 + 0 > /dev/null 2>&1    # mettere il $val giusto
if [ ! $? -eq 2 -a ! $? -eq 3 ];then
    echo $1 e\' numerico
    if [ ! $1 -ge 0 ]; then # in questo caso controlla che sia > 0
        echo $1 non e\' strettamente positivo
        exit 1  # attenzione
    fi
else
    echo $1 non numerico
    exit 2  # attenzione
fi

case $a in
    *)echo a;;
esac

# PARTE TERMINALE FCP.sh

PATH=$(pwd):$PATH
export PATH

tempfile=/tmp/tmp$$
>$tempfile
# da qui inizio la parte sulla/e gerarchie/fasi


# ALGORITMO DEL TIPO: trovare dir chiamata $dir_name in una directory O più gerarchie/directory
# E' NECESSARIO USARE IL CASE!

case $1 in
    */$2)   #echo dir con nome compatibile in $(pwd), verifico se esiste un file compatibile..
        for current_file_in_found_dir in *; do 
            if [ -f $current_file_in_found_dir -a -r $current_file_in_found_dir ]; then     # -r NON verifica che è anche un file, verifica solo se si hanno i perm. di read (al contrario di quanto scritto sul man.)
                # controlli necessari per verificare il file sia compatbile o altro
            fi
        done
esac




# TEMPLATE SCHELETRO FILE RICORSIVO

  
#!/usr/bin/sh

cd $1

for file in *; do
    if [ -f $file -a -r $file ];then
        # operazione sul file_current
    fi
done

for dir in *;do
    if [ -d $dir -a -x $dir ];then
        # $dir è il basename di tutte le subdir che sto esplorando
        $0 $(pwd)/$dir $2
    fi
done


# FINE TEMPLATE SCHELETRO FILE RICORSIVO

