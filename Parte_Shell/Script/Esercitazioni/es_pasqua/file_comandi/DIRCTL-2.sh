#!/usr/bin/sh

max_allowed=1

if [ $# -eq 0 ]; then
	echo "no param"
	more < $(ls -l *) 2> /dev/null	#?
fi

if [ ! $# -eq $max_allowed ]; then
	echo wrong number of parameters: $max_allowed is the maximum
	exit 1
fi

if [ -f $1 ]; then
echo "$1 e' un file"
echo "chiamo $1"
sh DIR.sh $1

elif [ -d $1 -a -x $1 ]; then
echo "$1 e' una directory attraversabile"
echo "entro in $1"
cd DIR
fi
