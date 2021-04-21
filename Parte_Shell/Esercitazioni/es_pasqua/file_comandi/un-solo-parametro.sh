#!/usr/bin/sh


echo "$1 e' un percorso"
case $1 in
/*) echo "assoluto";;
*/*) echo "relativo";;
*) echo "relativo semplice";;

# se si vuole obblgiatoriamente un rel. semplice
# */*) echo non rel semplice; exit 1

esac