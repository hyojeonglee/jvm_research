#!/bin/bash

if [ $# -ne 4 ]
then
	echo "Usage: $0 <RD file> <WR file> <SW_R file> <SW_W file>"
	exit 1
fi

gnuplot -persist <<-EOFMarker
	set terminal png
	set output "result.png"
	set key outside
	plot '$1' axis x1y1, '$2' axis x1y2, '$3' axis x2y1, '$4' axis x2y2
EOFMarker

