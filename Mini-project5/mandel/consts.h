#ifndef CONSTS_H_
#define CONSTS_H_

// maximum number of iterations
#define MAX_ITERS     35207

// image size
#define IMAGE_WIDTH   4096
#define IMAGE_HEIGHT  4096

// the extent of the parameter plane ( MIN_X + iMIN_Y <= c < MAX_X + iMAX_Y ) 
#define MIN_X   -2.1
#define MAX_X    0.7
#define MIN_Y   -1.4
#define MAX_Y    1.4

typedef struct
{
    long nx;
    long ny;
    long startx;
    long starty;
    long endx;
    long endy;
} Domain;

typedef struct
{
    int y;
    int x;
    int nx;
    int ny;
    MPI_Comm comm;
} Partition;

/**
Structure Partition represents the layout of the grid of processes
organized in the Cartesian communicator (p.comm) that needs to be created (MPI_Cart_create)
and contains information such as number of processes in x and y direction (p.nx, p.ny)
and the coordinates of the current MPI process (p.x, p.y).

*/
Partition createPartition(int mpi_rank, int mpi_size)
{
    Partition p;

    // TODO: determine size of the grid of MPI processes (p.nx, p.ny), see MPI_Dims_create()
    p.ny = 1;
    p.nx = 1;

    // TODO: Create cartesian communicator (p.comm), we do not allow the reordering of ranks here, see MPI_Cart_create()
    MPI_Comm comm_cart = MPI_COMM_WORLD;
    p.comm = comm_cart;
    
    // TODO: Determine the coordinates in the Cartesian grid (p.x, p.y), see MPI_Cart_coords()
    p.y = 0;
    p.x = 0;

    return p;
}

/**
Updates Partition structure to represent the process mpi_rank.
Copy the grid information (p.nx, p.ny and p.comm) and update
the coordinates to represent position in the grid of the given
process (mpi_rank)
*/
Partition updatePartition(Partition p_old, int mpi_rank)
{
    Partition p;

    // copy grid dimension and the communicator
    p.ny = p_old.ny;
    p.nx = p_old.nx;
    p.comm = p_old.comm;
    
    // TODO: update the coordinates in the cartesian grid (p.x, p.y) for given mpi_rank, see MPI_Cart_coords()
    p.y = 0;
    p.x = 0;

    return p;
}

/**
Structure Domain represents the information about the local domain of the current MPI process.
It holds information such as the size of the local domain (number of pixels in each dimension - d.nx, d.ny)
and its global indices (index of the first and the last pixel in the full image of the Mandelbrot set
that will be computed by the current process d.startx, d.endx and d.starty, d.endy).
*/
Domain createDomain(Partition p)
{
    Domain d;
    
    // TODO: compute size of the local domain
    d.nx = IMAGE_WIDTH;
    d.ny = IMAGE_HEIGHT;

    // TODO: compute index of the first pixel in the local domain
    d.startx = 0;
    d.starty = 0;

    // TODO: compute index of the last pixel in the local domain
    d.endx = IMAGE_WIDTH - 1;
    d.endy = IMAGE_HEIGHT - 1;

    return d;
}


#endif /*CONSTS_H_*/
