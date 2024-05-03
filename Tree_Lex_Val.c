#include "Tree_Lex_Val.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

TOKENDATA* tokendata(char* token, int line_number, TOKEN_TYPE type)
{
    TOKENDATA* valor_lexico;
    valor_lexico=(TOKENDATA*)malloc(sizeof(TOKENDATA));
    valor_lexico->token=strdup(token);
    valor_lexico->Numero_Linha=line_number;
    valor_lexico->token_type=type;

    return(valor_lexico);
}


NODOAST* Cria_folha(char* valor)
{   
    NODOAST *folha;
    folha=(NODOAST*)malloc(sizeof(NODOAST));
    folha->nome=strdup(valor);
    folha->numero_filhos=0;
    folha->filhos=NULL;
    folha->seguinte=NULL;
    folha->ultimo=folha;

    return(folha);
}


NODOAST* Cria_nodo(char* valor, NODOAST *filho1, NODOAST *filho2)
{
    NODOAST *folha;
    folha=(NODOAST*)malloc(sizeof(NODOAST));
    folha->nome=strdup(valor);
    folha->numero_filhos=0;

    if(filho1!=NULL) folha->numero_filhos+=1;
    if(filho2!=NULL) folha->numero_filhos+=1;

    folha->filhos=(NODOAST **)malloc(folha->numero_filhos*sizeof(NODOAST*));

    int i=0;
    if(filho1!=NULL) {folha->filhos[i]=filho1; i+=1;}
    if(filho2!=NULL) {folha->filhos[i]=filho2;}

    folha->seguinte=NULL;
    folha->ultimo=folha;

    return(folha);
}

NODOAST* Adiciona_filho(NODOAST *pai, NODOAST *filho)
{
    
    if(filho==NULL) return(pai);
    if(pai==NULL && filho!=NULL) return(filho);
    if(pai==NULL)  return(pai);

    pai->numero_filhos+=1;

    pai->filhos=(NODOAST **)realloc(pai->filhos, pai->numero_filhos*sizeof(NODOAST*));
    pai->filhos[pai->numero_filhos-1]=filho;
 
    return(pai);
}

NODOAST* Adiciona_Seguinte(NODOAST *pai, NODOAST *seguinte)
{
    
    if(pai==NULL) return(seguinte);
  
    NODOAST *atual=pai->ultimo;
    while(atual->seguinte!=NULL)
    {
        atual=atual->ultimo;
    }
    
    pai->ultimo=seguinte;

    atual->seguinte=seguinte;

    return(pai);
}

char* StringCat(char StringBegin[], char StringEnd[])
{
    int lenEnd=strlen(StringEnd);
    int lenOg=strlen(StringBegin);
    int NewWordLen=lenEnd+lenOg+2;

    char *New_String=(char*)malloc((NewWordLen)*sizeof(char));
    strcpy(New_String, StringBegin);
    strcat(New_String, StringEnd);

    return(New_String);
}

void Imprime_Label(NODOAST* nodo)
{
    printf("%p [label=\"%s\"];\n", nodo, nodo->nome);
}

void Imprime_Filho(NODOAST* nodo, NODOAST* filho)
{
    printf("%p, %p\n", nodo, filho);
}

void Imprime(NODOAST *raiz)
{
    int max_filhos=raiz->numero_filhos;

    Imprime_Label(raiz);

    if(max_filhos!=0)
    {
        for(int i=0;i<max_filhos;i++)
        {
            NODOAST *filho=raiz->filhos[i];
            Imprime_Filho(raiz, filho);
            Imprime(filho);
        }
    }

    if(raiz->seguinte!=NULL)
    {
        Imprime_Filho(raiz,raiz->seguinte);
        Imprime(raiz->seguinte);
    }
}

void exporta (void *arvore)
{
    NODOAST *raiz=(NODOAST*)arvore;
    Imprime(raiz);

}