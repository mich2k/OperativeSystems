#!/usr/bin/sh

# WORKING TEST: ./FCP.sh $(pwd)/dir a b c

G=$1

if [ ! $# -ge 3 ]; then
    echo "Numero parametri ($#) inseriti non validi, richiesti Q+1 con Q ge 2"
    exit 1
fi

case $G in
    /*)
        if [ ! -d $G -o ! -x $G ];then
            echo "$G non dir (check perm)"
            exit 2
        fi
        ;;
    *) echo $G deve essere una dir assoluta
        exit 3;;
esac

shift

#controllo sia UN carattere

for char in $*; do
    case $char in
    ?);;
    *) 
        echo $char non e un carattere
        exit 4;;
    esac
done

PATH=$(pwd):$PATH
export PATH


sh FCR.sh $G $*

