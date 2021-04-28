#!/usr/bin/sh

#primo par: dir assoluta
#secondo par: K linee

if [ ! $# -eq 2 ];then
    echo numero parametri non corretto, richiesti 2 non $#
    exit 1
fi

case $1 in
    /*);;
    *)
        echo richiesta dir assoluta, $1 non lo e\'
        exit 2
esac


expr $2 + 0 > /dev/null 2>&1
if [ ! $? -eq 2 -a ! $? -eq 3 ]; then
    #echo $2 e\' numerico
    if [ ! $2 -ge 0 ]; then
        echo $2 non e\' strettamente positivo
        exit 3
    fi
else
    echo $2 non numerico
    exit 4
fi


PATH=$(pwd):$PATH
export PATH
tempfile=/tmp/tmp$$
> $tempfile

if [ -d $1 -a -x $1 ];then
    FCR.sh $1 $2 $tempfile
    
    for line in $(cat $tempfile);do
        echo inserire valore input per $line
        read inp
        echo chiamo la parte in C per $line
        main.c $inp $line
    done
    
    
else
    echo $1 non dir o non attraversabile
    exit 5
fi

rm $tempfile
