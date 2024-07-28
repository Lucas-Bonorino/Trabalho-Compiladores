//Por Lucas dos Santos Bonorino e Rafael Lacerda Busatta

#define NUM_ARGS 2

typedef struct ILOC_OPERATION OPERATION;
typedef struct LIST_ILOC_OPERATIONS PROGRAM; 

//Por simplificação, em um projeto inicial, faremos com que todas as operações usem apenas
//Registradores, sem o uso de imediatos. Todas os imediatos devem ser carregados para registradores
//antes das operações
typedef enum {NOP, ADD ,SUB, MULT, DIV, OR, AND, XOR, LOAD, LOADI,LOADAI, LOADA0, STORE, STOREAI, STOREA0, I2I, JUMPI, JUMP, CBR, CMP_LT, CMP_LE, CMP_EQ, CMP_GE, CMP_GT, CMP_NE } OP_TYPE;
typedef enum {GLOBAL=0, LOCAL=1 } ESCOPO;

struct ILOC_OPERATION
{   
    OP_TYPE instruction;
    char *parameters[NUM_ARGS];
    char *target[NUM_ARGS];
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

PROGRAM *Atribution(PROGRAM *expression, int Deslocamento, ESCOPO escopo_var);

PROGRAM *Conditional_Flux(PROGRAM *expression, PROGRAM *comand_block, PROGRAM *else_block);

PROGRAM *Iterative_Flux(PROGRAM *expression, PROGRAM *command_block);

void Print_Program(PROGRAM *programa);

PROGRAM *Concat_Iloc_Op_Lists(PROGRAM *first_part, PROGRAM *second_part);

PROGRAM *Append_Op(PROGRAM *first_op, PROGRAM *second_op);




