#!/usr/bin/sh

cd $1

# numero linee su file
nl=

# contatore su quanti file rispettano le specifiche
count=0
# 
files=

for i in *; do
    if [ -f $i ]; then
        nl=$(wc -l < $i)    # verifichiamo n linee OBBLIGATORIO casting
        if [ $nl -eq $3 ]; then
            if grep $3 $i > /dev/null 2>&1; then  # verifico se troviamo il carattere
                files="$files $i"
                count=$(expr $count + 1)
            fi
        fi
    fi
done

if [ $count -eq $2 ]; then
    echo trovata directory $(pwd)
    echo invoco la parte C
    main $files $4 $3
fi

for i in *;do
    if [ -d $i -a -x $i ]; then
        #ricorsiva
        $0 $(pwd)/$1 $2 $3 $4
    fi
done
