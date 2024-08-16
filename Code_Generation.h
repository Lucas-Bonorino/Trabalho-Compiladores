//Por Lucas dos Santos Bonorino e Rafael Lacerda Busatta
#define USED_REG 1
#define FREE_REG 0
#define NUM_ARGS 2
#define NUM_REG 14



typedef struct ILOC_OPERATION OPERATION;
typedef struct LIST_ILOC_OPERATIONS PROGRAM; 
typedef struct BASIC_BLOCK DAG_NODE;

//Por simplificação, em um projeto inicial, faremos com que todas as operações usem apenas
//Registradores, sem o uso de imediatos. Todas os imediatos devem ser carregados para registradores
//antes das operações
typedef enum {NOP, ADD ,SUB, RSUBI, MULT, DIV, OR, AND, LOADI,LOADAI, STOREAI, JUMPI, CBR, CMP_LT, CMP_LE, CMP_EQ, CMP_GE, CMP_GT, CMP_NE, RETURN} OP_TYPE;
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
    //Adicionado um booleano para dizer se uma instrução é líder ou não
    int Is_Leader;
};

char *Cria_Label();

char *Cria_Temporario();

void Escreve_Codigo(OPERATION *operacao);

PROGRAM *Create_Operation(OPERATION *op);

PROGRAM *Load_Var(int Deslocamento, ESCOPO escopo_var, char *nome_var);

PROGRAM *Load_Literal(char *literal);

PROGRAM *Unary_Operation(char *operation_type, PROGRAM *operand);

PROGRAM *Binary_Operation(char *operation_type,PROGRAM *operand1, PROGRAM *operand2);

PROGRAM *Atribution(PROGRAM *expression, int Deslocamento, ESCOPO escopo_var, char *nome_var);

PROGRAM *Conditional_Flux(PROGRAM *expression, PROGRAM *comand_block, PROGRAM *else_block);

PROGRAM *Iterative_Flux(PROGRAM *expression, PROGRAM *command_block);

PROGRAM *Return(PROGRAM *operand);

void Print_Program(PROGRAM *programa);

PROGRAM *Concat_Iloc_Op_Lists(PROGRAM *first_part, PROGRAM *second_part);

PROGRAM *Append_Op(PROGRAM *first_op, PROGRAM *second_op);

char *Float_Int_Conversion(char *float_represented);

PROGRAM *Find_First(PROGRAM *op_list);

//Estrutura dos nodos DAG
struct BASIC_BLOCK
{
    int Block_Num;
    int Num_Next_Blocks;
    DAG_NODE **Next_Blocks;
};


