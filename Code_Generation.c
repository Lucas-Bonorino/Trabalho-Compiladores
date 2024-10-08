//Por Lucas dos Santos Bonorino e Rafael Lacerda Busatta
#include "Code_Generation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 10
#define NUM_BINARY_OPERATIONS 12

int Last_Label=0;
int Used_Temp[NUM_REG]={0};

//Consideramos que um registrador não será usado por variável, mas sim por temporário
//Sua vida útil torna-se o momento em que um valor é movido para ele, até o momento em que 
//O valor dele é usado
int Aloca_Reg()
{
    for(int i=0; i<NUM_REG; i++)
    {
        if(!Used_Temp[i])
        {
            Used_Temp[i]=USED_REG;
            return(i);
        }
    }

    printf("Erro, numero de labels invalido\n");
    exit(30);
}

void Libera_Reg(char *reg_used)
{
    char *buff=reg_used+1;

    int numero_registrador=atoi(buff);

    Used_Temp[numero_registrador]=FREE_REG;

}

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

    sprintf(temp_buffer, "r%d", Aloca_Reg());

    return(temp_buffer);
}

//Essa função escreve o código de uma única instrução
void Escreve_Codigo(OPERATION *operacao)
{
    switch(operacao->instruction)
    {
        case NOP:    printf("%s: nop\n", operacao->target[0]); break;
        case ADD:    printf("add %s, %s => %s\n", operacao->parameters[0], operacao->parameters[1], operacao->target[0]); break;
        case SUB:    printf("sub %s, %s => %s\n", operacao->parameters[0], operacao->parameters[1], operacao->target[0]); break;
        case RSUBI:  printf("rsubI %s, %s => %s\n", operacao->parameters[0], operacao->parameters[1], operacao->target[0]); break;
        case MULT:   printf("mult %s, %s => %s\n", operacao->parameters[0], operacao->parameters[1], operacao->target[0]); break;
        case DIV:    printf("div %s, %s => %s\n", operacao->parameters[0], operacao->parameters[1], operacao->target[0]); break;
        case OR:     printf("or %s, %s => %s\n", operacao->parameters[0], operacao->parameters[1], operacao->target[0]); break;
        case AND:    printf("and %s, %s => %s\n", operacao->parameters[0], operacao->parameters[1], operacao->target[0]); break;
        case LOADI:  printf("loadI %s => %s\n", operacao->parameters[0], operacao->target[0]); break;
        case LOADAI: printf("loadAI %s, %s => %s\n", operacao->parameters[0], operacao->parameters[1], operacao->target[0]); break;
        case STOREAI:printf("storeAI %s => %s, %s\n", operacao->parameters[0], operacao->target[0], operacao->target[1]); break;
        case JUMPI:  printf("jumpI -> %s\n", operacao->target[0]); break;
        case CBR:    printf("cbr %s -> %s, %s\n", operacao->parameters[0], operacao->target[0], operacao->target[1]); break;
        case CMP_LT: printf("cmp_LT %s, %s -> %s\n", operacao->parameters[0], operacao->parameters[1], operacao->target[0]); break;
        case CMP_LE: printf("cmp_LE %s, %s -> %s\n", operacao->parameters[0], operacao->parameters[1], operacao->target[0]); break;
        case CMP_EQ: printf("cmp_EQ %s, %s -> %s\n", operacao->parameters[0], operacao->parameters[1], operacao->target[0]); break;
        case CMP_GE: printf("cmp_GE %s, %s -> %s\n", operacao->parameters[0], operacao->parameters[1], operacao->target[0]); break;
        case CMP_GT: printf("cmp_GT %s, %s -> %s\n", operacao->parameters[0], operacao->parameters[1], operacao->target[0]); break;
        case CMP_NE: printf("cmp_NE %s, %s -> %s\n", operacao->parameters[0], operacao->parameters[1], operacao->target[0]); break;
        default: break;
    }
}

