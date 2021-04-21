#!/usr/bin/sh

if [ ! $# -eq 4 ]; then
    echo richiesti 4 parametri, non $#
    exit 1
fi

case $1 in
    /*);;
    *)  echo $1 deve essere dir assoluta
        exit 2
esac

if [ ! -d $1 -o ! -x $1 ]; then
    echo $1 deve essere una dir con permesso di lettura
    exit 3
fi

# CONTROLLO PARAMETRI NUMERICI

# PRIMO MODO

expr $2 + 0 > /dev/null 2>&1
if [ ! $? -eq 2 -a ! $? -eq 3 ]; then   # VERIFICO che il ritorno di expr non sia 2 o 3(errore di expr)
    echo $2 e\' numerico
    if [ $2 -le 0 ]; then
        echo $2 non e\' positivo
        exit 4
    fi
else
    echo $2 non numerico
    exit 5
fi

# SECONDO MODO su $3

case $3 in
*[!0-9]*) echo $3 non numerico o non positivo
            exit 6;;
*)  if [ -eq
