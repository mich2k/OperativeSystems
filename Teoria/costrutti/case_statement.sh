#! /usr/bin/sh

echo "Fornire risposta (Si,si,yes,Yes)"
read risp

case $risp in
S* | s* | Y* | y* echo OK;;
*)	ECHO NO;;		# SINTASSI PER IL DEFAULT!
esac