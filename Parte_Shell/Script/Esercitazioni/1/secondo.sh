#!/usr/bin/bash

cd $1
if [ -f $2 ]
then
echo il file $2 si trova in $(pwd)
fi
for i in *; do     #   for i in *      ->  considera tutti i file regolari
                #   for i in .*     ->  considera tutti i file nascosti
                #   for i in .* *   ->  considera entrambi
if [ -d $i -a -x $i ]; then     
    echo "Esploro directory $(pwd)/$i"
    secondo.sh $(pwd)/$i $2         # deve essere relativa semplice o qui dobbiamo          
fi                                  # rimuovere $(pwd)

done