PROGRAM *Create_Operation(OPERATION *op)
{
    PROGRAM *New_Operation;

    New_Operation=(PROGRAM*)malloc(sizeof(PROGRAM));
    New_Operation->operation=op;
    New_Operation->next=NULL;
    New_Operation->previous=NULL;
    New_Operation->Is_Leader=0;

    return(New_Operation);
}

PROGRAM *Find_First(PROGRAM *op_list)
{
    PROGRAM *current=op_list;

    if(current==NULL)
    {
        return(current);
    }

    while(current->previous!=NULL)
    {
        current=current->previous;
    }

    return(current);
}

PROGRAM *Find_Last(PROGRAM *op_list)
{
    PROGRAM *current=op_list;

    if(current==NULL)
    {
        return(current);
    }

    while(current->next!=NULL)
    {
        current=current->next;
    }

    return(current);
}

PROGRAM *Concat_Iloc_Op_Lists(PROGRAM *first_part, PROGRAM *second_part)
{
    PROGRAM *primeira_op, *ultima_op;

    if(first_part==NULL)
    {
        return(second_part);
    }

    if(second_part==NULL)
    {   
        return(first_part);
    }
 
    primeira_op=Find_First(second_part);
    ultima_op=Find_Last(first_part);
    ultima_op->next=primeira_op;
    primeira_op->previous=ultima_op;

    return(first_part);
}


PROGRAM *Append_Op(PROGRAM *first_op, PROGRAM *second_op)
{

    if(first_op==NULL)
    {
        return(second_op);
    }

    if(second_op==NULL)
    {
        return(first_op);
    }

    first_op->next=second_op;
    second_op->previous=first_op;

    return(first_op);
}


PROGRAM *Load_Var(int Deslocamento, ESCOPO escopo_var, char *nome_var)
{
    const char *registers[]={"rbss", "rfp"};
    OPERATION *operacao;
    operacao=(OPERATION*)malloc(sizeof(OPERATION));

    operacao->instruction=LOADAI; 

    char *imediato;
    
    if(strcmp(registers[escopo_var], "rbss")==0)
    {
        imediato=strdup(nome_var);
    }
    else
    {
        imediato=(char*)malloc(sizeof(char)*(BUFFER_SIZE+1));
        sprintf(imediato, "-%d", (Deslocamento+4));
    }

    operacao->parameters[0]=strdup(registers[escopo_var]);
    operacao->parameters[1]=imediato;
    operacao->target[0]=Cria_Temporario();

    return(Create_Operation(operacao));
}

PROGRAM *Load_Literal(char *literal)
{
    OPERATION *operacao;
    operacao=(OPERATION*)malloc(sizeof(OPERATION));

    operacao->instruction=LOADI;

    operacao->parameters[0]=strdup(literal);
    operacao->target[0]=Cria_Temporario();

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
    const int iloc_operation[]={ ADD ,SUB, MULT, DIV, OR, AND, CMP_LT, CMP_LE, CMP_GT, CMP_GE, CMP_EQ, CMP_NE}; 

    OPERATION *operacao;
    PROGRAM *link_operacao;
    operacao=(OPERATION*)malloc(sizeof(OPERATION));

    operacao->instruction=iloc_operation[Return_OP(operation_type)];

    operacao->parameters[0]=strdup(operand1->operation->target[0]);
    operacao->parameters[1]=strdup(operand2->operation->target[0]);

    Libera_Reg(operacao->parameters[0]);
    Libera_Reg(operacao->parameters[1]);

    operacao->target[0]=Cria_Temporario();

    link_operacao=Create_Operation(operacao);

    //Coloca a nova operação após a última operação do operando 2
    operand2=Append_Op(operand2, link_operacao);

    //Coloca a primeira operação do operando 2 após a última operação do operando 1 
    operand1=Append_Op(operand1, Find_First(operand2));

    return(link_operacao);
}

