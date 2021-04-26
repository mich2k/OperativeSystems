#!/bin/sh

cd $1   # mi sposto nella dir (questo per ogni ricorsione)
        # assegno i nomi ai parametri
F=$2
tempfile=$3

for elem in *; do
    case $elem in
        $F)
            if [ -f $elem -a -r $elem ];then
                curr_lines=$( wc -l < $elem )
                sort -f $elem > sorted_$elem
                echo $(pwd)/sorted_$elem > $tempfile
            fi
        ;;
        *);;
    esac
done



for dir in *;do   # espoloro gli elementi nella dir corrente
    if [ -d $dir -a -x $dir ];then  # verifico se l elemento considerato è una directory ed ho il permesso di attraversarla (-x, eseguibilità)
        $0 $(pwd)/$dir $F $tempfile # in caso affermativo richiamo lo script con $0, passo la dir corrente aggiunta alla dir trovata più tutti gli altri parametri necessari
    fi
done
