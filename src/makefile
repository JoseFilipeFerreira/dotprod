CC= gcc

CFLAGS= -std=c2x -Wall -Wextra -Werror=pedantic -pedantic-errors -Wfatal-errors -flto -march=native -mtune=native

OBJECTS:=$(patsubst %.c,%.o,$(wildcard *.c))
EXEC= dotprod

$(EXEC): $(OBJECTS)
	$(CC) -O2 -o $(EXEC) $(OBJECTS) $(CFLAGS)

clean:
	rm -rf *.o $(EXEC)
