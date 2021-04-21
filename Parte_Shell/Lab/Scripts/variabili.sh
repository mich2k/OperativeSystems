#! /usr/bin/sh

	#Il valore della variabile è SEMPRE trattato come stringa

a=10	#NON vanno inseriti spazi adiacenti al simbolo =
echo $a	#In questo modo, con '$' considero il valore della variabile
b=$a	#Assegno a b il valore di a
echo $b