#! /bin/bash
# This script plots the points of an A* path using gnuplot.

if [ -f "../test/astar_path.dat" ]
	then
		gnuplot -e "load \"load_astar_path.p\""
		gimp astar_path.ps
else
	echo "ERROR: An A* path could not be found to plot."
	exit 1
fi
