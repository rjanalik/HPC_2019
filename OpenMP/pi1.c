#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#ifdef _OPENMP
#  include <omp.h>
#endif

#define f(A) (4.0/(1.0+A*A))

const int n = 10000000;

int main(int argc, char** argv)
{
  int i;
  double w,x,sum,pi;
  clock_t t1,t2;
  struct timeval tv1,tv2; struct timezone tz;
# ifdef _OPENMP
    double wt1,wt2;
# endif
 
# ifdef _OPENMP
    int myrank, num_threads;
#   pragma omp parallel private(myrank, num_threads)
    { 
      myrank = omp_get_thread_num(); 
      num_threads = omp_get_num_threads();
      printf("I am thread %2d of %2d threads\n", myrank, num_threads); 
    } /* end omp parallel */
# else
    printf("This program is not compiled with OpenMP\n");
# endif

  gettimeofday(&tv1, &tz);
# ifdef _OPENMP
    wt1=omp_get_wtime();
# endif
  t1=clock();
 
/* calculate pi = integral [0..1] 4/(1+x**2) dx */
  w=1.0/n;
  sum=0.0;
  for (i=1;i<=n;i++)
  {
    x=w*((double)i-0.5);
    sum=sum+f(x);
  }
  pi=w*sum;

  t2=clock();
# ifdef _OPENMP
    wt2=omp_get_wtime();
# endif
  gettimeofday(&tv2, &tz);
  printf( "computed pi = %24.16g\n", pi );
  printf( "CPU time (clock)                = %12.4g sec\n", (t2-t1)/1000000.0 );
# ifdef _OPENMP
    printf( "wall clock time (omp_get_wtime) = %12.4g sec\n", wt2-wt1 );
# endif
  printf( "wall clock time (gettimeofday)  = %12.4g sec\n", (tv2.tv_sec-tv1.tv_sec) + (tv2.tv_usec-tv1.tv_usec)*1e-6 );
  return 0;
}
