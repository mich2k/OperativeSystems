#! /usr/bin/sh
if test $# -ne 2
then 
	echo $# non è il numero corretto di parametri
	exit 1	#esco con un codice di errore custom
fi
if test -f $2
then
		grep $1 $2 > /dev/null 2>&1	#se il comando grep ha successo, e trova la stringa passata per parametro
								# in più vado a castare sia stdout che stderr in /dev/null
		if test $? -eq 0
			then echo Trovata stringa $1 nel file: $2
			else echo Non Trovata $1 in $2
		fi
else
echo il file $2 non esiste
fi