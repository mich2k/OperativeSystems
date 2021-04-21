#!/bin/sh
#!/bin/bash

# CONTROLLO VALIDITA' NUMERO

expr $1 + 0 > /dev/null 2>&1    # mettere il $val giusto
if [ ! $? -eq 2 -a ! $? -eq 3 ];then
    echo $1 e\' numerico
    if [ ! $1 -ge 0 ]; then # in questo caso controlla che sia > 0
        echo $1 non e\' strettamente positivo
        exit 1  # attenzione
    fi
else
    echo $1 non numerico
    exit 2  # attenzione
fi

case $a in
    *)echo a;;
esac

# PARTE TERMINALE FCP.sh

PATH=$(pwd):$PATH
export PATH

tempfile=/tmp/tmp$$
>$tempfile
# da qui inizio la parte sulla/e gerarchie/fasi




# TEMPLATE SCHELETRO FILE RICORSIVO
