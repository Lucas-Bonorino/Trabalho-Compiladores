#include "Code_Generation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 10
#define NUM_BINARY_OPERATIONS 12

int Last_Label=0;
int Last_Temp=0;

char *Cria_Label()
{
    char *label_buffer;

    label_buffer=(char*)malloc(sizeof(char)*BUFFER_SIZE);

    sprintf(label_buffer, "L%d", Last_Label);
    Last_Label+=1;

    return(label_buffer);
}

char *Cria_Temporario()
{
    char *temp_buffer;

    temp_buffer=(char*)malloc(sizeof(char)*BUFFER_SIZE);

    sprintf(temp_buffer, "r%d", Last_Temp);
    Last_Temp+=1;

    return(temp_buffer);
}

//Essa função escreve o código de uma única instrução
void Escreve_Codigo(OPERATION *operacao)
{
    switch(operacao->instruction)
    {
        case NOP:    printf("nop\n"); break;
        case ADD:    printf("add %s, %s => %s\n", operacao->parameters[0], operacao->parameters[1], operacao->target); break;
        case SUB:    printf("sub %s, %s => %s\n", operacao->parameters[0], operacao->parameters[1], operacao->target); break;
        case MUL:    printf("mul %s, %s => %s\n", operacao->parameters[0], operacao->parameters[1], operacao->target); break;
        case DIV:    printf("div %s, %s => %s\n", operacao->parameters[0], operacao->parameters[1], operacao->target); break;
        case OR:     printf("or %s, %s => %s\n", operacao->parameters[0], operacao->parameters[1], operacao->target); break;
        case AND:    printf("and %s, %s => %s\n", operacao->parameters[0], operacao->parameters[1], operacao->target); break;
        case XOR:    printf("xor %s, %s => %s\n", operacao->parameters[0], operacao->parameters[1], operacao->target); break;
        case LOAD:   printf("load %s => %s\n", operacao->parameters[0], operacao->target); break;
        case LOADI:  printf("loadI %s => %s\n", operacao->parameters[0], operacao->target); break;
        case LOADAI: printf("loadAI %s, %s => %s\n", operacao->parameters[0], operacao->parameters[1], operacao->target); break;
        case LOADA0: printf("loadA0 %s, %s => %s\n", operacao->parameters[0], operacao->parameters[1], operacao->target); break;
        case STORE:  printf("store %s => %s\n", operacao->parameters[0], operacao->target); break;
        case STOREA0:printf("storeA0 %s => %s, %s\n", operacao->parameters[0], operacao->parameters[1], operacao->target); break;
        case I2I:    printf("i2i %s => %s\n", operacao->parameters[0], operacao->target); break;
        case JUMPI:  printf("jumpI -> %s\n", operacao->parameters[0]); break;
        case JUMP:   printf("jump -> %s\n", operacao->parameters[0]); break;
        case CBR:    printf("cbr %s -> %s, %s\n", operacao->parameters[0], operacao->parameters[1], operacao->target); break;
        case CMP_LT: printf("cmp_LT %s, %s -> %s\n", operacao->parameters[0], operacao->parameters[1], operacao->target); break;
        case CMP_LE: printf("cmp_LE %s, %s -> %s\n", operacao->parameters[0], operacao->parameters[1], operacao->target); break;
        case CMP_EQ: printf("cmp_EQ %s, %s -> %s\n", operacao->parameters[0], operacao->parameters[1], operacao->target); break;
        case CMP_GE: printf("cmp_GE %s, %s -> %s\n", operacao->parameters[0], operacao->parameters[1], operacao->target); break;
        case CMP_GT: printf("cmp_GT %s, %s -> %s\n", operacao->parameters[0], operacao->parameters[1], operacao->target); break;
        case CMP_NE: printf("cmp_NE %s, %s -> %s\n", operacao->parameters[0], operacao->parameters[1], operacao->target); break;
    }
}

PROGRAM *Create_Operation(OPERATION *op)
{
    PROGRAM *New_Operation;

    New_Operation=(PROGRAM*)malloc(sizeof(PROGRAM));
    New_Operation->operation=op;
    New_Operation->next=NULL;
    New_Operation->previous=NULL;
    Escreve_Codigo(op);

    return(New_Operation);
}

PROGRAM *Load_Var(int Deslocamento, ESCOPO escopo_var)
{
    const char *registers[]={"rbss", "rfp"};
    OPERATION *operacao;
    operacao=(OPERATION*)malloc(sizeof(OPERATION));

    operacao->instruction=LOADAI; 

    char *imediato;
    imediato=(char*)malloc(sizeof(char)*BUFFER_SIZE);
    sprintf(imediato, "%d", Deslocamento);

    operacao->parameters[0]=strdup(registers[escopo_var]);
    operacao->parameters[1]=imediato;
    operacao->target=Cria_Temporario();

    return(Create_Operation(operacao));
}

PROGRAM *Load_Literal(char *literal)
{
    OPERATION *operacao;
    operacao=(OPERATION*)malloc(sizeof(OPERATION));

    operacao->instruction=LOADI;

    operacao->parameters[0]=strdup(literal);
    operacao->target=Cria_Temporario();

    return(Create_Operation(operacao)); 
}

int Return_OP(char *desired_op)
{   
    const char *operations[]={"+", "-", "*", "/", "|", "&", "<", "<=", ">", ">=", "==", "!="};
    int i;
    for(i=0; i<NUM_BINARY_OPERATIONS; i++)
    {
        if(strcmp(desired_op, operations[i])==0)
        {
            return(i);
        }
    }
}

PROGRAM *Binary_Operation(char *operation_type,PROGRAM *operand1, PROGRAM *operand2)
{
    const int iloc_operation[]={ ADD ,SUB, MUL, DIV, OR, AND, CMP_LT, CMP_LE, CMP_GT, CMP_GE, CMP_EQ, CMP_NE}; 

    OPERATION *operacao;
    operacao=(OPERATION*)malloc(sizeof(OPERATION));

    operacao->instruction=iloc_operation[Return_OP(operation_type)];

    operacao->target=Cria_Temporario();

    operacao->parameters[0]=strdup(operand1->operation->target);
    operacao->parameters[1]=strdup(operand2->operation->target);

    return(Create_Operation(operacao));
}