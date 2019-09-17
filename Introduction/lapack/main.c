/*
 * LAPACK test code
 *
 * Use header file obtainable from http://www.netlib.org/lapack/lapacke.h
 * or explicitly specify signature of the used routines, i.e. 
 * extern "C" void dgetrf_(int* dim1, int* dim2, double* a, \
 *                         int* lda, int* ipiv, int* info);
 * extern "C" void dgetrs_(char *TRANS, int *N, int *NRHS, double *A, \
 *                         int *LDA, int *IPIV, double *B, int *LDB, int *INFO );
 *
 * Compile using -llapack flag (see Makefile)
 *
 * 
 * Juraj Kardos
 * USI Lugano
 */

#include <stdio.h>

extern void dgetrf_(int* dim1, int* dim2, double* a, \
                    int* lda, int* ipiv, int* info);
extern void dgetrs_(char *TRANS, int *N, int *NRHS, double *A, \
                    int *LDA, int *IPIV, double *B, int *LDB, int *INFO );

#define N 3
#define NRHS 1

int main()
{
    char trans = 'N';
    int dim = N;
    int nrhs = NRHS;
    int LDA = N;
    int LDB = N;
    int info;
       
    //column-wise storage
    double A[N*N] = {
        8.79,  9.93,  9.83,
        6.11,  6.91,  5.04,
       -9.15, -7.93,  4.86
    };
    
    double b[N*NRHS] = {
        1.0,
        1.0,
        1.0
    };
    

    int ipiv[N];

    printf("    %f %f %f\n", A[0], A[3], A[6]);
    printf("A = %f %f %f\n", A[1], A[4], A[7]);
    printf("    %f %f %f\n\n", A[2], A[5], A[8]);

    printf("    %f \n", b[0]);
    printf("b = %f \n", b[1]);
    printf("    %f \n\n", b[2]);

    //solve Ax = b
    dgetrf_(&dim, &dim, A, &LDA, ipiv, &info);
    printf("Factorization complete.\n");
    dgetrs_(&trans, &dim, &nrhs, A, &LDA, ipiv, b, &LDB, &info);
    printf("Solution complete.\n\n");

    //solution is in vector b
    printf("Solution y=A\\b is:\n");

   printf("    %f \n", b[0]);
   printf("y = %f \n", b[1]);
   printf("    %f \n", b[2]);

    return(0);
}
