#!/usr/bin/sh

# il fatto che si debba passare al file C indica che è necessario un file temporaneo

if [ ! $# -eq 2 ]; then
    echo sono richiesti 2 parametri, non $#
    exit 1
fi

case $1 in
/*);;
*)  echo errore, \'$1\' deve essere dir assoluta
    exit 2
;;
esac

case $2 in
*/*) echo errore, \'$2\' deve essere dir assoluta
    exit 3
;;
esac

if [ ! -d $1 -o ! -x $1 ]; then
    echo errore su \'$1\'
    exit 4
fi

# siccome andranno cercati nel ricorsivo non si deve controllare qui se è una directory!

#if [ ! -d $2 -o ! -x $2 ]; then
#    echo errore su \'$2\'
#    exit 4
#fi

PATH=$(pwd):$PATH
export PATH

# creo un file temporaneo, dove $$ si espande al PID, quindi sarà sempre diverso
> /tmp/tmp$$

# passo il file tmp come terzo parametro, con $* passo tutti i parametri di FCP.sh
FCR.sh $* /tmp/tmp$$

# qui stampiamo il numero di linee, ovvero il numero di file trovati (uno per riga)
echo Abbiamo trovato $(wc -l) < /tmp/tmp$$ file che soddisfano la specifica

echo I file sono: $(cat /tmp/tmp$$)

echo Richiamo la parte C passando come parametri i file trovati # adesso è commentata, non essendoci
#main $(cat /tmp/tmp$$)

#cancello il file tmp

rm /tmp/tmp$$
