#!/usr/bin/sh

cd $1
# la variabile trovata viene iniz. false e viene posta true ogni volta trovato un file
# con le specifiche richieste
trovato=false


# verifico se la dir corrente fa match con $2
case $1 in
*/$2)   # nome relativo già trovato ($2)
    for i in *; do
        if [ -f $ i ]; then # ci interessa solo che sia un file, non leggibile
            if grep [0-9] $i > /dev/null 2>&1; then
            echo $(pwd)/$i >> $3 # se abbiamot trovato una dir il cui nome rispetta la specifica e 
                                # anche il file la rispetta, allora metto in append nel parametro $3
                                # ovvero il file temporaneo tmp$$
            trovato=true
            fi
        fi
    done;;
esac

if [ $trovato = true ]; then  #ATTENZIONE: mettere spazio per string comparison
    echo TROVATA DIR: $(pwd)
fi

# ricorsione in tutta la gerarchia
for i in *; do
    if [ -d $i -a -x $i ]; then # se tra le foglie (composte anche da file) vi è una dir, attraversabile allora la esploriamo anch essa ricorsivamente
        $0 $(pwd)/$i $2 $3      # richiamo il file stesso sempre passando tre parametri come da principio
    fi
done
    
