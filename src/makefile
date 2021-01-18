CC=gcc

CFLAGS= -O2 -ftree-vectorize -ftree-vectorizer-verbose=2 -fopt-info-vec-all -std=c11 -Wall -Wextra -lpapi -fopenmp

OBJECTS:=$(patsubst %.c,%.o,$(wildcard *.c))
EXEC= dotprod

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXEC)

clean:
	rm -rf *.o $(EXEC)
