#!/bin/bash

if [ $# -ne 3 ]
then
	echo "Usage: $0 <RD file> <WR file> <SW file>"
	exit 1
fi

RD=$1
WR=$2
SW=$3

gnuplot -persist <<-EOFMarker
	set terminal svg
	set output "result.svg"
	plot '$1' axis x1y1, '$2' axis x1y2, '$3' axis x2y1
EOFMarker

