#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Usage: $0 <target file>"
	exit 1
fi

gnuplot -persist <<-EOFMarker
	set terminal png
	set output "result_WR_single.png"
	set key outside
	set yrange[410000000:500000000]
	set xrange[0:24]
	plot '$1' axis x1y1
EOFMarker

