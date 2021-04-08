#!/usr/bin/sh

H=$1
K=$2
#G per le singole gerarchie

#cont file che soddisfano la specifica

if [ ! $# -ge 3 ]; then
    echo $# non sufficiente, richieste Q+2
    exit 1
fi

expr $H + 0 > /dev/null 2>&1
if [ ! $? -eq 2 -a ! $? -eq 3 ]; then
    echo $H e\' numerico
    if [ ! $H -ge 0 ]; then
        echo $H non e\' strettamente positivo
        exit 2
    fi
else
    echo $H non numerico
    exit 3
fi

expr $K + 0 > /dev/null 2>&1
if [ ! $? -eq 2 -a ! $? -eq 3 ]; then
    echo $K e\' numerico
    if [ ! $K -ge 0 ]; then
        echo $K non e\' strettamente positivo
        exit 4
    fi
else
    echo $K non numerico
    exit 5
fi

shift
shift
PATH=$(pwd):$PATH
export PATH
tempfile=/tmp/tmp$$
> $tempfile
for G in $*; do
    if [ -d $G -a -x $G ];then
        case $G in
            /*);;
            *) 
                echo $G non e\' una directory assoluta
                exit 6
        esac
        FCR.sh $G $H $K $tempfile
        if [ $(wc -l < $tempfile) -eq 0 ]; then
            echo $G non compatibile
        else
            echo sono stati trovati $(wc -l < $tempfile) file compatibili in $G
            #cat $tempfile
        fi
    fi

done
rm /tmp/tmp$$