PROGRAM *Atribution(PROGRAM *expression, int Deslocamento, ESCOPO escopo_var, char *nome_var)
{
    const char *registers[]={"rbss", "rfp"};
    OPERATION *operacao;
    PROGRAM *link_operacao;
    operacao=(OPERATION*)malloc(sizeof(OPERATION));

    char *imediato;
    
    if(strcmp(registers[escopo_var], "rbss")==0)
    {
        imediato=strdup(nome_var);
    }
    else
    {
        imediato=(char*)malloc(sizeof(char)*(BUFFER_SIZE+1));
        sprintf(imediato, "-%d", (Deslocamento+4));
    }

    operacao->instruction=STOREAI;

    operacao->parameters[0]=strdup(expression->operation->target[0]);
    operacao->target[0]=strdup(registers[escopo_var]);
    operacao->target[1]=imediato;
    Libera_Reg(operacao->parameters[0]);
    link_operacao=Create_Operation(operacao);

    expression=Append_Op(expression, link_operacao);

    return(link_operacao);
}

PROGRAM *Generate_Label_Comand()
{
    OPERATION *label_comand;

    label_comand=(OPERATION*)malloc(sizeof(OPERATION));
    label_comand->instruction=NOP;
    label_comand->target[0]=strdup(Cria_Label());

    return(Create_Operation(label_comand));
}

PROGRAM *Generate_Jmp_Comand(char *jump_location)
{
    OPERATION *jmp_comand;

    jmp_comand=(OPERATION*)malloc(sizeof(OPERATION));
    jmp_comand->instruction=JUMPI;
    jmp_comand->target[0]=strdup(jump_location);

    return(Create_Operation(jmp_comand));
}


PROGRAM *Logical_Not(PROGRAM *operand)
{
    //Podemos dizer que o not lógico de um valor é 
    //O mesmo que perguntar se esse valor é zero
    //Ou seja, carregamos 0 para um registrador
    //E fazemos a comparação de igualdade com o operando
    PROGRAM *link_operacao_not, *link_operacao_load;
    
    link_operacao_load=Load_Literal("0");

    link_operacao_not=Binary_Operation("==", operand, link_operacao_load);

    return(link_operacao_not);
}

PROGRAM *Arithmetic_Inversion(PROGRAM *operand)
{
    OPERATION *operacao;
    PROGRAM *link_operacao;
    operacao=(OPERATION*)malloc(sizeof(OPERATION));

    operacao->instruction=RSUBI;

    operacao->parameters[0]=strdup(operand->operation->target[0]);
    operacao->parameters[1]=strdup("0");

    Libera_Reg(operacao->parameters[0]);

    operacao->target[0]=Cria_Temporario();

    link_operacao=Create_Operation(operacao);

    //Coloca a nova operação após a última operação do operando
    operand=Append_Op(operand, link_operacao);

    return(link_operacao);
}

PROGRAM *Unary_Operation(char *operation_type, PROGRAM *operand)
{
    PROGRAM *link_operacao;

    if(strcmp(operation_type, "!")==0)
    {   
        link_operacao=Logical_Not(operand);
    }
    else
    {   
        link_operacao=Arithmetic_Inversion(operand);
    }

    return(link_operacao);
}



