gnu: dgemm_example.c
	gcc dgemm_example.c -lmkl_intel_lp64 -lmkl_core  -lmkl_sequential -lm -o dgemm_example

intel: dgemm_example.c
	icc dgemm_example.c -o dgemm_example_intel -mkl

clean:
	rm -f dgemm_example dgemm_example_intel
