#!/usr/bin/sh

cont=1
B=${@:$#}
echo $B

# WORKING TEST: ./FCP.sh $(pwd)/dir $(pwd)/dir 4    COME SEMPRE stampa doppio passando due volte la stessa dir

if [ ! $# -ge 3 ]; then
    echo errore richiesti almeno Q+1 parametri con Q ge 2, non $#
    exit 1
fi

for ger in $*; do
    
    if [ $cont -eq $# ]; then
        continue
    fi

    if [ ! -d $ger -o ! -x $ger ]; then
        echo "$ger non e\' una dir valida (controllare i permessi)"
        exit 2
    fi

    case $ger in
        /*);;
        *) echo $ger non e\' una directory assoluta
        exit 3;;
    esac
    cont=$(expr $cont + 1)
    
done


expr $B + 0 > /dev/null 2>&1
if [ ! $? -eq 2 -a ! $? -eq 3 ]; then
    echo $B e\' numerico
    if [ ! $B -ge 0 ]; then
        echo $B non e\' strettamente positivo
        exit 4
    fi
else
    echo $B non numerico
    exit 5
fi

cont=1
PATH=$(pwd):$PATH
export PATH


for Q in $*; do
    if [ $cont -eq $# ]; then
        continue
    fi
    echo inizio con $Q
    sh FCR.sh $Q $B
    cont=$(expr $cont + 1)
done
