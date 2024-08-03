#include "Gerador_Assembly.h"
#include <stdio.h>

void Print_Static_Var(char *var_name)
{
    printf("\n\t.globl %s\n\t.data\n\t.align 4\n\t.type %s, @object\n\t.size %s, 4\n%s:\n\t.zero 4\n", var_name, var_name, var_name, var_name);
}

void Print_Function_Decl(char *fun_name)
{
    printf("\t.text\n\t.globl %s\n\t.type %s, @function\n%s:\n", fun_name, fun_name, fun_name);
}

void Print_Static(REGISTRO_SIMBOLO *registro)
{
    switch(registro->natureza_token)
    {
        case VARIAVEL: Print_Static_Var(registro->token); break;
        case FUNCAO: Print_Function_Decl(registro->token);break;
    }
}

void Area_Estatica(TABELA_DE_SIMBOLOS *tabela)
{   
    TABELA_DE_SIMBOLOS *Entrada_Atual=tabela;
    
    while(Entrada_Atual!=NULL)
    {
        Print_Static(Entrada_Atual->registro);
        Entrada_Atual=Entrada_Atual->prox;
    }
}

void Instruction_Print(PROGRAM *op, int *Op_type)
{
}