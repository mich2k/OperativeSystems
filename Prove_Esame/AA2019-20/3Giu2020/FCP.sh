#!/usr/bin/sh

#working test: ./FCP.sh d $(pwd) $(pwd)


C=$1

if [ ! $# -ge 3 ];then
    echo richiesti N+1 parametri non $#
    exit 1
fi

case $1 in
    ?);;
    *) echo $1 deve essere un carattere
        exit 2;;
esac

C=$1
shift

for ger in $*;do
    case $ger in
        /*) if [ ! -d $ger -o ! -x $ger ];then
                echo $ger non dir o permessi non sufficienti
                exit 3
            fi;;
            
        *)  echo $ger non e\' una directory assoluta
            exit 4;;
    esac
done



PATH=$(pwd):$PATH
export PATH
tmpfile=/tmp/nomiAssoluti$$
>$tmpfile


for ger in $*; do

    sh FCR.sh $ger $C $tmpfile


done

echo sono state trovate $(wc -l < $tmpfile) dir compatibili

for curr in $(cat $tmpfile); do
    echo visualizzare il contenuto di $curr?  # non scriverò Elena.
    read answer
    case $answer in
        s | S | y | Y)
            echo visualizzo $curr..
            cd $curr
            echo * .*
            ;;
        *) echo procedo..;;
    esac

done

rm $tmpfile
