#!/usr/bin/sh

cd $1
cont=1
files=
compatible=true

for curr_file in *;do
    if [ -f $curr_file -a -r $curr_file ];then
        n_chars=$(wc -c < $curr_file)
        
        for char in $*; do
            if [ $cont -eq 1 ];then
                #echo $char ignorato! con $cont
                cont=$(expr $cont + 1)
                continue
            fi
            n_compat=$(grep -c $char < $curr_file)
            if [ ! $n_compat -gt 0 ]; then
                compatible=false
                break
            fi

        done
        
        
        if [ "$compatible" = "true" ];then
            files="$files $curr_file"
            echo trovato: $(pwd)/$curr_file
            #passaggio parametri fatto segendo l ordine della traccia, il file test.sh stamperà $* per controprova
            test.sh $curr_file $(wc -l < $curr_file) ${@:2:$#}
            #main.c $curr_file $(wc -l < $curr_file) ${@:2:$#}
        fi
         
    fi

done




for i in *;do   # ricorsione
    if [ -d $i -a -x $i ];then
        $0 $(pwd)/$i $*
    fi
done
