#!/bin/sh
#imposto lo shabang compatibile 

#assegno le nomenglature indicate nella traccia rispetto i parametri inseriti

#working test: FCP.sh giusto $(pwd)/dir

if [ ! $# -ge 3 ];then
    echo richiesti almeno N+1 parametri, non $#
    #exit 1
fi

case $1 in
    */*)
        echo errore, $1 deve essere un nome relativo semplice di un file
        exit 2
        ;;
    *);;
esac

F=$1
shift

for ger in $*; do
    case $ger in
        /*)
            if [ ! -d $ger -o ! -x $ger ];then
                echo $ger non e\' una directory valida o non si hanno i permessi necessari
                exit 3
            fi
            ;;
        *)
            echo $ger non e\' una directory assoluta
            exit 4;;
    esac
done


tempfile=/tmp/tmp$$
>$tempfile

PATH=$(pwd):$PATH
export PATH

for ger in $*; do

    FCR.sh $ger $F $tempfile

done

echo sono stati creati $(wc -l < $tempfile) file\\s


for row in $(cat $tempfile);do
    
    echo sto considerando:
    echo path assoluta: $row
    echo nome file: $(basename $row)
    echo riporto la sua prima linea..
    head -1 < $row
    echo riporto la sua ultima linea..
    tail -1 < $row
done

#æggiungere tempfile commentato

rm $tempfile
