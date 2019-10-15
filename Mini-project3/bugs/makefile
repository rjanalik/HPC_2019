CFLAGS = -g -O3 -fopenmp

all: omp_bug1 omp_bug2 omp_bug3 omp_bug4 omp_bug5

omp_bug1: omp_bug1.c
	gcc $(CFLAGS) $< -o $@

omp_bug2: omp_bug2.c
	gcc $(CFLAGS) $< -o $@

omp_bug3: omp_bug3.c
	gcc $(CFLAGS) $< -o $@

omp_bug4: omp_bug4.c
	gcc $(CFLAGS) $< -o $@

omp_bug5: omp_bug5.c
	gcc $(CFLAGS) $< -o $@

clean:
	rm -rf omp_bug1 omp_bug2 omp_bug3 omp_bug4 omp_bug5

