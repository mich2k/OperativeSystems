#!/bin/sh

cd $1   # mi sposto nella dir (questo per ogni ricorsione)
        # assegno i nomi ai parametri
word=$2
tempfile=$3
    
#echo sono in $1    echo di debugging
for h in *; do
    case $h in
        *.$word) # verifico se l elemento corrente fa match con wildcard.$word, che si espanderà a un qualsiasi testo prima del punto e alla variabile word (ovvero la terminazione che cerchiamo) dopo il punto
                if [ -f $h ];then    # verifico sia effettivamente un file, e siccome non è richiesto di visualizzare il suo contenuto non attuo la verifica di leggibilità con -r
                    #echo trovato $h   echo di debug
                    echo $(pwd) >> $tempfile    # metto in APPEND nel tempfile
                    break   # una volta trovato il file compatibile in questa directory, potenzialmente potrebbero essercene anche altri, ma nella traccia è indicato di riportare le directory che ne abbiano almeno uno
                fi
                ;;
        *);;
    esac
done



for dir in *;do   # espoloro gli elementi nella dir corrente
    if [ -d $dir -a -x $dir ];then  # verifico se l elemento considerato è una directory ed ho il permesso di attraversarla (-x, eseguibilità)
        $0 $(pwd)/$dir $word $tempfile # in caso affermativo richiamo lo script con $0, passo la dir corrente aggiunta alla dir trovata più tutti gli altri parametri necessari
    fi
done
