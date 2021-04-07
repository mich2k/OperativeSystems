#!/usr/bin/sh

case $1 in
/*);;
*)  echo $1 non e\' una dir assoluta
    exit 1;;
esac

expr $2 + 0 > /dev/null 2>&1
if [ ! $? -eq 2 -a ! $? -eq 3 ]; then   # vedo se il ritorno e\' 2 o 3
    echo $2 e\' numerico
    if [ $2 -ge 2 ]; then
        echo $2 non e\' maggiore uguale a 2
        exit 2
    fi
else
    echo $2 non numerico
    exit 3
fi

expr $3 + 0 > /dev/null 2>&1
if [ ! $? -eq 2 -a ! $? -eq 3 ]; then   # vedo se il ritorno e\' 2 o 3
    echo $3 e\' numerico
    if [ $3 -gt 0 ]; then   #greater than
        echo $3 non e\' strettamente positivo
        exit 4
    fi
else
    echo $3 non numerico
    exit 5
fi

if [ ! -d $1 -o ! -x $1 ]; then
    echo $1 non esiste o non attraversabile
    exit 6
fi

#> /tmp/tmp$$
PATH=$(pwd):$PATH
export PATH
FCR.sh $* #/tmp/tmp$$

#rm /tmp/tmp$
