#!/bin/bash

# This script generates 8 walls with some different options. GPLv2
# requires 'bc', 'bash', 'mklabwall'

# simple conversion hex to float
find_color() {
	echo $(printf "%d" "0x${1}") / 255 | bc -l
}

export -f find_color

[ -d "$HOME/dem" ] || mkdir -p $HOME/dem
F=nunito
res="1280 720"
L=''
x=0
for i in '#1E333A' '#1E90B5' '#1EB544' '#9BB51E' '#B5651E' '#B51E2A' '#B51E82' '#3A1EB5'; do
	BR=${i:1:2}; BG=${i:3:2}; BB=${i:5:2};
	BHR=$(find_color $BR)
	BHG=$(find_color $BG)
	BHB=$(find_color $BB)
	[ $x -gt 3 ] && L=-l
	mklabwall -f $F -r "$res" -b "${BHR:0:4} ${BHG:0:4} ${BHB:0:4}" $L -n dem/lwall${x}
	x=$(($x + 1))
done
