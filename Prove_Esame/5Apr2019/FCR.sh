#!/usr/bin/sh

F=$2
tempfile=$3

cd $1   # mi sposto nel parametro $1, questo rappresenta la dir, e cambierà man mano che si esplora la gerarchia/gerarchie


for curr_file in *; do
    if [ -f $curr_file -a -r $curr_file ];then  # verifico che sia un file E che sia leggibile
        case $curr_file in
            $F)                         # con il case verifico che il nome relativo semplice corrisponda e faccia quindi match
                lines=$(wc -l < $curr_file)
                if [ $lines -ge 4 ];then
                    echo trovato file compatibile $curr_file in $(pwd)
                    echo $(pwd)/$curr_file:$lines >> $tempfile  # METTO IN APPEND
                
                fi
                ;;
            *)
                ;;
        esac
    fi
done



for dir in *; do
    if [ -d $dir -a -x $dir ];then  # verifico se l elemento considerato dal for è una dir ed è attraversabile
        $0 $(pwd)/$dir $F $tempfile           # chiamo la ricorsione
    fi
done
