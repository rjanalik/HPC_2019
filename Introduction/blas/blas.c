#include <stdio.h>

extern void daxpy_(int* n, double* a, double* x, int* incx, double* y, int* incy);
extern void dgemv_(char* trans, int* m, int* n, double* alpha, double* A, int* ldA, double* x, int* incx, double* beta, double* y, int* incy);
extern void dgemm_(char* transA, char* transB, int* m, int* n, int* k, double* alpha, double* A, int* ldA, double* B, int* ldB, double* beta, double* C, int* ldC);

int main()
{
   int n = 3;
   int ione = 1;
   double done = 1;
   char trans = 't';
   double a = 2;
   double x[3] = {1, 2, 3};
   double y[3] = {11, 12, 13};
   double A[9] = {1, 2, 3,
                  4, 5, 6,
                  7, 8, 9};
   double B[9] = {21, 22, 23,
                  24, 25, 26,
                  27, 28, 29};
   double C[9] = {0, 0, 0,
                  0, 0, 0,
                  0, 0, 0};

   printf("init:\n");
   printf("\n");
   printf("a = %f\n", a);
   printf("\n");
   printf("    %f\n", x[0]);
   printf("x = %f\n", x[1]);
   printf("    %f\n", x[2]);
   printf("\n");
   printf("    %f\n", y[0]);
   printf("y = %f\n", y[1]);
   printf("    %f\n", y[2]);
   printf("\n");
   printf("    %f %f %f\n", A[0], A[1], A[2]);
   printf("A = %f %f %f\n", A[3], A[4], A[5]);
   printf("    %f %f %f\n", A[6], A[7], A[8]);
   printf("\n");
   printf("    %f %f %f\n", B[0], B[1], B[2]);
   printf("B = %f %f %f\n", B[3], B[4], B[5]);
   printf("    %f %f %f\n", B[6], B[7], B[8]);
   printf("\n");
   printf("    %f %f %f\n", C[0], C[3], C[6]);
   printf("C = %f %f %f\n", C[1], C[4], C[7]);
   printf("    %f %f %f\n", C[2], C[5], C[8]);
   printf("\n");

   printf("BLAS1 example - axpy: y = a * x + y\n");
   daxpy_(&n, &a, x, &ione, y, &ione);
   printf("\n");
   printf("    %f\n", y[0]);
   printf("y = %f\n", y[1]);
   printf("    %f\n", y[2]);
   printf("\n");

   printf("BLAS2 example - gemv: y = A * x + y\n");
   dgemv_(&trans, &n, &n, &done, A, &n, x, &ione, &done, y, &ione);
   printf("\n");
   printf("    %f\n", y[0]);
   printf("y = %f\n", y[1]);
   printf("    %f\n", y[2]);
   printf("\n");

   printf("BLAS3 example - gemm: C = A * B + C\n");
   dgemm_(&trans, &trans, &n, &n, &n, &done, A, &n, B, &n, &done, C, &n);
   printf("\n");
   printf("    %f %f %f\n", C[0], C[3], C[6]);
   printf("C = %f %f %f\n", C[1], C[4], C[7]);
   printf("    %f %f %f\n", C[2], C[5], C[8]);
   printf("\n");

   return 0;
}
