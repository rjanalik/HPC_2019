CC=gcc
CFLAGS=-Wall
LDFLAGS=
LIBS=-lblas -llapack
SOURCES=main.c
EXECUTABLE=main

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(SOURCES) -o $@ $(LIBS)
clean:
	rm -f main.o main
