all: dotProduct

dotProduct: dotProduct.cpp walltime.h
	g++ -O3 -fopenmp $< -o $@

clean:
	rm -rf dotProduct
