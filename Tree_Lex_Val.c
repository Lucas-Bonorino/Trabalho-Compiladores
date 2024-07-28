//Por Lucas dos Santos Bonorino e Rafael Lacerda Busatta
#include "Tree_Lex_Val.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//Função para criação da estrutura dos dados dos tokens
TOKENDATA* tokendata(char* token, int line_number, TOKEN_NATURE type)
{
    TOKENDATA* valor_lexico;
    valor_lexico=(TOKENDATA*)malloc(sizeof(TOKENDATA));
    valor_lexico->token=strdup(token);
    valor_lexico->Numero_Linha=line_number;
    valor_lexico->natureza_token=type;

    return(valor_lexico);
}

//Função para criar as folhas da árvore
NODOAST* Cria_folha(char* valor, DATA_TYPE tipo)
{   
    NODOAST *folha;
    folha=(NODOAST*)malloc(sizeof(NODOAST));
    folha->nome=strdup(valor);
    folha->numero_filhos=0;
    folha->filhos=NULL;
    folha->seguinte=NULL;
    folha->ultimo=folha;
    folha->tipo_do_nodo=tipo;

    return(folha);
}

//Função para criar os nodos que possuam filhos na árvore
NODOAST* Cria_nodo(char* valor, NODOAST *filho1, NODOAST *filho2, DATA_TYPE tipo)
{
    NODOAST *folha;
    folha=(NODOAST*)malloc(sizeof(NODOAST));
    folha->nome=strdup(valor);
    folha->numero_filhos=0;
    folha->tipo_do_nodo=tipo;

    //Trata do caso em que algum dos filhos é nulo
    //Só aloca espaço para nodos não nulos
    if(filho1!=NULL) folha->numero_filhos+=1;
    if(filho2!=NULL) folha->numero_filhos+=1;

    folha->filhos=(NODOAST **)malloc(folha->numero_filhos*sizeof(NODOAST*));

    int i=0;
    //Adiciona o filho
    if(filho1!=NULL) {folha->filhos[i]=filho1; i+=1;}
    if(filho2!=NULL) {folha->filhos[i]=filho2;}

    folha->seguinte=NULL;
    //Serve para determinar o nodo como sendo o último de uma lista
    //Caso seja necessário
    folha->ultimo=folha;
    
    return(folha);
}

NODOAST* Adiciona_filho(NODOAST *pai, NODOAST *filho)
{
    //Trata do caso em que algum dos dois(pai ou filho) é nulo
    //Serve principalmente para edge cases em que o pai pode ser nulo
    if(filho==NULL) return(pai);
    if(pai==NULL && filho!=NULL) return(filho);

    pai->numero_filhos+=1;

    //Realoca o espaço de memória para os filhos e adiciona o novo filho
    pai->filhos=(NODOAST **)realloc(pai->filhos, pai->numero_filhos*sizeof(NODOAST*));
    pai->filhos[pai->numero_filhos-1]=filho;
 
    return(pai);
}

NODOAST* Adiciona_Seguinte(NODOAST *pai, NODOAST *seguinte)
{
    //Trata do edge case em que o pai é nulo
    //Como, por exemplo, se ele tivesse de ser
    //Uma declaração de variável
    if(pai==NULL) return(seguinte);
    if(seguinte==NULL) return(pai);
    //Busca o último nodo da lista atual(de expressões, funções).
    //A busca iterativa serve majoritariamente para casos de 
    //blocos de comando Encadeados
    NODOAST *atual=pai->ultimo;
    while(atual->seguinte!=NULL)
    {
        atual=atual->ultimo;
    }

    //Atualiza qual o último da lista do pai
    pai->ultimo=seguinte;

    //Atualiza qual o nodo seguinte do item atual
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

//Função recursiva para imprimir os nodos
void Imprime(NODOAST *raiz)
{
    int max_filhos=raiz->numero_filhos;
    //Primeiro imprime o label da raíz
    //Imprime_Label(raiz);

    if(raiz->codigo!=NULL)
    {
        Print_Program(raiz->codigo);
        printf("\n");
    }
    //Se houverem filhos
    if(max_filhos!=0)
    {   
        //Para cada filho
        for(int i=0;i<max_filhos;i++)
        {
            NODOAST *filho=raiz->filhos[i];

            //Imprime a relação hierárquica com o pai
            Imprime_Filho(raiz, filho);

            //Faz o passo recursivo com o filho
            Imprime(filho);
        }
    }

    //Se o nodo pertencer a uma lista
    if(raiz->seguinte!=NULL)
    {   
        //Imprime a relação hierárquica com o próximo
        //nodo da lista
        Imprime_Filho(raiz,raiz->seguinte);

        //Faz o passo recursivo com o nodo seguinte da lista
        Imprime(raiz->seguinte);
    }
}

//Função para exportar os nodos
void exporta (void *arvore)
{
    NODOAST *raiz=(NODOAST*)arvore;
    
    if(raiz!=NULL)
    {
        Imprime(raiz);
    }

}

void Adiciona_Codigo(NODOAST *nodo, PROGRAM *codigo)
{
    if(nodo==NULL)
    {
        return;
    }
    
    nodo->codigo=codigo;
}

PROGRAM *Get_Program_Conditional(NODOAST *nodo)
{
    if(nodo==NULL)
    {
        return(NULL);
    }

    return(nodo->codigo);
}