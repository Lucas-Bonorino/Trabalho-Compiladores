IDIR =./
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=./
LDIR=./

_DEPS = Tree_Lex_Val.c parser.tab.c main.c lex.yy.c Gerencia_Tabela_de_Simbolos.c Tabela_de_Simbolos.c Code_Generation.c Gerador_Assembly.c Tradutor_Iloc_Assembly.c
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = Tree_Lex_Val.o parser.tab.o main.o lex.yy.o Gerencia_Tabela_de_Simbolos.o Tabela_de_Simbolos.o Code_Generation.o Gerador_Assembly.o Tradutor_Iloc_Assembly.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: parser scanner generate_obj etapa5 clean

parser: parser.y
	bison -d parser.y 

scanner: scanner.l
	flex scanner.l

generate_obj: $(DEPS)
	$(CC) -c $(DEPS)

etapa5: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o parser.tab* lex.* 