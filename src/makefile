CC= gcc

CFLAGS= -std=c11 -Wall -Wextra -Werror=pedantic -pedantic-errors -Wfatal-errors -flto -march=native -mtune=native -lpapi -I. -I/share/apps/papi/5.5.0/include -L/share/apps/papi/5.5.0/lib

OBJECTS:=$(patsubst %.c,%.o,$(wildcard *.c))
EXEC= dotprod

$(EXEC): $(OBJECTS)
	$(CC) -O2 $(CFLAGS) $(OBJECTS) -o $(EXEC)

debug: $(OBJECTS)
	$(CC) -g $(CFLAGS) $(OBJECTS) -o $(EXEC) -lpapi

clean:
	rm -rf *.o $(EXEC)
