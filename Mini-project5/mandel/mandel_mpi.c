#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include <mpi.h>

#include "pngwriter.h"
#include "consts.h"

#define index(x, y, lda) (y*lda + x)

unsigned long get_time ()
{
    struct timeval tp;
    gettimeofday (&tp, NULL);
    return tp.tv_sec * 1000000 + tp.tv_usec;
}

int main (int argc, char** argv)
{
	// Initialize MPI
	MPI_Init(&argc, &argv);
	int mpi_rank, mpi_size;

	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

	// Create partitioning of the image
	// determine 2D dimensions of the grid of processes
	Partition p = createPartition(mpi_rank, mpi_size);

	// Compute the local domain size and boundaries
	// determine 2D dimensions of the local portion of the image
	Domain d = createDomain(p); 

	if(mpi_rank == 0) printf("Processor grid size (%d, %d)\n", p.nx, p.ny);
	printf("[Process %d]: Coordinates [%d, %d]\n", mpi_rank, p.x, p.y);

    printf("[Process %d] Domain X: %d -> %d\n", mpi_rank, d.startx, d.endx);
    printf("[Process %d] Domain Y: %d -> %d\n", mpi_rank, d.starty, d.endy);

    /****************************************************************************/
	// create image at the MASTER only
	png_data* pPng = NULL;
	if (mpi_rank == 0)
	{
		pPng = png_create (IMAGE_WIDTH, IMAGE_HEIGHT);
	}

	// Compute the global domain parameters
	double fDeltaX = (MAX_X - MIN_X) / (double) IMAGE_WIDTH;
	double fDeltaY = (MAX_Y - MIN_Y) / (double) IMAGE_HEIGHT;

    // Allocate local image data
    int *c;
    if (mpi_rank == 0)
    {
    	//allocate extra space at master
    	int extrax = IMAGE_WIDTH % p.nx;
    	int extray = IMAGE_HEIGHT % p.ny;
    	c = malloc((d.nx + extrax) * (d.ny + extray) * sizeof(int));
    } else
    {
    	c = malloc(d.nx*d.ny*sizeof(int));
    }

    /****************************************************************************/
	// do the calculation
	double x, y, x2, y2, cx, cy;
	long nTotalIterationsCount = 0;
	long i, j;
	unsigned long nTimeStart = get_time ();
	
	cy = MIN_Y + d.starty*fDeltaY;
	for (j = 0; j < d.ny; j++) // HEIGHT
	{
		cx = MIN_X + d.startx*fDeltaX;
		for (i = 0; i < d.nx; i++) // WIDTH
		{
			x = cx;
			y = cy;
			
			x2 = x * x;
			y2 = y * y;
			
			// compute the orbit z, f(z), f²(z), f³(z), ...
			// count the iterations until the orbit leaves the circle |z|=2.
			// stop if the number of iterations exceeds the bound MAX_ITERS.
			int n = 0;
			for ( ; x2 + y2 < 4 && n < MAX_ITERS; n++, nTotalIterationsCount++)
			{
				// z = z² + c, where z = x + iy, c = cx + icy
				y = 2 * x * y + cy;
				x = x2 - y2 + cx;

				x2 = x * x;
				y2 = y * y;
			}
			// write the local pixel [i,j] -> j*d.ny + i
			c[index(i,j,d.nx)] = ((long) n * 255) / MAX_ITERS;
			cx += fDeltaX;
		}
		cy += fDeltaY;
	}
	unsigned long nTimeEnd = get_time ();

	printf ("[Process %d] Total time: %g ms\n", mpi_rank, (nTimeEnd - nTimeStart) / 1000.0);
	printf ("[Process %d] Image size: %ld x %ld = %ld Pixels\n", mpi_rank, (long) d.nx, (long) d.ny, (long) (d.nx * d.ny));
	printf ("[Process %d] Total number of iterations: %ld\n", mpi_rank, nTotalIterationsCount);
	printf ("[Process %d] Avg. time per pixel: %g µs\n", mpi_rank, (nTimeEnd - nTimeStart) / (double) (d.nx * d.ny));
	printf ("[Process %d] Avg. time per iteration: %g µs\n", mpi_rank, (nTimeEnd - nTimeStart) / (double) nTotalIterationsCount);
	printf ("[Process %d] Iterations/second: %g\n", mpi_rank, nTotalIterationsCount / (double) (nTimeEnd - nTimeStart) * 1e6);
	// assume there are 8 floating point operations per iteration
	printf ("[Process %d] MFlop/s:                    %g\n", mpi_rank, nTotalIterationsCount * 8.0 / (double) (nTimeEnd - nTimeStart));

	// Send the data to the master
	if (mpi_rank != 0)
	{
		// TODO: send local partition c to the master process
	}
	/****************************************************************************/
	// Write the image
	if (mpi_rank == 0)
	{
		// first write master's own data
		for (j = 0; j < d.ny; j++) // HEIGHT
		{
			for (i = 0; i < d.nx; i++) // WIDTH
			{
				int c_ij = c[index(i,j,d.nx)];
				png_plot (pPng, i+d.startx, j+d.starty, c_ij ,c_ij, c_ij);
			}
		}

		// receive and write the data from other processes
		for (int proc = 1; proc < mpi_size; proc++)
		{
			Partition p1 = updatePartition(p, proc);
			Domain d1 = createDomain(p1);

			// TODO: receive partition of the process proc into array c (overwrite its data)

			// write the partition of the process proc
			for (j = 0; j < d1.ny; j++) // HEIGHT
			{
				for (i = 0; i < d1.nx; i++) // WIDTH
				{
					int c_ij = c[index(i,j,d1.nx)];
					png_plot (pPng, i+d1.startx, j+d1.starty, c_ij ,c_ij, c_ij);
				}
			}
		}

		png_write (pPng, "mandel.png");
	}

	//TODO: uncomment after you implement createPartition(int mpi_rank, int mpi_size)
	//MPI_Comm_free(&p.comm);
	free(c);
	MPI_Finalize();
	return 0;
}
