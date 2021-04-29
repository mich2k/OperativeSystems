#!/bin/sh

case $# in
1)	case $1 in
	/*) if test ! -d $1 -o ! -x $1
	    then
	    echo $1 non direttorio
	    exit 1
	    fi;;
	*)  echo $1 non nome assoluto; exit 2;;
	esac;;
*) 	echo Errore: Usage is $0 dirass 
	exit 3;;
esac


PATH=$(pwd):$PATH
export PATH

conta=0
#fase=A in questo file non implemento le fasi
livello=0

#invochiamo per la fase A: il quarto parametro ($livello) NON viene usato nella prima fase, ma lo passiamo lo stesso per uniformita' 
FCR.sh $1 $conta $livello   

tot=$?
echo Il numero di livelli totali della gerarchia = $tot
#passiamo alla seconda fase



: <<'END_COMMENT'
echo Stiamo iniziando la fase B
adesso chiediamo all'utente quale livello vuole visualizzare
echo "dammi il numero di livello pari che vuoi visualizzare (il numero deve essere compreso fra 1 e $tot" > /dev/tty
read livello 
echo $livello
case $livello in
*[!0-9]*) echo NON hai fornito un numero o non positivo
          exit 4;;
#*) #numero giusto;;
esac
if test $livello -ge 1 -a $livello -le $tot
then
echo $livello compreso OK
if test `expr $livello % 2` -eq 0
then
	echo $livello pari
else
	echo $livello dispari #questo sarebbe un errore per il testo considerato, ma lo teniamo buono per fare un numero maggiore di prove
fi
else
	echo $livello NON compreso
exit 5
fi
fase=B
#passiamo alla seconda chiamata 
FCR.sh $1 $conta $fase $livello  
echo FINITO
END_COMMENT

