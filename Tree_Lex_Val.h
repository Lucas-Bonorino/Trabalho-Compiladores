//Por Lucas dos Santos Bonorino e Rafael Lacerda Busatta
typedef struct Dados_Token TOKENDATA;

//Tipo enum para dizer se token é literal ou identificador
typedef enum { IDENTIFICADOR, LITERAL } TOKEN_TYPE;

//Estrutura para armazenar os dados do token
struct Dados_Token
{
    //Número da linha em que aparece
    int Numero_Linha;

    //Tipo de token
    TOKEN_TYPE token_type;

    //Valor do lexema
    char *token;
};

typedef struct Nodo_Arvore NODOAST;

struct Nodo_Arvore
{
    //String com o nome do nodo
    char *nome;

    //Número de filhos que o nodo possui
    int numero_filhos;

    //Armazena os filhos que estão abaixo do nodo da árvore
    NODOAST **filhos;

    //Serve para armazenar o nodo seguinte em listas(como a função, expressão, etc seguinte)
    NODOAST  *seguinte;

    //Descritor para qual o última item da sequência(ultima função, expressão, etc)
    NODOAST  *ultimo;
};

//Função para criação da estrutura dos dados dos tokens
TOKENDATA* tokendata(char* token, int line_number, TOKEN_TYPE type);

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