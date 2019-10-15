#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include "pngwriter.h"
#include "consts.h"


unsigned long get_time ()
{
    struct timeval tp;
    gettimeofday (&tp, NULL);
    return tp.tv_sec * 1000000 + tp.tv_usec;
}

int main (int argc, char** argv)
{
	png_data* pPng = png_create (IMAGE_WIDTH, IMAGE_HEIGHT);
	
	double x, y, x2, y2, cx, cy;
	cy = MIN_Y;
	
	double fDeltaX = (MAX_X - MIN_X) / (double) IMAGE_WIDTH;
	double fDeltaY = (MAX_Y - MIN_Y) / (double) IMAGE_HEIGHT;
	
	long nTotalIterationsCount = 0;
	unsigned long nTimeStart = get_time ();
	
	long i, j, n;
        
        n = 0;
	// do the calculation
	for (j = 0; j < IMAGE_HEIGHT; j++)
	{
		cx = MIN_X;
		
		for (i = 0; i < IMAGE_WIDTH; i++)
		{			
			x = cx;
			y = cy;
			
			x2 = x * x;
			y2 = y * y;
			
			// compute the orbit z, f(z), f²(z), f³(z), ...
			// count the iterations until the orbit leaves the circle |z|=2.
			// stop if the number of iterations exceeds the bound MAX_ITERS.

                        //TODO  
		        // >>>>>>>> CODE IS MISSING



                        // <<<<<<<< CODE IS NISSING
                        // n indicates if the point belongs to the mandelbrot set
			
			// plot the number of iterations at point (i, j)
			int c = ((long) n * 255) / MAX_ITERS;
			png_plot (pPng, i, j, c, c, c);
			
			cx += fDeltaX;
		}
		
		cy += fDeltaY;
	}
	
	unsigned long nTimeEnd = get_time ();
	
	// print benchmark data
	printf ("Total time:                 %g ms\n", (nTimeEnd - nTimeStart) / 1000.0);
	printf ("Image size:                 %ld x %ld = %ld Pixels\n",
		(long) IMAGE_WIDTH, (long) IMAGE_HEIGHT, (long) (IMAGE_WIDTH * IMAGE_HEIGHT));
	printf ("Total number of iterations: %ld\n", nTotalIterationsCount);
	printf ("Avg. time per pixel:        %g µs\n", (nTimeEnd - nTimeStart) / (double) (IMAGE_WIDTH * IMAGE_HEIGHT));
	printf ("Avg. time per iteration:    %g µs\n", (nTimeEnd - nTimeStart) / (double) nTotalIterationsCount);
	printf ("Iterations/second:          %g\n", nTotalIterationsCount / (double) (nTimeEnd - nTimeStart) * 1e6);
	// assume there are 8 floating point operations per iteration
	printf ("MFlop/s:                    %g\n", nTotalIterationsCount * 8.0 / (double) (nTimeEnd - nTimeStart));
	
	png_write (pPng, "mandel.png");
	return 0;
}
