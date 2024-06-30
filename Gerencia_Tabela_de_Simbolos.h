//Por Lucas dos Santos Bonorino e Rafael Lacerda Busatta
#include "Tabela_de_Simbolos.h"

//Cria um registro para ser adicionado à tabela de símbolos
REGISTRO_SIMBOLO *Criar_Registro(char *valor, TOKEN_NATURE natureza_token, DATA_TYPE tipo_do_token,int linha); 

//Adiciona o registro de uma declaração de identificador à tabela de símbolos
//Além de verificar e indicar se a declaração desse símbolo já foi feita no escopo
//avaliado(ERR_DECLARED)
void Adicionar_Declaracao_de_Identificador(REGISTRO_SIMBOLO *registro, PILHA *tabelas);

//Adiciona um registro de uso de identificador à tabela de símbolos
//Além de verificar se a declaração desse símbolo ainda não foi feita no escopo 
//avaliado(ERR_UNDECLARED)
void Adicionar_Uso_de_Identificador(REGISTRO_SIMBOLO *registro, PILHA *tabelas);

//Cria uma tabela de símbolos
TABELA_DE_SIMBOLOS *Cria_Tabela();

//Cria uma pilha para colocar as mútliplas tabelas de símbolos em escopos aninhados
PILHA *Cria_Pilha();

void Destroi_Tabela(TABELA_DE_SIMBOLOS *tabela);

PILHA *Empilha_Tabela(PILHA *pilha, TABELA_DE_SIMBOLOS *tabela);

PILHA *Desempilha_Tabela(PILHA *pilha);

