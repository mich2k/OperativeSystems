#!/usr/bin/sh
#21:36

if [ $# -lt 3 ];then    # controllo il numero dei parametri, $# rappresenta quanti parametri l utente ha inserito
    echo richiesti N+1 parmetri, non $#
    exit 1      # se non conformi, esco
else
    echo procedo con $# parametri
fi

F=$1 # assegno al primo parametro la denominazione indicata dalla traccia, anche perchè dovremo shiftare i parametri se il controllo andrà a buon fine

case $F in
    */*)
        echo errore: $F deve essere relativo semplice
        exit 2;;
    *);;    # in questo caso non ci sarà nessun controllo sull esistenza o leggibilità, siccome il nome verrà usato nel ricorsivo, dove verranno implementati questi controlli
esac

shift   # rimuovo $1 in $*, così da avere solo gerarchie in $* e poterle usare nel for

# ora effettuo il controllo sulle gerarchie inserite, che esistano, siano attraversabili (-x) e che siano assolute

for ger in $*; do
    case $ger in
        /*) # in caso passi il controllo di essere assoluta verifico che si dispongano i permessi adatti e che sia effettivamente una gerarchia
            if [ ! -d $ger -o ! -x $ger ];then
                echo $ger non è una directory o non si hanno i permessi adatti
                exit 3
            fi;;
        *)
            echo $ger non è una directory di tipo assoluto
            exit 4;;
    
    esac
done


PATH=$(pwd):$PATH
export PATH

tempfile=/tmp/tmp$$    #assegno e creo un file tmp identificato anche dal PID ($$), opero in questo modo per salvare i dati dal ricorsivo e far comunicare i due file FCP e FCR
>$tempfile

for ger in $*;do # grazie allo shift scrivo for ger in $*
    
    FCR.sh $ger $F $tempfile
    
done

echo sono stati trovati $(wc -l < $tempfile) file compatibili

for row in $(cat $tempfile); do
    file_path=$(echo $row | cut -d ':' -f 1)
    lines=$(echo $row | cut -d ':' -f 2)
    echo Trovato \'$file_path\' con $lines linee.
    echo -n Inserire il valore X per questo file:
    read X
    echo $X
    expr $X + 0 > /dev/null 2>&1
    if [ ! $? -eq 2 -a ! $? -eq 3 ];then
        #echo $1 e\' numerico
        if [ $X -lt 1 -o $X -gt $lines ];then
            echo $X non e\' compatibile: deve essere compreso tra 1 e $lines
            rm $tempfile
            exit 5
        fi
    else
        echo $X non numerico
        rm $tempfile
        exit 6
    fi
    echo riporto le prime $X linee di \'$(basename $file_path)\'
    head -$X $file_path
done

echo FINE ALGORITMO

rm $tempfile
