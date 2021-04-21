#!/bin/sh

cd $1   # mi sposto nella dir (questo per ogni ricorsione)
        # assegno i nomi ai parametri

for f in *; do
    if [ -f $f -a -r $f ];then
        curr_lines=$( wc -l < $f )
    fi
done



for dir in *;do   # espoloro gli elementi nella dir corrente
    if [ -d $dir -a -x $dir ];then  # verifico se l elemento considerato è una directory ed ho il permesso di attraversarla (-x, eseguibilità)
        $0 $(pwd)/$dir $Y $tempfile # in caso affermativo richiamo lo script con $0, passo la dir corrente aggiunta alla dir trovata più tutti gli altri parametri necessari
    fi
done
