/* ==================================================================== *
 *                                                                      *
 *  membench.c --   Measurement of the performance of the memory        *
 *                  hierarchy.                                          *
 *                                                                      *
 *  Based on an assignment of the University of Berkeley from           *
 *  1999, http://www.cs.berkeley.edu/~demmel/cs267_Spr99/               *
 *                                                                      *
 *  @author:    Olaf Schenk                                             *
 *  @date:      2013-03-01                                              *
 *                                                                      *
 * ==================================================================== */

#include <unistd.h>
#include <stdio.h>
#include <sys/resource.h>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <limits.h>

#define CACHE_MIN (128)         /* smallest cache */
#define CACHE_MAX (1024 * 1024) /* largest cache */
#define	SAMPLE	10              /* to get larger time sample */

int x[CACHE_MAX];               /* stride thru this array */

/**
 * Get the number of CPU ticks since last booting the computer
 */
inline unsigned long long getCPUTick (void)
{
    unsigned lo, hi;
    asm volatile ("rdtsc" : "=a" (lo), "=d" (hi));
    return (unsigned long long) hi << 32 | lo;
}

/**
 * Get the current system time in milliseconds
 */
unsigned long timeGetTime (void)
{
    /* Using Linux Time Functions To Determine Time */
    struct timeval tv;
    gettimeofday (&tv, 0);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

/**
 * Determine the CPU clock speed.
 * @param nTime The time in milliseconds used to perform the measurement
 */
unsigned long getCPUSpeed (long nTime)
{
    long long timeStart, timeStop;
    long long startTick, endTick;

    long long overhead = getCPUTick () - getCPUTick ();

    /* Calculate Starting Time And Start Tick */
    timeStart = timeGetTime ();
    while (timeGetTime () == timeStart)
        timeStart = timeGetTime();

    while (1)
    {
        timeStop = timeGetTime ();
        if ((timeStop - timeStart) > 1)
        {
            startTick = getCPUTick ();
            break;
        }
    }

    /* Calculate Stop Time And End Tick */
    timeStart = timeStop;
    while (1)
    {
        timeStop = timeGetTime();
        if ((timeStop - timeStart) > nTime)
        {
            endTick = getCPUTick();
            break;
        }
     }

     /* Return The Processors Speed In Hertz */
     return (unsigned long) ((endTick - startTick) + (overhead));
}


int main ()
{
    int register i, index, stride, limit, temp;
    long steps, tsteps;
    int csize;

    /* timing variables */
    double sec;

    /* number of processor cycles used */
    unsigned long long cycles0, cycles;

    /* The CPU speed in Hz */
    unsigned long nHz = getCPUSpeed (1000);

    for (csize = CACHE_MIN; csize <= CACHE_MAX; csize <<= 1)
    {
        for (stride = 1; stride <= csize / 2; stride <<= 1)
        {
            /* init cycles counter */
            cycles = 0;

            /* cache size this loop */
            limit = csize - stride + 1;
            steps = 0;

            do
            {		
                cycles0 = getCPUTick ();
                for (i = SAMPLE * stride; i != 0; i--)
                {
                    /* larger sample */
                    for (index = 0; index < limit; index += stride)
                    {
                        /* cache access */
                        x[index] = x[index] + 1;
                    }	
                }

                /* count while loop iterations */
                steps++;
                cycles += getCPUTick () - cycles0;
            } while (cycles < nHz); /* repeat until collected 1 sec */
            sec = cycles / (double) nHz;

            /* repeat empty loop to subtract loop overhead */

            /* used to match # while iterations */
            tsteps = 0;

            /* repeat until same # iterations as above */
            do
            {
                cycles0 = getCPUTick ();
                for (i = SAMPLE * stride; i != 0; i--)
                {
                    /* larger sample */

                    for (index = 0; index < limit; index += stride)
                    {
                        /* dummy code */
                        temp = temp + index;
                    }
                }

                /* count while loop iterations */
                tsteps++;
                cycles += getCPUTick () - cycles0;
            } while (tsteps < steps);

            printf ("Size:%7lu Stride:%7lu read+write:%10.3f ns, sec = %6.3f, cycles = %lld steps = %6.0f\n",
                csize * sizeof (int), stride * sizeof (int),
                (double) sec * 1e9 / (steps * SAMPLE * stride * ((limit - 1) / stride + 1)),
                sec, cycles, (double) steps);
            fflush(stdout);
        }

        printf ("\n\n");
    }

    return 0;
}
