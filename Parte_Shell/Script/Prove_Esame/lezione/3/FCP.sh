#!/usr/bin/sh

if [ ! $# -ge 3 ]; then
    echo richiesti N+1 parametri, non $#
    exit 1
fi

# per controllare tutti gli altri useremo lo shift, per verificare che siano tutte dir assolute



