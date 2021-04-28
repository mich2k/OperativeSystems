#!/usr/bin/sh

case $# in
1)  
    echo modalità con un parametro \'$1\'
    cat >> $1
;;
2)
    echo metto in append \'$2\' a \'$1\'
    cat $2 >>  $1
;;
*)
    echo 1 or 2 parameters expected, exiting..
    exit 1
;;
esac
