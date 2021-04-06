#!/usr/bin/sh

if [ ! $# -eq 2 ]; then
    echo 2 parameters are requested, not $#
    exit 1
fi

if [ ! -w $(pwd) ]; then
    echo "error: $2 does not exist and $(pwd) is not writable"
    exit 4
fi

case $* in
*/*)echo sono richieste path relative semplici.;;
esac

if [ ! -r $1 ]; then
    echo "$1 must exist and must be readable (Permission denied)"
    exit 3
fi

