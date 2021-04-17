#!/usr/bin/sh

#script inutile che stampa i file FCR.sh minori di $2 linee
# usage: ./lines.sh {dir} {max lines}

# SI l ho scritto perchè mi serviva un esempio e mi scocciavo di cercarlo a mano

PATH=$(pwd):$PATH
export PATH
sh script.sh $1 $2
