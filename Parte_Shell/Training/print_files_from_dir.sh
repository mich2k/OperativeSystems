#! /usr/bin/sh

# GOAL: stampo tutti i file regolari (non directory) nella working path corrente e subdirs

#todo: controllo input


if [ ! -d $1 -o ! -x $1 ];then
    echo "richiesta dir esistente ed attraversabile ($1 non soddisfa i requisiti)"
	exit 1
fi

PATH=$PATH:$(pwd)
export PATH

print_files_recursive.sh $1