PROGRAM *Conditional_Flux(PROGRAM *expression, PROGRAM *command_block, PROGRAM *else_block)
{
    OPERATION *operacao;
    PROGRAM *if_label=Generate_Label_Comand(), *else_label=Generate_Label_Comand(), *escape_label, *link_operacao, *operacao_jmp;

    operacao=(OPERATION*)malloc(sizeof(OPERATION));

    operacao->instruction=CBR;

    operacao->parameters[0]=strdup(expression->operation->target[0]);
    operacao->target[0]=strdup(if_label->operation->target[0]);
    operacao->target[1]=strdup(else_label->operation->target[0]);
    link_operacao=Create_Operation(operacao);

    Libera_Reg(operacao->parameters[0]);
    //Colocar a expressão de branch após a última operação da expressão
    expression=Append_Op(expression, link_operacao);

    //Colocar o label após a expressão de branch criada
    link_operacao=Append_Op(link_operacao, if_label);

    //Colocar o if label antes do primeiro comando do bloco de comandos
    if_label=Append_Op(if_label, Find_First(command_block));

    //Colocar o else label antes do primeiro comando do bloco de comando do else
    else_label=Append_Op(else_label, Find_First(else_block));

    //Se houver um bloco de else
    if(else_block!=NULL)
    {   
        //Criar um label para o quê vêm após o fim do bloco else
        escape_label=Generate_Label_Comand();

        //Colocar o label após o último comando do else
        else_block=Append_Op(Find_Last(else_block), escape_label);

        operacao_jmp=Generate_Jmp_Comand(escape_label->operation->target[0]);
        //Colocar um comando de jump após o último comando do bloco do if
        command_block=Append_Op(Find_Last(command_block), operacao_jmp);

        //Colocar o label do else logo após a operação de jump
        operacao_jmp=Append_Op(operacao_jmp, else_label);
    }
    else 
    { //Senão
        //colocar o label de else após o último comando do bloco de comandos
        //(Nesse caso, o label que vêm após o else é usado como o label de escape do bloco condicional)
        command_block=Append_Op(Find_Last(command_block), else_label);
    }

    //Print_Program(expression);

    return(link_operacao);
}

PROGRAM *Iterative_Flux(PROGRAM *expression, PROGRAM *command_block)
{
    OPERATION *operacao;
    PROGRAM *condition_label=Generate_Label_Comand(), *loop_label=Generate_Label_Comand(), *escape_label=Generate_Label_Comand(), *link_operacao, *operacao_jmp=Generate_Jmp_Comand(condition_label->operation->target[0]);

    operacao=(OPERATION*)malloc(sizeof(OPERATION));
    operacao->instruction=CBR;
    operacao->parameters[0]=strdup(expression->operation->target[0]);
    operacao->target[0]=strdup(loop_label->operation->target[0]);
    operacao->target[1]=strdup(escape_label->operation->target[0]);
    link_operacao=Create_Operation(operacao);
    
    //Libera_Reg(operacao->parameters[0]);
    //Coloca o label da expressão de condição antes da primeira operação da expressão 
    condition_label=Append_Op(condition_label, Find_First(expression));

    //Coloca a operação de branch após a última operação da expressão
    expression=Append_Op(expression, link_operacao);

    //Coloca o label do loop após a operação de branch
    link_operacao=Append_Op(link_operacao, loop_label);

    //Coloca o label do loop antes da primeira expressão do bloco de comandos
    loop_label=Append_Op(loop_label, Find_First(command_block));

    //Coloca a instrução de jump após o último comando do bloco de comandos
    command_block=Append_Op(Find_Last(command_block), operacao_jmp);

    //Coloca o label para escape do loop após o comando de jmp
    operacao_jmp=Append_Op(operacao_jmp, escape_label);

    //Print_Program(escape_label);

    return(link_operacao);

}

PROGRAM *Return(PROGRAM *operand)
{
    OPERATION *operacao;
    PROGRAM *link_operacao;

    if(operand==NULL)
    {
        return(operand);
    }

    operacao=(OPERATION*)malloc(sizeof(OPERATION));
    operacao->instruction=RETURN;
    operacao->parameters[0]=strdup(operand->operation->target[0]);

    Libera_Reg(operand->operation->target[0]);

    link_operacao=Create_Operation(operacao);

    Append_Op(operand, link_operacao);

    return(link_operacao);
}


void Print_Program(PROGRAM *programa)
{
    PROGRAM *current=Find_First(programa);
    
    while(current!=NULL)
    {
        Escreve_Codigo(current->operation);
        current=current->next;
    }
    printf("\n");
}

char *Float_Int_Conversion(char *float_represented)
{   
    char *Float_Converted;
    char *token;

    token=strdup(float_represented);
    float_represented=strtok(float_represented, ".");

    if(strtok(NULL, ".")==NULL)
    {
        Float_Converted=strdup("0");
    }
    else
    {
        Float_Converted=strdup(float_represented);
    }

    return(Float_Converted);
}