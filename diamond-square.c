/*  Quick and dirty implementation of the diamond-square algorithm.
 *  Copyright (C) 2017-2018 - Jérôme Kirman
 *
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Hoping an ASCII art can be worth a thousand ASCII words.
 *
 * > Zoom step :
 *
 *  A   B   C   .   .   >   A   .   B   .   C
 *                      >                    
 *  D   E   F   .   .   >   .   .   .   .   .
 *                      >                    
 *  G   H   I   .   .   >   D   .   E   .   F
 *                      >                    
 *  .   .   .   .   .   >   .   .   .   .   .
 *                      >                    
 *  .   .   .   .   .   >   G   .   H   .   I
 *
 * Write : (2x,2y) <- (x,y)
 *
 *
 * > Diamond step :
 *
 *  S   .   S   .   S     ( 2x , 2y )   (2x+2, 2y )
 *
 *  .   +   .   +   .             \       /
 *
 *  S   .   S   .   S            (2x+1,2y+1)
 *
 *  .   +   .   +   .             /       \
 *
 *  S   .   S   .   S     ( 2x ,2y+2)   (2x+1,2y+2)
 *
 *
 * > Square step (compute value iff x + y is odd) :
 *
 *  S   +   S   +   S     (y == 0) ? 0 : ( x ,y-1)
 *
 *  +   D   +   D   +     (x == 0) ? 0 : (x-1, y )
 *
 *  S   +   S   +   S     (x == W) ? 0 : (x+1, y )
 *
 *  +   D   +   D   +     (y == H) ? 0 : ( x ,y+1)
 *
 *  S   +   S   +   S     ^ Don't step off the edge.
 *
 * Ternaries are worth a circumfence.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <fcntl.h>

// If SKIP_STEPS is defined, only the final result is written out.
// #define SKIP_STEPS

#define MAX_ITER 10

typedef float height;
#define AMP_STEP_FACTOR 0.5
#define INIT_AMP 2048.0

height random_height_delta (height amplitude)
{
	return ((float) rand() / RAND_MAX - 0.5) * amplitude;
}

#define Ind(x,y) ( (x)*side + (y) )

// Export functions :
// -> stdout
void display_hmap (height* hmap, int side)
{
	for (int x = 0 ; x < side ; x++)
	{
		for (int y = 0 ; y < side ; y++)
			printf ("%i\t", (int) hmap[Ind(x,y)]);
		printf ("\n");
	}
	printf ("\n");
}

// -> PGM file
void export (char* filename, height* hmap, int side)
{
	// Clamping check
	height h = 0.0,
	minh =  INIT_AMP,
	maxh = -INIT_AMP;
	for (int x = 0 ; x < side ; x++)
		for (int y = 0 ; y < side ; y++)
		{
			h = hmap[Ind(x,y)];
			if (h < minh) { minh = h; }
			if (h > maxh) { maxh = h; }
		}

	// Save as PGM
	FILE* of = fopen(filename, "w+");
	fprintf(of, "P2\n");
	fprintf(of, "%i %i\n", side, side);
	fprintf(of, "%i\n", (int) (maxh-minh));
	for (int x = 0 ; x < side ; x++)
	{
		for (int y = 0 ; y < side ; y++)
			fprintf(of, "%i ", (int) (hmap[Ind(x,y)] - minh));
		fprintf(of, "\n");
	}
	fclose(of);
}

int main ()
{
	srand(time(NULL));
#ifndef SKIP_STEPS
#define FILELEN 12
	char* outfile = calloc(FILELEN, sizeof(char));
#endif

	// Generate heightmap
	int side = 2;
	height* hmap = calloc(sizeof(height), side*side);
	for (int c = 0 ; c < side*side ; c++)
		hmap[c] = 0.0;

	height amplitude = INIT_AMP;

	for (int i = 0 ; i < MAX_ITER ; i++)
	{
		side = 2*side - 1;
		hmap = realloc(hmap, sizeof(height) * side*side);

		// Zoom step (proceeding from bottom-right to avoid data loss)
		for (int x = side/2 ; x >= 0  ; x--)
			for (int y = side/2 ; y >= 0 ; y--)
				hmap[Ind(2*x,2*y)] = hmap[x*(side/2+1) + y];

		// Diamond step
		for (int x = 0 ; x < side/2 ; x++)
			for (int y = 0 ; y < side/2 ; y++)
				hmap[Ind(2*x+1, 2*y+1)] =
				( hmap[Ind( 2*x , 2*y )] + hmap[Ind(2*x+2, 2*y )] +
				  hmap[Ind( 2*x ,2*y+2)] + hmap[Ind(2*x+2,2*y+2)] ) / 4
				+ random_height_delta(amplitude);

#ifndef SKIP_STEPS
		// Cleanup intermediate data for diamond step output.
		for (int x = 0 ; x < side ; x++)
			for (int y = 0 ; y < side ; y++)
				if ( (x+y) % 2 != 0 )
					hmap[Ind(x,y)] =
					( ( y ==    0   ? 0.0 : hmap[Ind( x ,y-1)] ) +
					  ( x ==    0   ? 0.0 : hmap[Ind(x-1, y )] ) +
					  ( x == side-1 ? 0.0 : hmap[Ind(x+1, y )] ) +
					  ( y == side-1 ? 0.0 : hmap[Ind( x ,y+1)] ) ) / 4;

		snprintf(outfile, FILELEN, "data-%id.pgm", i);
		export(outfile, hmap, side);
#endif

		// Square step
		for (int x = 0 ; x < side ; x++)
			for (int y = 0 ; y < side ; y++)
				if ( (x+y) % 2 != 0 ) // Surface oddity
					hmap[Ind(x,y)] =
					( ( y ==    0   ? 0.0 : hmap[Ind( x ,y-1)] ) +
					  ( x ==    0   ? 0.0 : hmap[Ind(x-1, y )] ) +
					  ( x == side-1 ? 0.0 : hmap[Ind(x+1, y )] ) +
					  ( y == side-1 ? 0.0 : hmap[Ind( x ,y+1)] ) ) / 4
					+ random_height_delta(amplitude);

#ifndef SKIP_STEPS
		snprintf(outfile, FILELEN, "data-%is.pgm", i);
		export(outfile, hmap, side);
#endif

		amplitude *= AMP_STEP_FACTOR;
	}

	export ("data-final.pgm", hmap, side);

#ifndef SKIP_STEPS
	free(outfile);
#endif

	return EXIT_SUCCESS;
}
