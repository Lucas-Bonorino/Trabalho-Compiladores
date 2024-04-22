#include "Tree_Lex_Val.h"
#include<string.h>

TOKENDATA tokendata(char* token, int line_number, TOKEN_TYPE type)
{
    TOKENDATA valor_lexico;
    valor_lexico.token=strdup(token);
    valor_lexico.Numero_Linha=line_number;
    valor_lexico.token_type=type;

    return(valor_lexico);
}