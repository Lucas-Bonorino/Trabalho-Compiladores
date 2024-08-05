#include "Gerador_Assembly.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *Get_Reg_Num(char *reg)
{
    char *buff=reg+1;

    int index=atoi(buff);

    const char *registers[]={"eax", "ecx", "edx", "ebx", "esi", "edi", "r8d", "r9d", "r10d",  "r11d",  "r12d",  "r13d",  "r14d",  "r15d", "esp", "ebp"};

    return(registers[index]);
}

const char *Get_Var_Reg(char *reg)
{
    if(strcmp(reg, "rfp")==0)
    {
        return("rbp");
    }
    else
    {
        return("rip");
    }
}

void Divide_Op(OPERATION *operacao)
{   
    char *buf=operacao->parameters[0]+1;
    int reg=atoi(buf);


    if(reg!=2)
    {
        printf("\tpushq %%rdx\n");
    }

    if(reg!=0)
    {
        printf("\tpushq %%rax\n\tmovl %%%s, %%eax\n", Get_Reg_Num(operacao->parameters[0]));
    }

    printf("\tcltd\n\tidivl %%%s\n",Get_Reg_Num(operacao->parameters[1]));

    if(reg!=0)
    {   
        printf("\tmovl %%eax, %%%s\n\tpopq %%rax\n", Get_Reg_Num(operacao->target[0]));
    }

    if(reg!=2)
    {
        printf("\tpopq %%rdx\n");
    }
}

void Return_Op(OPERATION *operacao)
{
    char *buf=operacao->parameters[0]+1;
    int reg=atoi(buf);

    if(reg!=0)
    {
        printf("\tmovl %%%s, %%eax\n", operacao->parameters[0]);
    }

    printf("\tpopq %%rbp\n\tret\n");
}

void Print_Global_Var(REGISTRO_SIMBOLO *registro, int first_var_in_section)
{
    printf("\t.globl %s\n",registro->token);

    if(first_var_in_section)
    {
        printf("\t.bss\n");
    }

    printf("\t.align 4\n\t.type %s, @object\n\t.size %s, 4\n%s:\n\t.zero 4\n", registro->token, registro->token, registro->token);
}

void Print_Function_Header(REGISTRO_SIMBOLO *registro)
{
    printf("\t.text\n\t.globl %s\n\t.type %s, @function\n%s:\n\tpushq %%rbp\n\tmovq %%rsp, %%rbp\n", registro->token, registro->token, registro->token);
}

