//Por Lucas dos Santos Bonorino e Rafael Lacerda Busatta
#include "Estruturas.h"

//Faz a inferência de tipos de um dado(ainda será movido daqui)
DATA_TYPE inferencia_de_tipo_expressao(DATA_TYPE tipo_dado_1, DATA_TYPE tipo_dado_2);

//Verifica na tabela de símbolos se um identificador já foi declarado
REGISTRO_SIMBOLO *Ja_Declarado(CHAVE buscado, TABELA_DE_SIMBOLOS *tabela);

//Adiciona um registro a tabela de símbolos
void Adiciona_Registro(REGISTRO_SIMBOLO *registro, TABELA_DE_SIMBOLOS *tabela);

//Cria uma tabela de símbolos
TABELA_DE_SIMBOLOS *Cria_Tabela();

//Destrói uma tabela de símbolos
void Destroi_Tabela(TABELA_DE_SIMBOLOS *tabela);
