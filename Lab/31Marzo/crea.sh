#! /usr/bin/sh

# se dopo il for non andiamo a scrivere nulla
# è come se ci fosse $*
# eseguendo ./crea.sh uno due tre
# verranno quindi creati
for i	# qui non va messo in
do
> $i
done	# ritorna status 0
