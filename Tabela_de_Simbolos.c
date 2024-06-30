//Por Lucas dos Santos Bonorino e Rafael Lacerda Busatta
#include "Tabela_de_Simbolos.h"

//Realiza a inferência de tipos de uma expressão
//Isso pode ser feito com uma operação bitwise, considerando a maneira
//Como codificamos os tipos
DATA_TYPE inferencia_de_tipo_expressao(DATA_TYPE tipo_dado_1, DATA_TYPE tipo_dado_2)
{
    return(tipo_dado_1 | tipo_dado_2);
}

int Ja_Declarado(CHAVE buscado, TABELA_DE_SIMBOLOS *tabela)
{

    return(0);
}

void Adiciona_Registro(REGISTRO_SIMBOLO *registro, TABELA_DE_SIMBOLOS *tabela)
{

}