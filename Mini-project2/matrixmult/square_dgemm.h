/* ==================================================================== *
 *									*
 *  square_dgemm.h -- Prototype Matrix-Matrix function stub		*
 *									*
 * ==================================================================== */


#ifndef __SQUARE_DGEMM_H
#define __SQUARE_DGEMM_H


/**
 * Function prototype that the variants of square_dgemm should implement.
 */
void square_dgemm (const double *A, const double *B,
		   double *C,
		   const unsigned M);



#endif  /* __SQUARE_DGEMM_H */
