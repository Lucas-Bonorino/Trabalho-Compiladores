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
    NODOAST *filhos[];
};

TOKENDATA tokendata(char* token, int line_number, TOKEN_TYPE type);