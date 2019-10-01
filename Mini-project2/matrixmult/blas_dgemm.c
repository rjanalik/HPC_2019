/* ==================================================================== *
 *									*
 *  atlas_dgemm.c -- Benutzt die Matrix-Multiplikation aus der		*
 *		     optimierten ATLAS-Bibliothek.			*
 *									*
 * ==================================================================== */

#include "square_dgemm.h"


/**
 * Wrapper to call BLAS library DGEMM routine (GotoBLAS, ATLAS, etc) 
 * The function calculates:
 * 
 *     [C] = alpha*op([A])*op([B]) + beta*[C]
 * 
 * Where:
 * op() is an operator controlled by:
 * 'N' for "[N]o Transpose"
 * 'T' for a hermitian transpose (no complex conjugation)
 * 'C' for a transpose with complex conjugation (however, since we are
 *     dealing with real matrices, this is identical to 'T')
 * 
 * NOTE: This routine is the fortran version, which expects matrix to
 * be stored in column-major order. Also note that it expects all
 * input parameters to be given as pointers, including matrix size and
 * transpose characters. Also note that the underscore at the end of
 * the function name is a fortran compiler convention.

 *  You can get the full specification by checking:  `man dgemm'.
 *
 *  Parameter:
 *  
 *    transA	: Tranpose [A]? ('T', 'N', 'C')
 *    transB	: Tranpose [B]? ('T', 'N', 'C')
 *
 *    M		: Number of rows of op([A]) und [C]
 *    N		: Number of columns of  op([B]) und [C]
 *    K		: Number of columns of op([A]) und op([B])
 *
 *    alpha	: scalar factor of [A] x [B]
 *
 *    A		: pointer to [A]
 *    ldA	: dimension of [A]
 *
 *    B		: pointer to [B]
 *    ldB	: dimension of [B]
 *
 *    beta	: scalar factor of [C]
 *    C		: pointer to [C]
 *    ldC	: dimension of [C]
 */

extern
int dgemm_(char* transA, char* transB,
	   const unsigned *M, const unsigned *N, const unsigned* K,
	   const double *alpha, 
	   const double *A, const unsigned *ldA,
	   const double *B, const unsigned *ldB,
	   const double *beta,
	   double *C, const unsigned *ldC);


/**
 *  square_dgemm -- Wrapper for an external dgemm following the BLAS standard
 *
 */
void
square_dgemm(const double *A,  const double *B,
	     double *C,
	     const unsigned M) 
{
    char    trans = 'N';
    double  one = 1.0;

    /* note that fortran passes all function arguments by reference,
       hence &M,&one,etc */
    dgemm_(&trans, &trans,   
	   &M, &M, &M,
	   &one,
	   B, &M,
	   A, &M,
	   &one,
	   C, &M);
}
