#!/bin/sh
#imposto lo shabang compatibile 

if [ ! $# -ge 4 ];then  # controllo il numero di parametri inseriti e verifico siano greater equal di 4
    echo errore: sono necessari un numero di parametri maggiore o uguale a 4, non $#
    exit 1
fi

# assegno tutte le nomenglature definite nella traccia ai parametri
# e salvo nella variabile word il secondo parametro
J=$1
word=$2

expr $J + 0 > /dev/null 2>&1 
if [ ! $? -eq 2 -a ! $? -eq 3 ]; then   # in caso l expr ritorni ($?) due o tre significa si trovi in uno stato di errore, non proseguo, stampo l errore ed esco, altrimenti proseguo
    #echo $K e\' numerico   echo di debug
    if [ ! $J -gt 0 ]; then # se non è strettamente positivo (greater than) di zero, riporto l errore ed esco
        echo $J non e\' strettamente positivo
        exit 2
    fi
else
    echo $J non e\' numerico
    exit 3
fi

case $word in   #verifico sia una stringa semplice e non un percorso
    */*)        # per essere compatibile NON dovra fare pattern matching con questa espressione
        echo \'$word\' deve essere una stringa
        exit 4;;
    *);;
esac

shift   # shifto due volte solo dopo aver assegnato a delle variabili i parametri precedenti, in questo modo posso considerare $* unicamente formato dalle restanti directory assolute che ora potrò verificare, avendole salvate non perderò i contenuti di $1 e $2
shift


# faccio il controllo di validità delle gerarchie inserite PRIMA del ciclo relativo al file ricorsivo

for Ger in $*;do    # ciclo su $* per considerare e controllare tutte le directory inserite
    case $Ger in
        /*) # in caso sia compatibile con la path assoluta verifico anche sia una directory valida
            if [ ! -d $Ger -o ! -x $Ger ];then
                echo $Ger non e\' una directory valida o non si hanno i permessi adatti
                exit 5
            fi
            ;;
        *)
            echo $Ger non e\' una directory di tipo assoluto
            exit 6
            ;;
    esac
done

tempfile=/tmp/tempfile$$ #assegno ad una variabile il percorso e nome del file temporaneo, così poi da poterlo creare, in più aggiungo $$ che identifica il PID (Process IDentifier), per evitare di modificare file temporanei di sistema
>$tempfile  # vado quindi a creare il file tempfile, in questo stadio sarà ovviamente vuoto

PATH=$(pwd):$PATH   # modifico ed esporto la variabile di ambiente PATH, così che durante la ricorsione il programma non vada in errore perchè non trova più l eseguibile
export PATH

for Ger in $*; do  # organizzo le Y fasi, nonchè le Y chiamate al file ricorsivo, una per gerarchia
    
    FCR.sh $Ger $word $tempfile #chiamo il file ricorsivo, $ger varierà ad ogni ciclo, scorrendo $*

done

K=  # "inizializzo" la variabile K, questa sarà l input successivo dall utente

compatible_rows=$(wc -l < $tempfile)

echo sono stati\\e trovati\\e \'$compatible_rows\' directory compatibili
echo confronto questo numero a \'$J\', precedentemente inserito!


if [ ! $compatible_rows -ge $J ];then # in caso le directory compatibili trovate NON siano maggiori o uguali a J allora il codice termina, e mi assicuro di rimuovere il file temporaneo precedentemente creato
    echo le directory compatibili non sono maggiori o uguali a \'$J\'
    echo programma terminato, siccome il numero di righe trovate non e\' maggiore uguale a \'$J\'!
    rm $tempfile
    exit 7
fi

# in caso sia compatibile posso procedere, preferisco questo metodo così da non dover annidare tutto il resto del codice all interno dell if statement precedente

K=  # "inizializzo" la variabile K, indicata nel testo come variabile per selezionare la riga tra le directory compatibili trovate

echo Michele inserisca la variabile K, per selezionare la directory tra quelle trovate
read K

expr $K + 0 > /dev/null 2>&1 
if [ ! $? -eq 2 -a ! $? -eq 3 ]; then   # in caso l expr ritorni ($?) due o tre significa si trovi in uno stato di errore, non proseguo, stampo l errore ed esco, altrimenti proseguo
    #echo $K e\' numerico
    if [ ! $K -ge 1 -o ! $K -le $J ]; then # se non è maggiore uguale a uno e minore uguale a J esco
        echo $K non e\' maggiore uguale di \'1\' e minore uguale di \'$J\'
        rm $tempfile
        exit 8
    fi
else
    echo $K non e\' numerico
    rm $tempfile
    exit 9
fi

echo la directory corrispondente al numero inserito \'$K\' e\':
echo $(awk NR==$K $tempfile)    # utilizzo il macroprogramma awk con opzione NR (Row selector) in relazione alla variabile $K su $tempfile, alternativamente avrei potuto usare head e tail in pipeline

echo codice TERMINATO!

rm $tempfile    # a programma completato, mi assicuro di rimuovere il file temporaneo creato precedentemente
