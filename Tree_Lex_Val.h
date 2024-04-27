typedef struct Dados_Token TOKENDATA;

typedef enum { IDENTIFICADOR, LITERAL } TOKEN_TYPE;

struct Dados_Token
{
    int Numero_Linha;
    TOKEN_TYPE token_type;
    char *token;
};

typedef struct Nodo_Arvore NODOAST;

struct Nodo_Arvore
{
    char *nome;
    int numero_filhos;
    NODOAST **filhos;
};

TOKENDATA* tokendata(char* token, int line_number, TOKEN_TYPE type);

NODOAST* Cria_folha(char* valor);

NODOAST* Cria_nodo(char* valor, NODOAST *filho1, NODOAST *filho2);

char* StringCat(char StringBegin[], char StringEnd[]);

NODOAST* Adiciona_filho(NODOAST *pai, NODOAST *filho);