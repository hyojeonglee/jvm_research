#!/bin/bash

if [ $# -ne 4 ]
then
	echo "Usage: $0 <RD file> <WR file> <SW_R file> <SW_W file>"
	exit 1
fi

gnuplot -persist <<-EOFMarker
	set terminal png
	set output "result_RD.png"
	set key outside
	set xrange[0:25]
	set yrange[430000000:500000000]
	plot '$1' axis x1y1
EOFMarker


gnuplot -persist <<-EOFMarker
	set terminal png
	set output "result_WR.png"
	set key outside
	set yrange[430000000:500000000]
	set xrange[0:25]
	plot '$2' axis x1y1
EOFMarker



gnuplot -persist <<-EOFMarker
	set terminal png
	set output "result_SW_RD.png"
	set key outside
	set yrange[430000000:500000000]
	set xrange[0:25]
	plot '$3' axis x1y1
EOFMarker


gnuplot -persist <<-EOFMarker
	set terminal png
	set output "result_SW_WR.png"
	set key outside
	set yrange[430000000:500000000]
	set xrange[0:25]
	plot '$4' axis x1y1
EOFMarker

