.PHONY: benchmark clean tar

UNAME = $(shell uname)

ifeq ($(UNAME),Linux)
	GOTO=-Wl,--no-as-needed -L${MKLROOT}/lib/intel64 -lmkl_intel_lp64 -lmkl_core -lmkl_sequential -lpthread -lm -ldl -m64 -I${MKLROOT}/include
else
	ifeq ($(UNAME),Darwin)
	GOTO=-Wl,-L${MKLROOT}/lib -Wl,-rpath,${MKLROOT}/lib -lmkl_intel_lp64 -lmkl_core -lmkl_sequential -lpthread -lm -ldl -m64 -I${MKLROOT}/include
	else
		GOTO=-lblas
	endif
endif


#
# Compiler and compiler flags
#
CC = gcc
#CC = icc
CFLAGS = -Wall -pedantic -O3 -std=gnu99
#CFLAGS = -Wall -O3 -std=gnu99
#CFLAGS = -Wall -g -O0 -std=gnu99

#
# Linker flags to resolve calls from external libraries
#

#GOTO = -mkl -static-intel
LDLIBS = $(GOTO)
 


EXECS = basic_dgemm blas_dgemm blocked_dgemm 

all: $(EXECS)

basic_dgemm: matmul.c basic_dgemm.c
blocked_dgemm: matmul.c blocked_dgemm.c
blas_dgemm: matmul.c blas_dgemm.c
blocked_dgemm: CFLAGS+=-funroll-loops

timing.ps: $(EXECS)
	sbatch ./run_matrixmult.sh
	module load gnuplot
	gnuplot timing.gp

benchmark: timing.ps $(EXECS)

clean:
	rm -f $(EXECS) *.o *.ps .matmul.data_* *.data *.out

tar:
	cd ../; tar -cf matrixmult.tar matrixmult/*

