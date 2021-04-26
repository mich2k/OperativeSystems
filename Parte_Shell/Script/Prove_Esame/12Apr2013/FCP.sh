#!/usr/bin/sh
# TEST: ./FCP.sh 
if [ ! $# -eq 3 ];then
    echo richiesti 3 parametri, non $#
    exit 1
fi

case $1 in
    /*)
        if [ ! -d $1 -o ! -x $1 ];then
            echo "$1 non e\' una dir valida (controllare i permessi)"
            exit 2
        fi;;
    *)
        echo $1 non e\' una dir assoluta
        exit 3
        ;;
esac

expr $3 + 0 > /dev/null 2>&1
if [ ! $? -eq 2 -a ! $? -eq 3 ]; then
    echo $3 e\' numerico
    if [ ! $3 -ge 0 ]; then
        echo $3 non e\' strettamente positivo
        exit 4
    fi
else
    echo $3 non numerico
    exit 5
fi


PATH=$(pwd):$PATH
export PATH

G=$1
S=$2
N=$3
X=
tempfile=/tmp/tmp$$
>$tempfile

#$(ls -R $G | grep -c ':$')

echo INIZIO FASE A

sh FCR.sh $G $S $tempfile A

echo sono stati trovati $(wc -l < $tempfile) file

cat $tempfile

if [ $(wc -l < $tempfile) -gt $N ];then
    echo INIZIO FASE B
    echo -n Inserire X:
    read X
    if [ $X -lt 1 -o $X -gt $N ];then
        echo "X ($X) non valido"
        rm $tempfile
        exit 6;
    fi
    sh FCR.sh $(awk NR==$X $tempfile) $S $tempfile B
else
    echo valore inferiore a $N
fi


rm $tempfile
