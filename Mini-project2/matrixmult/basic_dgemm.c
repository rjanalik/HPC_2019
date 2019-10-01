/* ==================================================================== *
 *
 *  basic_dgemm.c -- naive variant of the matrix-matrix multiplikation.
 *
 * ==================================================================== */

#include "square_dgemm.h"


void
square_dgemm (const double *A, const double *B,
	      double *C,
	      const unsigned dim)
{
    unsigned  i, j, k;

    for (i = 0; i < dim; i++)
    {
    	const double *A_ix = A + i*dim;

    	for (j = 0; j < dim; j++)
	{
            const double *B_xj = B + j;

            double	cij   = C[i*dim + j];
	    unsigned	k_ind = 0;

            for (k = 0; k < dim; k++)
	    {
    	    	cij += A_ix[k] * B_xj[k_ind];
		k_ind += dim;
            }

            C[i*dim + j] = cij;
    	}
    }
}
