//Por Lucas dos Santos Bonorino e Rafael Lacerda Busatta
#include "Tabela_de_Simbolos.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

REGISTRO_SIMBOLO *Criar_Registro(char *valor, TOKEN_NATURE natureza_token, DATA_TYPE tipo_do_token,int linha)
{
    REGISTRO_SIMBOLO *registro;
    registro=(REGISTRO_SIMBOLO*)malloc(sizeof(REGISTRO_SIMBOLO));
    registro->Numero_Linha=linha;
    registro->natureza_token=natureza_token;
    registro->tipo_simbolo=tipo_do_token;
    registro->token=strdup(valor);
    return(registro);
}

void Adicionar_Declaracao_de_Identificador(REGISTRO_SIMBOLO *registro, PILHA *tabelas)
{
    PILHA *tabela_atual;
    int linha;

    while(tabela_atual!=NULL)
    {
        //Se o identificado já tiver sido declarado
        if(linha=Ja_Declarado(registro->token, tabela_atual->tabela))
        {
            //Imprime mensagem de erro
            printf("Erro: redeclaracao do identificador %s, na linha: %d, originalmente declarado na linha: %d", registro->token, registro-> Numero_Linha, linha);
            exit (ERR_DECLARED);
        }

        tabela_atual=tabela_atual->proxima;
    }

    Adiciona_Registro(registro, tabelas->tabela);
}

void Adicionar_Uso_de_Identificador(REGISTRO_SIMBOLO *registro, TABELA_DE_SIMBOLOS *tabela)
{
    PILHA *tabela_atual;

    while(tabela_atual!=NULL)
    {   
        //Verifica se identificador já foi declarado
        if(Ja_Declarado(registro->token, tabela_atual->tabela)) 
        {
            //Se sim, adiciona um registro para o seu uso em uma linha e retorna
            Adiciona_Registro(registro, tabela_atual->tabela);
            return;
        }
        tabela_atual=tabela_atual->proxima;
    }

    //Senão informa erro
    printf("Erro: identificador %s, na linha: %d, usado mas nao declarado(primeira referencia no escopo analisado)", registro->token, registro-> Numero_Linha);
    exit (ERR_UNDECLARED);
}

TABELA_DE_SIMBOLOS *Cria_Tabela()
{
    TABELA_DE_SIMBOLOS *tabela;

    
    return(NULL);
}

void Destroi_Tabela(TABELA_DE_SIMBOLOS *tabela)
{

}

PILHA *Cria_Pilha()
{
    PILHA *pilha;
    pilha=(PILHA*)malloc(sizeof(PILHA));
    pilha->proxima=NULL;
    pilha->tabela=NULL;
    return(pilha);
}

PILHA *Empilha_Tabela(PILHA *pilha, TABELA_DE_SIMBOLOS *tabela)
{
    PILHA *novo_topo;
    novo_topo=Cria_Pilha();
    novo_topo->tabela=tabela;
    novo_topo->proxima=pilha;
    return(novo_topo);
}

PILHA *Desempilha_Tabela(PILHA *pilha)
{
    PILHA *novo_topo=pilha->proxima;
    Destroi_Tabela(pilha->tabela);
    return(novo_topo);
}


