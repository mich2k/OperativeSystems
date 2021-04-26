#!/usr/bin/sh

if [ ! $# -eq 1 ]; then
    echo "$# unlegit (max 1 parametro)"
fi

case $@ in
*/*) echo e\' necessario un nome relativo semplice.
    exit 1;;
esac

# for "a lezione" originario, credo

c=0
for i in `cat $1`
    do
    c=$(( c + 1 ))
    if [ $(( c % 2 )) -eq 1 ]
        then
            echo uneven, filename: $i
                if [ -f $i ]
                    then cat $i
                fi
        else 
            echo ""
            echo even, lenght: = $i
    fi
done
