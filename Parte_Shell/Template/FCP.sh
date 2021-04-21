#!/bin/sh
#imposto lo shabang compatibile 

#assegno le nomenglature indicate nella traccia rispetto i parametri inseriti

if [ ! $# ];then

    exit 1
fi




expr $1 + 0 > /dev/null 2>&1
if [ ! $1 -eq 2 -a ! $? -eq 3 ]; then
    echo $1 e\' numerico
    if [ ! $1 -ge 0 ]; then
        echo $Y non e\' strettamente positivo
        exit 2
    fi
else
    echo $1 non numerico
    exit 3
fi





PATH=$(pwd):$PATH
export PATH


FCR.sh $ $


#rm $tempfile
