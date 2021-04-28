#!/usr/bin/sh

#   $1 = dir relativa
#   $2 = numero strett. positivo

dir_to_find=$1
lines_length_requested=$2

# COMANDO PER TEST: ./FCP.sh dir 3 $(pwd)/radice

if [ $# -ge 4 ]; then # necessario NOT DSDWCW
    echo "$# non ammesso: almeno W+2 (4) parametri richiesti"
    exit 1
fi

case $1 in
*/*) 
    echo errore, $1 deve essere relativo semplice
    exit 2;;
*);;
esac

# ERRORE!
#if [ ! -d $1 -o ! -x $1 ]; then
#    echo $1 non e\' una directory o non si hanno i permessi richiesti
#    exit 3
#fi

expr $2 + 0 > /dev/null 2>&1
if [ ! $? -eq 2 -a ! $? -eq 3 ]; then   # vedo se il ritorno e\' 2 o 3
    echo $2 e\' numerico
    if [ ! $2 -ge 0 ]; then
        echo $2 non e\' maggiore uguale a 2
        exit 4
    fi
else
    echo $2 non numerico
    exit 5
fi

shift
shift
PATH=$(pwd):$PATH
export PATH

# OGNI AZIONE che è un sum-up o comunque considera valori di ritorno globali richiede un file tmp, siccome in bash non esistono valori di ritorno come contatori o simili
# 2 METODI ALTERNATIVI:
#   -   fare echo di ogni file con la sua path IN APPEND e fare $(wc -l) in questo file controllo
#   -   avere un counter nel ricorsivo, che scriva ogni volta il valore IN OVERWRITE sul file 
#   ACHTUNG: il secondo metodo non può funzionare, SICCOME la variabile count non è globale, ma è valida solo in una gerarchia

# creo il file tmp
tempfile=/tmp/tmp$$
> $tempfile

for gerarchia in $*; do
    if [ ! -d $gerarchia -o ! -x $gerarchia ]; then
        echo $gerarchia non e\' una directory o non si hanno i permessi richiesti
    fi
    FCR.sh $gerarchia $dir_to_find $lines_length_requested $tempfile
done

echo ci sono $(wc -l < $tempfile) file compatibili
echo fine algoritmo.
rm $tempfile

