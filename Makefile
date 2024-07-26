IDIR =./
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=./
LDIR=./

_DEPS = Tree_Lex_Val.c parser.tab.c main.c lex.yy.c Gerencia_Tabela_de_Simbolos.c Tabela_de_Simbolos.c Code_Generation.c
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = Tree_Lex_Val.o parser.tab.o main.o lex.yy.o Gerencia_Tabela_de_Simbolos.o Tabela_de_Simbolos.o Code_Generation.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: parser scanner generate_obj etapa4 clean

parser: parser.y
	bison -d parser.y 

scanner: scanner.l
	flex scanner.l

generate_obj: $(DEPS)
	$(CC) -c $(DEPS)

etapa4: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o parser.tab* lex.* 