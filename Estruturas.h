//Por Lucas dos Santos Bonorino e Rafael Lacerda Busatta
#define ERR_UNDECLARED 10 //2.2
#define ERR_DECLARED 11 //2.2
#define ERR_VARIABLE 20 //2.3
#define ERR_FUNCTION 21 //2.3

typedef struct Dados_Token TOKENDATA;
typedef struct Nodo_Arvore NODOAST;
typedef struct Entrada_da_Tabela_de_Simbolos REGISTRO_SIMBOLO;
typedef struct Pilha_de_Tabelas PILHA;

//Typedef para que possamos fazer uma interface com a estrutura da tabela de símbolos
typedef struct Lista_de_Simbolos TABELA_DE_SIMBOLOS;
//Typedef para que possamos fazer uma interface com a chave única usada para busca 
//na tabela de símbolos
typedef char * CHAVE;

//Tipo enum para dizer se token é literal , identificador ou função
typedef enum { IDENTIFICADOR, LITERAL, FUNCAO=ERR_FUNCTION, VARIAVEL=ERR_VARIABLE } TOKEN_NATURE;
//Tipo enum para dizer qual o tipo do dado
typedef enum { BOOL=0, INT=1, FLOAT=3, UNKNOWN=2 } DATA_TYPE;

//Estrutura para armazenar os dados do token
struct Dados_Token
{
    //Número da linha em que aparece
    int Numero_Linha;

    //Natureza do token
    TOKEN_NATURE natureza_token;

    //Valor do lexema
    char *token;
};

struct Nodo_Arvore
{
    //String com o nome do nodo
    char *nome;

    //Contém o tipo de dados do nodo
    DATA_TYPE tipo_do_nodo;

    //Número de filhos que o nodo possui
    int numero_filhos;

    //Armazena os filhos que estão abaixo do nodo da árvore
    NODOAST **filhos;

    //Serve para armazenar o nodo seguinte em listas(como a função, expressão, etc seguinte)
    NODOAST  *seguinte;

    //Descritor para qual o última item da sequência(ultima função, expressão, etc)
    NODOAST  *ultimo;
};

//Estrutura de uma entrada na tabela de símbolos
struct Entrada_da_Tabela_de_Simbolos
{
    //Número da linha em que símbolo aparece pela primeira vez
    int Numero_Linha;

    //Natureza do símbolo
    TOKEN_NATURE natureza_token;

    //Tipo do símbolo
    DATA_TYPE tipo_simbolo;

    //Valor do lexema
    CHAVE token;
};

//Estrutura para a pilha de tabelas de símbolos
struct Pilha_de_Tabelas
{
    TABELA_DE_SIMBOLOS *tabela;
    PILHA *proxima;
};

//Estrutura para tabela de simbolos
struct Lista_de_Simbolos
{
    REGISTRO_SIMBOLO *registro;

    TABELA_DE_SIMBOLOS *prox;

    TABELA_DE_SIMBOLOS *ultimo;
};