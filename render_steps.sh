#!/bin/bash

for f in data-*.pgm
do
	sed s/data.pgm/$f/ < render.pov > do_render.pov
	OUT=`echo "$f" | sed s/data/render/ | sed s/pgm/png/`
	povray -D +O$OUT do_render.pov
done

rm do_render.pov
