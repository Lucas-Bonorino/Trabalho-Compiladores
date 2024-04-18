//Por Lucas dos Santos Bonorino e Rafael Lacerda Busatta
%{
#include <stdio.h>
extern int get_line_number(void); 
int yylex(void);
void yyerror (char const *mensagem);
%}

%start PROGRAM
%define parse.error verbose

%token TK_PR_INT
%token TK_PR_FLOAT
%token TK_PR_BOOL
%token TK_PR_IF
%token TK_PR_ELSE
%token TK_PR_WHILE
%token TK_PR_RETURN
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_OC_AND
%token TK_OC_OR
%token TK_IDENTIFICADOR
%token TK_LIT_INT
%token TK_LIT_FLOAT
%token TK_LIT_FALSE
%token TK_LIT_TRUE
%token TK_ERRO

%%
//Seção 3
//O programa pode ser vazio, 
PROGRAM:                        PROGRAM_COMPONENT_LIST |  %empty;

//ou uma lista de componentes de um programa 
PROGRAM_COMPONENT_LIST:         PROGRAM_COMPONENT_LIST PROGRAM_COMPONENT | PROGRAM_COMPONENT;

//Um componente de um programa pode ser uma declaração de variável global 
//Ou uma declaração de função
PROGRAM_COMPONENT:              VARIABLE_DECLARATION | FUNCTION_DECLARATION;  

//Seção 3.1 
//Uma declaração de variável consiste em seu tipo
VARIABLE_DECLARATION:           DATA_TYPE VARIABLE_LIST ',' ;

//Seguido de pelo menos um lista composta por pelo menos um nome de varíavel
//separados por ponto e vírgula
VARIABLE_LIST:                  VARIABLE_LIST ';' TK_IDENTIFICADOR | TK_IDENTIFICADOR ;

//Seção 3.2
//Cada função é definida por um cabeçalho e um corpo, o corpo da função é um bloco de comandos
FUNCTION_DECLARATION:           FUNCTION_HEADER COMMAND_BLOCK;

//O cabeçalho consiste na lista de parâmetros, seguido de TK_OC_OR o tipo de retorno, uma / e o nome da função
FUNCTION_HEADER:                FUNCTION_PARAMETERS TK_OC_OR DATA_TYPE '/' TK_IDENTIFICADOR;

//Os parâmetros da função são uma lista de parâmetros entre parênteses  
FUNCTION_PARAMETERS:            '(' PARAMETERS ')';

//Essa lista pode ser vazia
PARAMETERS:                     PARAMETER_LIST | %empty ;

// ou conter parâmetros separados por ponto e vírgula
PARAMETER_LIST:                 PARAMETER_LIST ';' PARAMETER | PARAMETER;

//Cada parâmetro é definido pelo seu tipo e nome 
PARAMETER:                      DATA_TYPE TK_IDENTIFICADOR; 

//Seção 3.3 e 3.4
//Um bloco de comandos é definido entre chaves
COMMAND_BLOCK:                  '{' COMMAND_LIST '}';

//E contém uma lista, potencialmente vazia de comandos simples
COMMAND_LIST:                   COMMAND_LIST COMMAND |  %empty;

//Um comando simples pode ser um comando de atribução, uma declaração de variável, uma chamada de função, um comando de retorno ou um comando de controle de fluxo
//Um bloco de comandos é considerado um comando simples recursivamente
COMMAND:                        VARIABLE_DECLARATION |  VARIABLE_ASSIGNMENT | FUNCTION_CALLING ',' | RETURN_COMMAND | FLUX_CONTROL_COMMAND ',' | COMMAND_BLOCK ',' ;

//Seçao 3.4 

//Uma atribuição de variável consiste em um identificador seguido pelo caractere de igualdade seguido por uma expressão
VARIABLE_ASSIGNMENT:            TK_IDENTIFICADOR '=' EXPRESSION_7TH ',' ;

//Uma chamada de função consiste em um identificador de função seguida pelos argumentos entre parênteses
FUNCTION_CALLING:               TK_IDENTIFICADOR '(' ARGUMENTS ')' ;

//A lista contem argumentos separados por ponto e vírgula, um argumento pode ser uma expressão
ARGUMENTS:                      ARGUMENT_LIST | %empty; 
ARGUMENT_LIST:                  ARGUMENT_LIST ';' EXPRESSION_7TH| EXPRESSION_7TH;

//Um comando de retorno consistem em um token "return" seguido de uma expressão
RETURN_COMMAND:                 TK_PR_RETURN EXPRESSION_7TH ',';

//A linguagem contém uma construção condicional e uma iterativa para o controle estruturado de fluxo
FLUX_CONTROL_COMMAND:           CONDITIONAL_STRUCTURE | ITERATIVE_STRUCTURE;

//A estrutura condicional consiste em um token if, seguido de uma expressão entre parênteses e então um bloco de comandos obrigatório 
CONDITIONAL_STRUCTURE:          TK_PR_IF '(' EXPRESSION_7TH ')' COMMAND_BLOCK OPTIONAL_ELSE_STRUCTURE;
//O else é opcional, e, caso empregado, é seguido obrigatoriamente por um bloco de comandos
OPTIONAL_ELSE_STRUCTURE:        TK_PR_ELSE COMMAND_BLOCK | %empty;

//A estrutura iterativa é consiste em um token while seguido de uma expressão entre parênteses e um bloco de comandos
ITERATIVE_STRUCTURE:            TK_PR_WHILE '(' EXPRESSION_7TH ')' COMMAND_BLOCK;

//Secao 3.5 
EXPRESSION_7TH:                     EXPRESSION_7TH TK_OC_OR EXPRESSION_6TH       | EXPRESSION_6TH;

EXPRESSION_6TH:                     EXPRESSION_6TH TK_OC_AND EXPRESSION_5TH      | EXPRESSION_5TH;
 
EXPRESSION_5TH:                     EXPRESSION_5TH EQ_COMP_OP EXPRESSION_4TH     | EXPRESSION_4TH;
EQ_COMP_OP:                         TK_OC_EQ | TK_OC_NE;

EXPRESSION_4TH:                     EXPRESSION_4TH COMP_OP EXPRESSION_3RD        | EXPRESSION_3RD;
COMP_OP:                            TK_OC_GE | TK_OC_LE | '<' | '>' ;

EXPRESSION_3RD:                     EXPRESSION_3RD SUM_SUB_OP EXPRESSION_2ND     | EXPRESSION_2ND;
SUM_SUB_OP:                         '+' | '-';

EXPRESSION_2ND:                     EXPRESSION_2ND DIV_MUL_MOD_OP EXPRESSION_1ST | EXPRESSION_1ST;
DIV_MUL_MOD_OP:                     '*' | '/' | '%';

EXPRESSION_1ST:                     UNARY_OP EXPRESSION_1ST                      | OPERAND;
UNARY_OP:                           '-' | '!';

//Operandos podem ser literais, identificadores, ou chamadas de funções
//mas para permitir a mudança de associatividade através dos parênteses,
//também permitimos que sejam expressões entre parênteses, assim a recursão
//é feita sem que seja necessariamente feita a associatividade a esquerda
OPERAND:                        TK_LIT_FALSE | TK_LIT_TRUE | TK_LIT_INT | TK_LIT_FLOAT | TK_IDENTIFICADOR | FUNCTION_CALLING | '(' EXPRESSION_7TH ')';  

DATA_TYPE:                      TK_PR_FLOAT | TK_PR_INT | TK_PR_BOOL;
           
%%

void yyerror(char const *mensagem)
{
 printf("%s at line:%d\n", mensagem, get_line_number());
} 
