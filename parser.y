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
PROGRAM:                        PROGRAM_COMPONENT_LIST |  %empty;
PROGRAM_COMPONENT_LIST:         PROGRAM_COMPONENT_LIST PROGRAM_COMPONENT | PROGRAM_COMPONENT;
PROGRAM_COMPONENT:              VARIABLE_DECLARATION | FUNCTION_DECLARATION;  

//Seção 3.1 
VARIABLE_DECLARATION:           DATA_TYPE VARIABLE_LIST ',' ;
VARIABLE_LIST:                  VARIABLE_LIST ';' TK_IDENTIFICADOR | TK_IDENTIFICADOR ;

//Seção 3.2
FUNCTION_DECLARATION:           FUNCTION_HEADER COMMAND_BLOCK;
FUNCTION_HEADER:                FUNCTION_PARAMETERS TK_OC_OR DATA_TYPE '/' TK_IDENTIFICADOR;
FUNCTION_PARAMETERS:            '(' PARAMETERS ')';
PARAMETERS:                     PARAMETER_LIST | %empty ;
PARAMETER_LIST:                 PARAMETER_LIST ';' PARAMETER | PARAMETER;
PARAMETER:                      DATA_TYPE TK_IDENTIFICADOR; 

//Seção 3.3
COMMAND_BLOCK:                  '{' COMMAND_LIST '}';
COMMAND_LIST:                   COMMAND_LIST COMMAND |  %empty;
COMMAND:                        VARIABLE_DECLARATION |  VARIABLE_ASSIGNMENT | FUNCTION_CALLING ',' | RETURN_COMMAND | FLUX_CONTROL_COMMAND | COMMAND_BLOCK;

//Seçao 3.4
VARIABLE_ASSIGNMENT:            TK_IDENTIFICADOR '=' EXPRESSION ',' ;

FUNCTION_CALLING:               TK_IDENTIFICADOR '(' ARGUMENTS ')' ;
ARGUMENTS:                      ARGUMENT_LIST | %empty; 
ARGUMENT_LIST:                  ARGUMENT_LIST ';' EXPRESSION | EXPRESSION;

RETURN_COMMAND:                 TK_PR_RETURN EXPRESSION ',';

FLUX_CONTROL_COMMAND:           CONDITIONAL_STRUCTURE | ITERATIVE_STRUCTURE;
CONDITIONAL_STRUCTURE:          TK_PR_IF '(' EXPRESSION ')' COMMAND_BLOCK OPTIONAL_ELSE_STRUCTURE;
OPTIONAL_ELSE_STRUCTURE:        TK_PR_ELSE COMMAND_BLOCK | %empty;
ITERATIVE_STRUCTURE:            TK_PR_WHILE '(' EXPRESSION ')' COMMAND_BLOCK;

//Secao 3.5 
EXPRESSION:                     EXPRESSION TK_OC_OR OPERAND;
EXPRESSION:                     EXPRESSION TK_OC_AND OPERAND;
EXPRESSION:                     EXPRESSION TK_OC_EQ OPERAND | EXPRESSION TK_OC_NE OPERAND;
EXPRESSION:                     EXPRESSION TK_OC_GE OPERAND | EXPRESSION TK_OC_LE OPERAND | EXPRESSION '<' OPERAND | EXPRESSION '>' OPERAND;
EXPRESSION:                     EXPRESSION '+' OPERAND | EXPRESSION '-' OPERAND;
EXPRESSION:                     EXPRESSION '*' OPERAND | EXPRESSION '/' OPERAND | EXPRESSION '%' OPERAND;
EXPRESSION:                     OPERAND;

//Expressões unárias são tratadas como operandos para que possamos tê-las 
//mesmo no lado direito de expressões binárias(sem necessidade do uso de
//parênteses). A priori, isso não causa nenhuma consequência indesejada,
//uma vez que um operando não pode ser uma expressão sem parênteses e, portanto
//não viola as regras de associatividade a esquerda.
UNARY_EXPRESSION:               '-' OPERAND | '!' OPERAND;

//Operandos podem ser literais, identificadores, ou chamadas de funções
//mas para permitir a mudança de associatividade através dos parênteses,
//também permitimos que sejam expressões entre parênteses, assim a recursão
//é feita sem que seja necessariamente feita a associatividade a esquerda
OPERAND:                        TK_LIT_FALSE | TK_LIT_TRUE | TK_LIT_INT | TK_LIT_FLOAT | TK_IDENTIFICADOR | FUNCTION_CALLING | '(' EXPRESSION ')' | UNARY_EXPRESSION;  

DATA_TYPE:                      TK_PR_FLOAT | TK_PR_INT | TK_PR_BOOL;
           
%%

void yyerror(char const *mensagem)
{
 printf("%s at line:%d\n", mensagem, get_line_number());
} 
