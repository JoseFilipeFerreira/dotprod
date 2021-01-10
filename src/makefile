CC= gcc

CFLAGS= -std=c11 -Wall -Wextra -Werror=pedantic -pedantic-errors -Wfatal-errors -flto -march=native -mtune=native

INCLUDES = -I. -I/share/apps/papi/5.4.1/include
LIBDIRS = -L/share/apps/papi/5.4.1/lib

OBJECTS:=$(patsubst %.c,%.o,$(wildcard *.c))
EXEC= dotprod

$(EXEC): $(OBJECTS)
	$(CC) -O2 $(CFLAGS) $(INCLUDES) $(LIBDIRS) $(OBJECTS) -o $(EXEC) -lpapi

debug: $(OBJECTS)
	$(CC) -g $(CFLAGS) $(INCLUDES) $(LIBDIRS) $(OBJECTS) -o $(EXEC) -lpapi

clean:
	rm -rf *.o $(EXEC)
