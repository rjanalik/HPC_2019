gnu: blas.c
	gcc blas.c -o blas -lblas

intel: blas.c
	icc blas.c -o blas_intel -mkl

clean:
	rm -f blas blas_intel
