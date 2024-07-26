//Por Lucas dos Santos Bonorino e Rafael Lacerda Busatta

#define NUM_ARGS 2

typedef struct ILOC_OPERATION OPERATION;
typedef struct LIST_ILOC_OPERATIONS PROGRAM; 

//Por simplificação, em um projeto inicial, faremos com que todas as operações usem apenas
//Registradores, sem o uso de imediatos. Todas os imediatos devem ser carregados para registradores
//antes das operações
typedef enum {NOP, ADD ,SUB, MUL, DIV, OR, AND, XOR, LOAD, LOADI,LOADAI, LOADA0, STORE, STOREA0, I2I, JUMPI, JUMP, CBR, CMP_LT, CMP_LE, CMP_EQ, CMP_GE, CMP_GT, CMP_NE } OP_TYPE;
typedef enum {GLOBAL=0, LOCAL=1 } ESCOPO;

struct ILOC_OPERATION
{   
    OP_TYPE instruction;
    char *parameters[NUM_ARGS];
    char *target;
};

struct LIST_ILOC_OPERATIONS
{
    OPERATION *operation;
    PROGRAM *next;
    PROGRAM *previous;
};

char *Cria_Label();

char *Cria_Temporario();

void Escreve_Codigo(OPERATION *operacao);

PROGRAM *Create_Operation(OPERATION *op);

PROGRAM *Load_Var(int Deslocamento, ESCOPO escopo_var);

PROGRAM *Load_Literal(char *literal);

PROGRAM *Binary_Operation(char *operation_type,PROGRAM *operand1, PROGRAM *operand2);






