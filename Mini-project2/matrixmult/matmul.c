/* ==================================================================== *
 *
 *  matmul.c -- Driver functions to measure the performance of
 *  Matrix-Multiplication
 *
 *  This file must be compiled with an additional routine that
 *  contains the square_dgemm routine.
 *
 * Based on an assignment given in 1999 at the University of Berkeley,
 * http://www.cs.berkeley.edu/~demmel/cs267_Spr99/
 *
 *  @author:	Olaf Schenk
 *  @date:	20013-11-01
 *
 * ==================================================================== */

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <float.h>
#include <math.h>

#include <sys/types.h>
#include <sys/resource.h>

#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#ifdef __APPLE__
#include <libgen.h>
#endif


#include "square_dgemm.h"


/* -------------------------------------------------------------------- */

/*
 * Minimum number of matrix-iterations and resulting minimum runtime
 * in order to minimize any time-measuring overhead.
 */
#define MIN_RUNS	4
#define MIN_CPU_SECS	2.0


/*
 *  Matrix sizes (n^2) that will be tested
 */
const unsigned test_sizes[] = {
    020,  24,  31,  32,  48,  64,  73,  96,  97, 127,
    128, 129, 163, 191, 192, 229, 255, 256, 257, 319,
    320, 321, 370, 417, 450, 479, 480, 511, 512, 600,
    700, 800, 900, 1000, 1200
};

/* Number of tested sizes */
#define N_SIZES ((unsigned) sizeof (test_sizes) / sizeof (unsigned))


/* -------------------------------------------------------------------- */

/*
 * Statically allocated matrix sizes (must be >= max(test_sizes))
 */

#define MAX_SIZE 1200


double A[MAX_SIZE * MAX_SIZE];
double B[MAX_SIZE * MAX_SIZE];
double C[MAX_SIZE * MAX_SIZE];


/* -------------------------------------------------------------------- */

/**
 *  Helper functions (random initialization and zeroing)
 */
void 
matrix_init (double	*A,
	     unsigned	 dim)
{
     unsigned i;

     for (i = 0; i < dim * dim; ++i) {
          A[i] = drand48 ();
     }
}


void 
matrix_clear (double	*C,
	      unsigned	 dim)
{
    memset (C, 0, dim * dim * sizeof (double));
}


/**
 * Test the correctness of any computed dgemm result C = A * B, with
 * special consideration towards rounding errors. This is important
 * because an improper implementation may be strongly affected by
 * rounding errors in the case of matrix-matrix multiplication.
 */
void
validate_dgemm (const double *A, const double *B,
		const double *C,
		const unsigned dim)
{
    unsigned i, j, k;

    for (i = 0; i < dim; ++i)
    {
        for (j = 0; j < dim; ++j)
	{

            /** Error approximation for the scalar product:
             *
             * float(sum(a_i * b_i)) = sum(a_i * b_i) * (1 + delta_i)
             * 
             * where
             *    delta_i  = n * epsilon
	     *    n	   = number of elements (= number of multiplications)
	     *    epsilon  = smallest representable floating point number (double precision)
             * 
             * We approximate the error by assuming that each
             * multiply-add as part of the scalar-product will have a
             * rounding error of epsilon. This error multiplied by the
             * dimensions (or number of elements in the scalar
             * product) will give an approximate error for each
             * element in C. The given answer in C should be within (3 * error_bound)
             * of the simplistic scalar product result
             * (which can actually be compiled to a FMA instruction
             * when the hardware supports it)
             */
                        
            double dot = 0.0;
            double error_bound = 0.0;
            double error;

            for (k = 0; k < dim; k++)
	    {
                double prod = A[dim*i + k] * B[dim*k + j];
                dot += prod;
                error_bound += fabs(prod);
            }
            error_bound *= (dim * DBL_EPSILON);
            error = fabs(C[dim*i + j] - dot);

            if (error > 3 * error_bound)
	    {
                printf("Error in the scalar product!\n");
                printf("C[%d,%d] should be %g, but is: %g.\n",
		       i, j, dot, C[dim*i + j]);
                printf("Error is: %g, vs. Error Tolerance: %g\n",
                       error, 3 * error_bound);

		/* Program Exit */
                exit(-1);
            }
        }
    }
}


unsigned long get_time(void)
{
    struct timeval	tp;
    gettimeofday(&tp, NULL);
    /* printf(">> %lu\n", tp.tv_sec * 1000000 + tp.tv_usec); */
    return  (tp.tv_sec * 1000000 + tp.tv_usec);
}


/**
 * Measure the performance of the linked square_dgemm function. The
 * number of floating point calculations is approximated as 2*(n^3)
 * per matrix-matrix multiply.
 */
double
time_dgemm (const double *A, const double *B,
	    double *C,
	    const unsigned dim)
{
    double	gflops, gflop_s;
    double	secs = 0.0;

    unsigned num_iterations = MIN_RUNS;

    while (secs < MIN_CPU_SECS)
    {
	unsigned long  start_clock, stop_clock;
	unsigned i;

	matrix_clear(C, dim);

        start_clock = get_time();
        for (i = 0; i < num_iterations; i++)
	{
            square_dgemm (A, B, C, dim);
        }
        stop_clock = get_time();
        secs    = (stop_clock - start_clock) / 1000000.0;

	gflops  = (2.0 * dim * dim * dim * num_iterations) / 1.0e9;
	gflop_s = gflops / secs;

	printf(".. runs:%5d,  time:%10g,  GFlop:%8g,  GFlop/s:%8g\n",
	       num_iterations, secs, gflops, gflop_s);

        num_iterations *= 2;
    }
    return gflop_s;
}


/* -------------------------------------------------------------------- */

/**
 *  Main driver
 *
 */
int
main (int  argc,  char  *argv[])
{
     unsigned	 i;
     double	 gflop_s;

     int	 validate = 1;
     int	 size_step = 1;

     FILE	*tmp_file;
     int	 tmp_file_des;
     char	*tmp_file_template = strdup(".matmul.data_XXXXXX");
     char	*timing_file_name = NULL;

     matrix_init (A, MAX_SIZE);
     matrix_init (B, MAX_SIZE);

     /* argv[0] is the program name */
     asprintf(&timing_file_name, "timing_%s.data", basename(argv[0]));

     /* Use a temporary file to avoid deleting existing data. */
     
     tmp_file_des = mkstemp(tmp_file_template);
     if (tmp_file_des == -1)
     {
	 fprintf(stderr, "ERROR: couldin't open file %s.\n", tmp_file_template);
	 return  EXIT_FAILURE;
     }
     tmp_file = fdopen(tmp_file_des, "w");

     /*
      * Performance test for each matrix size.
      */
     for (i = 0; i < N_SIZES; i += size_step)
     {
          const unsigned dim = test_sizes[i];

          gflop_s = time_dgemm(A, B, C, dim);
	  if (validate)
	  {
	      matrix_clear(C, MAX_SIZE);
	      square_dgemm(A, B, C, dim);
	      validate_dgemm (A, B, C, dim);
	  }

          printf("Size: %u\tGFlop/s: %g\n", dim, gflop_s);
	  fprintf(tmp_file, "%u %g\n", dim, gflop_s);
     }
     fclose(tmp_file);

     /* rename the (unique) temporary file */
     rename(tmp_file_template, timing_file_name);

     return  EXIT_SUCCESS;
}
