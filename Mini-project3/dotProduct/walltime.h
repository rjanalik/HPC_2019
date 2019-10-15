#ifndef WALLTIME_H
#define WALLTIME_H

#include <sys/time.h>

/** walltime() -- gives the elapsed time since 't0'
Usage:
 double t0 = walltime(0);
 runtask();
 printf("Task took: %f\n",walltime(t0));
*/

double walltime(double t0){
    double mic;
    double time;
    double mega = 0.000001;
    struct timeval tp;
    struct timezone tzp;

    (void) gettimeofday(&tp, &tzp);

    time = (double)(tp.tv_sec);
    mic  = (double)(tp.tv_usec);
    time = (time + mic * mega) - t0;
    return(time);
}

#endif // WALLTIME_H
