//Por Lucas dos Santos Bonorino e Rafael Lacerda Busatta
#include "Tabela_de_Simbolos.h"

//Cria um registro para ser adicionado à tabela de símbolos
REGISTRO_SIMBOLO *Criar_Registro(char *valor, TOKEN_NATURE natureza_token, DATA_TYPE tipo_do_token,int linha, ESCOPO escopo_var, int *Numero_Var, int Var_Signal); 

//Adiciona o registro de uma declaração de identificador à tabela de símbolos
//Além de verificar e indicar se a declaração desse símbolo já foi feita no escopo
//avaliado(ERR_DECLARED)
void Adicionar_Declaracao_de_Identificador(REGISTRO_SIMBOLO *registro, PILHA *tabelas);

//Adiciona um registro de uso de identificador à tabela de símbolos
//Além de verificar se a declaração desse símbolo ainda não foi feita no escopo 
//avaliado(ERR_UNDECLARED), além de verificar erros semânticos de uso do 
//Identificador(ERR_VARIABLE e ERR_FUNCTION)
REGISTRO_SIMBOLO *Uso_de_Identificador(REGISTRO_SIMBOLO *registro, PILHA *tabelas);

//Cria uma tabela de símbolos
TABELA_DE_SIMBOLOS *Cria_Tabela();

//Cria uma pilha para colocar as mútliplas tabelas de símbolos em escopos aninhados
PILHA *Cria_Pilha();

PILHA *Empilha_Tabela(PILHA *pilha);

PILHA *Desempilha_Tabela(PILHA *pilha);


void Cria_e_Adiciona_Registro_Funcao(char *valor, TOKEN_NATURE natureza_token, DATA_TYPE tipo_do_token,int linha, PILHA *pilha);

void Cria_e_Adiciona_Registro_Variavel(char *valor, TOKEN_NATURE natureza_token, DATA_TYPE tipo_do_token,int linha, PILHA *pilha);

REGISTRO_SIMBOLO *Verifica_Uso(char *valor, TOKEN_NATURE natureza_token, DATA_TYPE tipo_do_token,int linha, PILHA *pilha);

void Imprime_Tabela(TABELA_DE_SIMBOLOS *tabela);

void Tipagem_Lista_Variaveis(PILHA *tabelas, DATA_TYPE tipo);

