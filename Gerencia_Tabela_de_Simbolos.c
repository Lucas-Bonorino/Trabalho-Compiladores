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
    PILHA *tabela_atual=tabelas;
    REGISTRO_SIMBOLO *registro_tabela;

    while(tabela_atual!=NULL)
    {
        //Se o identificado já tiver sido declarado
        if(registro_tabela=Ja_Declarado(registro->token, tabela_atual->tabela))
        {
            //Imprime mensagem de erro
            printf("Erro: redeclaracao do identificador \"%s\", na linha: %d, originalmente declarado na linha: %d\nexit code 11: ERR_DECLARED\n", registro->token, registro->Numero_Linha, registro_tabela->Numero_Linha);
            exit (ERR_DECLARED);
        }
        tabela_atual=tabela_atual->proxima;
    }

    Adiciona_Registro(registro, tabelas->tabela);
}

void Mensagem_Erro_Semantica_Identificador(REGISTRO_SIMBOLO *registro_uso,REGISTRO_SIMBOLO *registro_tabela)
{
    if(registro_tabela->natureza_token==VARIAVEL)
    {
        printf("Identificador \"%s\" usado como funcao na linha %d, declarado como variavel na linha %d\nexit code 20: ERR_VARIABLE\n", registro_uso->token, registro_uso->Numero_Linha, registro_tabela->Numero_Linha);
    }
    else
    {
        printf("Identificador \"%s\" usado como variavel na linha %d, declarado como funcao na linha %d\nexit code 21: ERR_FUNCTION\n", registro_uso->token, registro_uso->Numero_Linha, registro_tabela->Numero_Linha);
    }
}

void Uso_de_Identificador(REGISTRO_SIMBOLO *registro, PILHA *tabelas)
{
    PILHA *tabela_atual=tabelas;
    REGISTRO_SIMBOLO *registro_tabela;

    while(tabela_atual!=NULL)
    {   
        //Verifica se identificador já foi declarado
        if(registro_tabela=Ja_Declarado(registro->token, tabela_atual->tabela)) 
        {   
            //Se foi, verifica se a natureza dele(identificador ou função)
            //Está de acordo com a natureza em sua declaração
            if(registro->natureza_token!=registro_tabela->natureza_token)
            {   
                //Senão for, da uma mensagem de erro
                Mensagem_Erro_Semantica_Identificador(registro, registro_tabela);
                //E sai com o código de erro 
                exit(registro_tabela->natureza_token);
            }

            return;
        }
        tabela_atual=tabela_atual->proxima;
    }

    //Senão informa erro
    printf("Erro: identificador %s, na linha: %d, usado mas nao declarado(primeira referencia no escopo analisado)\nexit code 10: ERR_UNDECLARED\n", registro->token, registro-> Numero_Linha);
    exit (ERR_UNDECLARED);
}

PILHA *Cria_Pilha()
{
    PILHA *pilha;
    pilha=(PILHA*)malloc(sizeof(PILHA));
    pilha->proxima=NULL;
    pilha->tabela=NULL;
    return(pilha);
}

PILHA *Empilha_Tabela(PILHA *pilha)
{
    PILHA *novo_topo;
    TABELA_DE_SIMBOLOS *tabela;

    tabela=Cria_Tabela();
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

void Cria_e_Adiciona_Registro_Variavel(char *valor, TOKEN_NATURE natureza_token, DATA_TYPE tipo_do_token,int linha, PILHA *pilha)
{
    REGISTRO_SIMBOLO *registro=Criar_Registro(valor, natureza_token, tipo_do_token, linha);
    Adicionar_Declaracao_de_Identificador(registro, pilha);
}

void Cria_e_Adiciona_Registro_Funcao(char *valor, TOKEN_NATURE natureza_token, DATA_TYPE tipo_do_token,int linha, PILHA *pilha)
{
    REGISTRO_SIMBOLO *registro=Criar_Registro(valor, natureza_token, tipo_do_token, linha);
    PILHA *atual=pilha->proxima;
    Adicionar_Declaracao_de_Identificador(registro, atual);
}

void Verifica_Uso(char *valor, TOKEN_NATURE natureza_token, DATA_TYPE tipo_do_token,int linha, PILHA *pilha)
{
    REGISTRO_SIMBOLO *registro=Criar_Registro(valor, natureza_token, tipo_do_token, linha);

    Uso_de_Identificador(registro, pilha);
}

void Imprime_Tabela(PILHA *tabelas)
{   
    TABELA_DE_SIMBOLOS *atual=tabelas->tabela;
    
    while(atual!=NULL)
    {
        if(atual->registro!=NULL)
        {
            REGISTRO_SIMBOLO *reg=atual->registro;
            printf("Nome: %s\tNatureza: %d\tTipo: %d\tLinha: %d\n", reg->token, reg->natureza_token, reg->tipo_simbolo, reg->Numero_Linha);
        }

        atual=atual->prox;
    }
    printf("\n");
}