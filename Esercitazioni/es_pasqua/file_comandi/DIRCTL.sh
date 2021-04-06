#!/usr/bin/sh

max_allowed=1

if [ ! $# -eq $max_allowed ]; then
	echo wrong number of parameters: $max_allowed is the maximum
	exit 1
fi
echo "chiamo DIR.sh"

#$(pwd)/DIR.sh $1
#./DIR.sh $1
sh DIR.sh $1
