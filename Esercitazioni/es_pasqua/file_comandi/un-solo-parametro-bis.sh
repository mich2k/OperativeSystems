#!/usr/bin/bash


echo "$1 e' un percorso"
case $1 in
/*) echo "assoluto"		# unary op. expected, right order in test statement
	if [ -f $1 ]; then
		 echo "$1 e' un file"
	elif [ -d $1 -a -x $1 ]; then
		echo "$1 directory attraversabile"
	fi
;;
*/*) echo "relativo";;
*) echo "relativo semplice";;

# se si vuole obblgiatoriamente un rel. semplice
# */*) echo non rel semplice; exit 1

esac