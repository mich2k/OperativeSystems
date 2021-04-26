#!/usr/bin/sh

if [ ! $# -eq 1 ]; then
	echo "1 parameter expected";	exit 1
fi

case $1 in
*/*) echo "direct path expected";	exit 2;;
*);;
esac

if [ ! -f $1 ]; then
	echo "the file must exist";		exit 3;
fi

echo "$1 respects all the parameter checks from this script"