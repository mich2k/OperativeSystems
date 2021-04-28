#!/bin/sh
#file comandi ricorsivo che torna il numero totale di livelli 
#contati fino a quel punto

#echo siamo nella fase $3
cd $1
livello=$3
conta=$(expr $2 + 1)
#il primo livello verra' contato come livello 1
livello_max=$conta
for i in *; do
        if [ -d $i -a -x $i ]; then
#		echo stiamo per andare in `pwd`/$i
                FCR.sh $(pwd)/$i $conta $livello
                ret=$?
		if [ $ret -gt $livello_max ];then
                        livello_max=$ret
        fi
        fi
done

# qui posso fare considerazioni nel caso volessi rivolgermi al fare un operazione in un livello specifico in una fase secondaria del codice

#echo in `pwd` conta = $conta
#if test $3 = B;then
    #echo siamo passati alla fase $3
#    if test $4 -eq $conta; then
#     echo  Adesso visualizzo il contenuto del direttorio $(pwd) che fa parte del livello $4
#        ls -l #se dovessimo visualizzare tutte le info inclusi i file nascosti sarebbe ls -la oppure inclusi i file nascosci a parte . e .. sarebbe ls -lA
#    fi 
#fi 

exit $livello_max
