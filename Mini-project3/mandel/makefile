all: mandel_seq

mandel_seq: mandel_seq.c pngwriter.c
	gcc -o $@ -I. -O3 $^ -lpng

clean:
	rm -rf mandel_seq

