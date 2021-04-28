#!/usr/bin/sh

if [ ! $# -eq 1 ]; then
    echo un comando, non $#
    exit 1
fi

case $1 in
/*)
    if [! -d $1 -o ! -x $1 ]; then
    echo "not existing dir (check permissions)"
    exit 2
    fi
;;
*)  echo "dir non assoluta ($1)"
    exit 3
;;
esac

sh file-dir.sh $1
