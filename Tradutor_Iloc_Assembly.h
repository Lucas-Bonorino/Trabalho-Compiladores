#include "Estruturas.h"

//Precisamos usar movs condicionais, uma vez que misturamos operadores relacionais com aritméticos
typedef enum { ADDL, SUBL, DIVL, MULL, ANDL, ORL, MOVL, CMPL, CLTD, JMP, JMPNZ, CMOVE, CMOVG, CMOVGE, CMOVEL, CMOVELE, CMOVNE } ASSEMBLY_OP;

PROGRAM *Traduzir(PROGRAM *codigo);