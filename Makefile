CC=gcc
CFLAGS=-I.
DEPS = tokens.h
OBJ = main.o scanner.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: etapa1 clean

etapa1: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o