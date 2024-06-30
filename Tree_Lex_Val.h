//Por Lucas dos Santos Bonorino e Rafael Lacerda Busatta
#include "Gerencia_Tabela_de_Simbolos.h"
//Função para criação da estrutura dos dados dos tokens
TOKENDATA* tokendata(char* token, int line_number, TOKEN_NATURE type);

//Função para criar as folhas da árvore
NODOAST* Cria_folha(char* valor);

//Função para criar os nodos que possuam filhos na árvore
NODOAST* Cria_nodo(char* valor, NODOAST *filho1, NODOAST *filho2);

//Função para adicionar o nodo seguinte em listas(como listas de comando, função, expressão, etc)
NODOAST* Adiciona_Seguinte(NODOAST *pai, NODOAST *seguinte);

//Função auxiliar para concatenação de strings
char* StringCat(char StringBegin[], char StringEnd[]);

//Função para adicionar um nodo filho a um nodo pai
NODOAST* Adiciona_filho(NODOAST *pai, NODOAST *filho);