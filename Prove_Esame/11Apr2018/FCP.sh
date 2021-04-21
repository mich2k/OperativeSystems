#!/bin/sh
# working test: FCP.sh 5 $(pwd)     ho commentato il controllo sul >= N+2
Y=$1

if [ ! $# -ge 3 ];then
    echo richiesti N+1 parametri con N ge 2, non $#
    #exit 1 
fi

expr $Y + 0 > /dev/null 2>&1
if [ ! $? -eq 2 -a ! $? -eq 3 ]; then
    echo $Y e\' numerico
    if [ ! $Y -ge 0 ]; then
        echo $Y non e\' strettamente positivo
        exit 2
    fi
else
    echo $Y non numerico
    exit 3
fi

shift

for ger in $*; do
    case $ger in
        /*)
            if [ ! -d $ger -o ! $ger ];then
                echo $ger non e\' una dir o non si hanno i permessi adatti
                exit 4
            fi
            ;;
        *)
            echo $ger non e\' una dir assoluta
            exit 5
            ;;
    esac
done

tempfile=/tmp/tmp$$
> $tempfile

PATH=$(pwd):$PATH
export PATH

for ger in $*; do
    FCR.sh $ger $Y $tempfile
done

echo sono stati creati in totale $(cat $tempfile | wc -l) file\\files!

for elem in $(cat $tempfile);do
    echo trovato $elem!
done

rm $tempfile