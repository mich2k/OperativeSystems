#!/usr/bin/sh

#TEST: ./FCP.sh $(pwd)/dir $(pwd)/dir   stamperà doppio andando a operare due volte sulla dir

if [ ! $# -eq 3 ]; then
    echo N+1 parametri richiesti, non $#, con N greater equal 2
    exit 1
fi

expr $1 + 0 > /dev/null 2>&1
if [ ! $? -eq 2 -a ! $? -eq 3 ]; then
    #echo $1 e\' numerico
    if [ ! $1 -ge 0 ]; then
        echo $1 non e\' strettamente positivo
        exit 2
    fi
else
    echo $1 non numerico
    exit 3
fi

X=$1
shift
for dir in $*; do
if [ ! -d $dir -o ! -x $dir ]; then
    echo non si dispongono i permessi necessari oppure la directory $dir non esiste
    exit 4
    fi
done


PATH=$(pwd):$PATH
export PATH
tempfile=/tmp/tmp$$
> $tempfile



for dir in $*;do
    FCR.sh $dir $X $tempfile
done

echo $(wc -l < $tempfile) file trovati

cat $tempfile




rm $tempfile
