#!/usr/bin/sh

# WORKING TEST: ./FCP.sh $(pwd)/dir z b c d

G=$1

if [ ! $# -ge 3 ]; then
    echo errore richiesti Q+1 param con Q ge 2, non $#
    exit 1
fi

shift

case $G in
    /*)
        if [ ! -d $G -o ! -x $G ];then
            echo $G non e\' una dir o non traversabile
            exit 2
        fi;;
    *)
        echo $G non e\' una dir assoluta
        exit 3
        ;;
esac

for dir in $*;do
    case $dir in
        */*) echo $dir non e\' un nome relativo semplice
        exit 2;;    
        *);;
    esac
done


PATH=$(pwd):$PATH
export PATH


sh FCR.sh $G $*


