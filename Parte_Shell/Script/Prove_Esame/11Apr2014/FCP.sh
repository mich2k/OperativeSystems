#!/usr/bin/sh

if [ ! $# -eq 2 ];then
    echo richiesti N+2 parametri con N ge 2
    exit 1
else
    echo procedo con $# parametri
fi

for ger in $*;do
    case $ger in
        /*)
            if [ ! -d $ger -o ! -x $ger ];then
                echo $ger non e\' una dir o non si hanno i permessi adatti
                exit 2
            fi;;
        *)
            echo $ger non e\' una dir assoluta
            exit 3;;
        esac
done

cont_temp=0
PATH=$(pwd):$PATH
export PATH

for ger in $*;do
    subtemp=/tmp/tmp$$_$cont_temp
    > $subtemp 
    echo $subtemp
    FCR.sh $ger $subtemp
    cont_temp=$(expr $cont_temp + 1)
done

cont_temp=0

for ger in $*; do
    subtemp=/tmp/tmp$$_$cont_temp
    cat $subtemp
    echo in $ger sono stati trovati $(wc -l < $subtemp) files compatibili
    for curr in $(cat $subtemp);do
        echo inserire X per $curr
        read X
        head -$X < $curr
    done
    rm $subtemp
    cont_temp=$(expr $cont_temp + 1)
done
