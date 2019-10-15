// #include <omp.h>
#include <iostream>
#include "walltime.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

#define NUM_ITERATIONS 100

// Example benchmarks
// 0.03s ~0.8MB
#define NMAX 100000
// 0.3s ~8MB
// #define NMAX 1000000
// 3.s ~80MB
// #define NMAX 10000000
// 30s ~800MB 
// #define NMAX 100000000
#define EPSILON 0.1


using namespace std;

int main()
{	int myId, numTdreads;
  double time_serial, time_start=0.0;
  long double dotProduct;
  double *a,*b;

  // Allocate memory for the vectors as 1-D arrays
  a = new double[NMAX];
  b = new double[NMAX];
  
  // Initialize the vectors with some values
  for(int i=0; i<NMAX; i++)
    {
      a[i] = i;
      b[i] = i/10.0;
    }

  long double alpha = 0;
  
  // serial execution
  // Note that we do extra iterations to reduce relative timing overhead
  time_start = walltime(0);
  for(int iterations=0;iterations<NUM_ITERATIONS;iterations++) {
    alpha=0.0;
    for(int i=0; i< NMAX; i ++)
      {
        alpha += a[i] * b[i];
      }
  }
  time_serial = walltime(time_start);
  cout << "Serial execution time = " << time_serial << " sec" << endl;
  
  long double alpha_parallel = 0;
  double time_red=0;
  double time_critical=0;
  // TODO: Write parallel version (2 ways!)
  //   i. Using reduction pragma
  //   ii. Using critical pragma
  

  if( (fabs(alpha_parallel - alpha)/fabs(alpha_parallel)) > EPSILON) {
    cout << "parallel reduction: " << alpha_parallel << " serial :" << alpha << "\n";
    cerr << "Alpha not yet implemented correctly!\n";
    exit(1);
  }
  cout << "Parallel dot product = " << alpha_parallel
       << " time using reduction method = " << time_red
       << " sec, time using critical method " << time_critical
       << " sec" << endl;
  
  // De-allocate memory
  delete [] a;
  delete [] b;

  return 0;
}
