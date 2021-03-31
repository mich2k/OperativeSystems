#! /usr/bin/sh

# vogliamo cercare un file in una sottogerarchia con un nome relativo semplice
# se specificata la dir è un percorso assoluto altrimenti è pwd

# dividiamo in due problemi

# PRIMO SOTTOPROBLEMA:

d=  #variabile per la dir corrente
f=  #variabile per il nome relativo semplice

# vado a controllare i parametri

case $# in
0)  echo "Numero parametri non sufficiente - Necessario: $0 [directory] file"
    exit 1;;    #double period comma for case statement
1)  d=`pwd`     #se viene passato un solo parametro, questo sarà il nome relativo semplice del file
    f=$1;;      #qui la gerarchia non è specificata, quindi partiamo dalla dir corrente
2)  d=$1        #qui la gerarchia di partenza è custom, ovvero $d
    f=$2;;
*)  echo "Numero parametri oltre il consentito (2) - - Necessario: $0 [directory] file"
    exit 2;;
esac

# controllo il tipo di parametri

case $d in
/*) ;;              #verifico che il nome venga dato in modo assoluto, deve quindi iniziare con '/'
*)  echo "Errore: $d non in forma assoluta"
    exit 3;;
esac

case $f in
*/*)    echo "Errore: $f non in forma relativa semplice"
        exit 4;;
*);;
esac

# controllo che il valore della variabile sia traversabile (-x)
if test ! -d $d -o ! -x $d  # -o = logic or; -a = logic and  ->  se $d non è una directory o non è attraversabile, errore
then echo "Errore: $d non traversabile"
exit 5
fi

PATH=`pwd`:$PATH    # facciamo ciò perchè generalmente andremo ad invocare a partire dalla dir corrente
                    # se ciò non è applicabile dovremmo scrivere invece di pwd tutta la dir a mano assoluta dei file esegubiili
                    # questo serve perchè il file ricorsivo deve essere in ogni caso recuperato ovunque ci troviamo nel file system
export PATH

echo "Secondo file..."
secondo.sh $d $f
echo "Fine di tutti gli script programmati"