void Print_Operation(OPERATION *operacao)
{
    switch(operacao->instruction)
    {
        case NOP:    printf("%s:\n", operacao->target[0]);break;
        case ADD:    printf("\taddl %%%s, %%%s\n", Get_Reg_Num(operacao->parameters[1]), Get_Reg_Num(operacao->parameters[0]));break;
        case SUB:    printf("\tsubl %%%s, %%%s\n", Get_Reg_Num(operacao->parameters[1]), Get_Reg_Num(operacao->parameters[0]));break;
        case RSUBI:  printf("\tneg %%%s\n", Get_Reg_Num(operacao->parameters[0]));break;
        case MULT:   printf("\timull %%%s, %%%s\n", Get_Reg_Num(operacao->parameters[1]), Get_Reg_Num(operacao->parameters[0]));break;
        case DIV:    Divide_Op(operacao);break;
        case OR:     printf("\torl %%%s, %%%s\n", Get_Reg_Num(operacao->parameters[1]), Get_Reg_Num(operacao->parameters[0]));break;
        case AND:    printf("\tandl %%%s, %%%s\n",Get_Reg_Num(operacao->parameters[1]), Get_Reg_Num(operacao->parameters[0]));break;
        case LOADI:  printf("\tmovl $%s, %%%s\n", operacao->parameters[0], Get_Reg_Num(operacao->target[0]));break;
        case LOADAI: printf("\tmovl -%d(%%%s), %%%s\n", (4+atoi(operacao->parameters[1])), Get_Var_Reg(operacao->parameters[0]), Get_Reg_Num(operacao->target[0]));break;
        case STOREAI:printf("\tmovl %%%s, -%d(%%%s)\n",  Get_Reg_Num(operacao->parameters[0]), (4+atoi(operacao->target[1])), Get_Var_Reg(operacao->target[0]));break;
        case JUMPI:  printf("\tjmp %s\n", operacao->target[0]);break;
        case CBR:    printf("\tcmp %%%s, $0\n\tjnz %s\n\tjmp %s\n", Get_Reg_Num(operacao->parameters[0]), operacao->target[0], operacao->target[1]);break;
        case CMP_LT: printf("\tcmp %%%s, %%%s\n\tmovl $1, %%%s\n\tmovge $0, %%%s\n", Get_Reg_Num(operacao->parameters[1]), Get_Reg_Num(operacao->parameters[0]), Get_Reg_Num(operacao->target[0]), Get_Reg_Num(operacao->target[0]));break;
        case CMP_LE: printf("\tcmp %%%s, %%%s\n\tmovle $1, %%%s\n\tmovg $0, %%%s\n", Get_Reg_Num(operacao->parameters[1]), Get_Reg_Num(operacao->parameters[0]), Get_Reg_Num(operacao->target[0]), Get_Reg_Num(operacao->target[0]));break;
        case CMP_EQ: printf("\tcmp %%%s, %%%s\n\tmove $1, %%%s\n\tmovne $0, %%%s\n", Get_Reg_Num(operacao->parameters[1]), Get_Reg_Num(operacao->parameters[0]), Get_Reg_Num(operacao->target[0]), Get_Reg_Num(operacao->target[0]));break;
        case CMP_GE: printf("\tcmp %%%s, %%%s\n\tmovge $1, %%%s\n\tmovl $0, %%%s\n", Get_Reg_Num(operacao->parameters[1]), Get_Reg_Num(operacao->parameters[0]), Get_Reg_Num(operacao->target[0]), Get_Reg_Num(operacao->target[0]));break;
        case CMP_GT: printf("\tcmp %%%s, %%%s\n\tmovg $1, %%%s\n\tmovle $0, %%%s\n", Get_Reg_Num(operacao->parameters[1]), Get_Reg_Num(operacao->parameters[0]), Get_Reg_Num(operacao->target[0]), Get_Reg_Num(operacao->target[0]));break;
        case CMP_NE: printf("\tcmp %%%s, %%%s\n\tmovne $1, %%%s\n\tmove $0, %%%s\n", Get_Reg_Num(operacao->parameters[1]), Get_Reg_Num(operacao->parameters[0]), Get_Reg_Num(operacao->target[0]), Get_Reg_Num(operacao->target[0]));break;
        case RETURN: Return_Op(operacao);break;
    }
}

PROGRAM *Gerar_Codigo_Funcao(PROGRAM *codigo)
{
    PROGRAM *comando_atual=codigo;

    while(comando_atual!=NULL)
    {
        Print_Operation(comando_atual->operation);

        if(comando_atual->operation->instruction==RETURN)
        {
            return(comando_atual->next);
        }

        comando_atual=comando_atual->next;
    }

    return(comando_atual);
}

void generateASM(void *tabela_global, void *raiz)
{
    PILHA *p=(PILHA*)tabela_global;
    TABELA_DE_SIMBOLOS *entrada_atual=p->tabela;
    NODOAST *arvore=(NODOAST*)raiz;
    PROGRAM *comando_corrente=Find_First(arvore->codigo);
    
    int first_var=1;
    printf("\t.text\n");

    while(entrada_atual!=NULL)
    {
        if(entrada_atual->registro->natureza_token==FUNCAO)
        {
            Print_Function_Header(entrada_atual->registro);
            comando_corrente=Gerar_Codigo_Funcao(comando_corrente);
            first_var=1;
        }
        else
        {
            Print_Global_Var(entrada_atual->registro, first_var);
            first_var=0;
        }
        
        entrada_atual=entrada_atual->prox;
    }
}
