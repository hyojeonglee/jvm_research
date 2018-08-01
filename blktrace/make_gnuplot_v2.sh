#!/bin/bash

if [ $# -ne 2 ]
then
	echo "Usage: $0 <SW_R file> <SW_W file>"
	exit 1
fi

gnuplot -persist <<-EOFMarker
	set terminal svg
	set output "result_sw.svg"
	set key outside
	plot '$1' axis x1y1, '$2' axis x1y2
EOFMarker

