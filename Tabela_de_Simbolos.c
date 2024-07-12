//Por Lucas dos Santos Bonorino e Rafael Lacerda Busatta
#include "Tabela_de_Simbolos.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//Realiza a inferência de tipos de uma expressão
//Isso pode ser feito com uma operação bitwise, considerando a maneira
//Como codificamos os tipos
DATA_TYPE inferencia_de_tipo_expressao(DATA_TYPE tipo_dado_1, DATA_TYPE tipo_dado_2)
{
    return(tipo_dado_1 | tipo_dado_2);
}

REGISTRO_SIMBOLO *Ja_Declarado(CHAVE buscado, TABELA_DE_SIMBOLOS *tabela)
{
    TABELA_DE_SIMBOLOS *atual=tabela;

    if(tabela->registro==NULL)
    {
        return(NULL);
    }

    while(atual!=NULL)
    {
        
        if(strcmp(atual->registro->token, buscado)==0)
        {
            return(atual->registro);
        }
        
        atual=atual->prox;
    }

    return(NULL);
}

TABELA_DE_SIMBOLOS *Cria_Tabela()
{
    TABELA_DE_SIMBOLOS *tabela;
    tabela=(TABELA_DE_SIMBOLOS*)malloc(sizeof(TABELA_DE_SIMBOLOS));
    tabela->registro=NULL;
    tabela->prox=NULL;
    tabela->ultimo=tabela;
    return(tabela);
}

void Destroi_Tabela(TABELA_DE_SIMBOLOS *tabela)
{
    TABELA_DE_SIMBOLOS *atual=tabela, *deletado=atual;
  
    while(atual!=NULL)
    {
        deletado=atual;
        atual=atual->prox;
        
        if(deletado->registro!=NULL)
        {   
            if(deletado->registro->token!=NULL)
            {
                free(deletado->registro->token);
            }

            free(deletado->registro);
        }
        
        free(deletado);    
    }
    
}

void Adiciona_Registro(REGISTRO_SIMBOLO *registro, TABELA_DE_SIMBOLOS *tabela)
{
    TABELA_DE_SIMBOLOS *atual=tabela->ultimo;

    if(atual->registro==NULL)
    {
        atual->registro=registro;
    }
    else
    {
        atual->prox=Cria_Tabela();
        tabela->ultimo=atual->prox;
        atual->prox->registro=registro;
    }

}