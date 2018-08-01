#!/bin/bash

if [ $# -ne 2 ]
then
	echo "Usage: $0 <SW_R file> <SW_W file>"
	exit 1
fi

gnuplot -persist <<-EOFMarker
	set terminal png
	set output "result_sw_re.png"
	set key outside
	plot '$2' axis x1y1, '$1' axis x1y2
EOFMarker

