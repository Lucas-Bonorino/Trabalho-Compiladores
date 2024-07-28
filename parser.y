//Por Lucas dos Santos Bonorino e Rafael Lacerda Busatta
%{
#include <stdio.h>
extern int get_line_number(void); 
int yylex(void);
void yyerror (char const *mensagem);
extern void *arvore;
void *pilha_de_tabelas=NULL;
%}

%start PROGRAM
%define parse.error verbose

%code requires
{
    #include "Tree_Lex_Val.h"
}

%union
{
    TOKENDATA* valor_lex;
    NODOAST* nodo;
    char*    lexema;
    int      D_Type;
}

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
%token <valor_lex> TK_IDENTIFICADOR
%token <valor_lex> TK_LIT_INT
%token <valor_lex> TK_LIT_FLOAT
%token <valor_lex> TK_LIT_FALSE
%token <valor_lex> TK_LIT_TRUE
%token TK_ERRO

%type  <D_Type>    DATA_TYPE

%type  <nodo>      FUNCTION_HEADER
%type  <nodo>      PROGRAM_COMPONENT_LIST
%type  <nodo>      PROGRAM_COMPONENT
%type  <nodo>      FUNCTION_DECLARATION
%type  <nodo>      COMMAND_BLOCK 
%type  <nodo>      COMMAND_LIST
%type  <nodo>      COMMAND
%type  <nodo>      FUNCTION_CALLING
%type  <nodo>      ARGUMENTS
%type  <nodo>      ARGUMENT_LIST
%type  <nodo>      VARIABLE_ASSIGNMENT
%type  <nodo>      RETURN_COMMAND
%type  <nodo>      FLUX_CONTROL_COMMAND
%type  <nodo>      CONDITIONAL_STRUCTURE
%type  <nodo>      OPTIONAL_ELSE_STRUCTURE
%type  <nodo>      ITERATIVE_STRUCTURE

%type  <nodo>      OPERAND
%type  <nodo>      EXPRESSION_7TH
%type  <nodo>      EXPRESSION_6TH
%type  <nodo>      EXPRESSION_5TH
%type  <nodo>      EXPRESSION_4TH
%type  <nodo>      EXPRESSION_3RD
%type  <nodo>      EXPRESSION_2ND
%type  <nodo>      EXPRESSION_1ST
%type  <lexema>    UNARY_OP  
%type  <lexema>    EQ_COMP_OP
%type  <lexema>    COMP_OP
%type  <lexema>    DIV_MUL_MOD_OP
%type  <lexema>    SUM_SUB_OP    

%%

PROGRAM:                        EMPILHA PROGRAM_COMPONENT_LIST DESEMPILHA{arvore=$2; Print_Program($2->codigo);}|  %empty{arvore=NULL;};


PROGRAM_COMPONENT_LIST:         PROGRAM_COMPONENT_LIST PROGRAM_COMPONENT {$$=Adiciona_Seguinte($1, $2); Adiciona_Codigo($$, Concat_Iloc_Op_Lists(Get_Program_Conditional($1), Get_Program_Conditional($2)));}| PROGRAM_COMPONENT {$$=$1;};


PROGRAM_COMPONENT:              VARIABLE_DECLARATION {$$=NULL;}| FUNCTION_DECLARATION {$$=$1;};  


VARIABLE_DECLARATION:           DATA_TYPE VARIABLE_LIST ',' {Tipagem_Lista_Variaveis(pilha_de_tabelas, $1);};


VARIABLE_LIST:                  VARIABLE_LIST ';' TK_IDENTIFICADOR {Cria_e_Adiciona_Registro_Variavel($3->token, VARIAVEL, UNKNOWN, get_line_number(), pilha_de_tabelas);}| TK_IDENTIFICADOR {Cria_e_Adiciona_Registro_Variavel($1->token,VARIAVEL, UNKNOWN, get_line_number(), pilha_de_tabelas);};


FUNCTION_DECLARATION:           FUNCTION_HEADER COMMAND_BLOCK DESEMPILHA {$$=Adiciona_filho($1, $2); Adiciona_Codigo($$, $2->codigo);};


FUNCTION_HEADER:                EMPILHA FUNCTION_PARAMETERS TK_OC_OR DATA_TYPE '/' TK_IDENTIFICADOR {$$=Cria_folha($6->token, $4); Cria_e_Adiciona_Registro_Funcao($6->token, FUNCAO, $4, get_line_number(), pilha_de_tabelas);};


FUNCTION_PARAMETERS:            '(' PARAMETERS ')';


PARAMETERS:                     PARAMETER_LIST | %empty ;

PARAMETER_LIST:                 PARAMETER_LIST ';' PARAMETER | PARAMETER;

PARAMETER:                      DATA_TYPE TK_IDENTIFICADOR {Cria_e_Adiciona_Registro_Variavel($2->token, VARIAVEL, $1, get_line_number(), pilha_de_tabelas);}; 


COMMAND_BLOCK:                  '{' COMMAND_LIST '}' {$$=$2;} | '{' '}' {$$=NULL;};


COMMAND_LIST:                   COMMAND_LIST COMMAND {$$=Adiciona_Seguinte($1, $2); Adiciona_Codigo($$, Concat_Iloc_Op_Lists(Get_Program_Conditional($1), Get_Program_Conditional($2)));}| COMMAND {$$=$1;} ;


COMMAND:                        VARIABLE_DECLARATION {$$=NULL;}|  VARIABLE_ASSIGNMENT {$$=$1;}| FUNCTION_CALLING ','{$$=$1;} | RETURN_COMMAND {$$=$1;}| FLUX_CONTROL_COMMAND ',' {$$=$1;}| EMPILHA COMMAND_BLOCK ',' DESEMPILHA {$$=$2;};


VARIABLE_ASSIGNMENT:            TK_IDENTIFICADOR '=' EXPRESSION_7TH ',' {REGISTRO_SIMBOLO *reg= Verifica_Uso($1->token, VARIAVEL, UNKNOWN, get_line_number(), pilha_de_tabelas); $$=Cria_nodo("=", Cria_folha($1->token, reg->tipo_simbolo), $3, reg->tipo_simbolo); Adiciona_Codigo($$,Atribution($3->codigo, reg->Deslocamento_Endereco, reg->escopo));};


FUNCTION_CALLING:               TK_IDENTIFICADOR '(' ARGUMENTS ')' {REGISTRO_SIMBOLO *reg=Verifica_Uso($1->token, FUNCAO, UNKNOWN,get_line_number(), pilha_de_tabelas);$$=Cria_nodo(StringCat("call ",$1->token), $3, NULL, reg->tipo_simbolo);};


ARGUMENTS:                      ARGUMENT_LIST {$$=$1;}| %empty {$$=NULL;}; 
ARGUMENT_LIST:                  ARGUMENT_LIST ';' EXPRESSION_7TH    {$$=Adiciona_Seguinte($1, $3);}| EXPRESSION_7TH {$$=$1;};


RETURN_COMMAND:                 TK_PR_RETURN EXPRESSION_7TH ',' {$$=Cria_nodo("return", $2, NULL, UNKNOWN);};


FLUX_CONTROL_COMMAND:           CONDITIONAL_STRUCTURE {$$=$1;}| ITERATIVE_STRUCTURE {$$=$1;};

CONDITIONAL_STRUCTURE:          TK_PR_IF '(' EXPRESSION_7TH ')' COMMAND_BLOCK OPTIONAL_ELSE_STRUCTURE {$$=Adiciona_filho(Cria_nodo("if", $3, $5, UNKNOWN),$6); Adiciona_Codigo($$, Conditional_Flux($3->codigo, $5->codigo, Get_Program_Conditional($6)));};

OPTIONAL_ELSE_STRUCTURE:        TK_PR_ELSE COMMAND_BLOCK {$$=$2;}| %empty {$$=NULL;};


ITERATIVE_STRUCTURE:            TK_PR_WHILE '(' EXPRESSION_7TH ')' COMMAND_BLOCK  {$$=Cria_nodo("while", $3, $5, UNKNOWN); Adiciona_Codigo($$, Iterative_Flux($3->codigo, $5->codigo));};


EXPRESSION_7TH:                     EXPRESSION_7TH TK_OC_OR EXPRESSION_6TH       {$$=Cria_nodo("|", $1, $3,inferencia_de_tipo_expressao($1->tipo_do_nodo, $3->tipo_do_nodo)); Adiciona_Codigo($$, Binary_Operation("|",  $1->codigo, $3->codigo));}| EXPRESSION_6TH {$$=$1;};

EXPRESSION_6TH:                     EXPRESSION_6TH TK_OC_AND EXPRESSION_5TH      {$$=Cria_nodo("&", $1, $3,inferencia_de_tipo_expressao($1->tipo_do_nodo, $3->tipo_do_nodo)); Adiciona_Codigo($$, Binary_Operation("&", $1->codigo, $3->codigo));}| EXPRESSION_5TH {$$=$1;};
 
EXPRESSION_5TH:                     EXPRESSION_5TH EQ_COMP_OP EXPRESSION_4TH     {$$=Cria_nodo($2, $1, $3,inferencia_de_tipo_expressao($1->tipo_do_nodo, $3->tipo_do_nodo)); Adiciona_Codigo($$, Binary_Operation($2,  $1->codigo, $3->codigo));}| EXPRESSION_4TH {$$=$1;};
EQ_COMP_OP:                         TK_OC_EQ {$$="==";}
                                    | TK_OC_NE {$$="!=";};

EXPRESSION_4TH:                     EXPRESSION_4TH COMP_OP EXPRESSION_3RD        {$$=Cria_nodo($2, $1, $3,inferencia_de_tipo_expressao($1->tipo_do_nodo, $3->tipo_do_nodo)); Adiciona_Codigo($$, Binary_Operation($2, $1->codigo, $3->codigo));}| EXPRESSION_3RD {$$=$1;};
COMP_OP:                            TK_OC_GE {$$=">=";}
                                    | TK_OC_LE {$$="<=";}
                                    | '<' {$$="<";}
                                    | '>' {$$=">";};

EXPRESSION_3RD:                     EXPRESSION_3RD SUM_SUB_OP EXPRESSION_2ND     {$$=Cria_nodo($2, $1, $3,inferencia_de_tipo_expressao($1->tipo_do_nodo, $3->tipo_do_nodo)); Adiciona_Codigo($$, Binary_Operation($2, $1->codigo, $3->codigo));}| EXPRESSION_2ND {$$=$1;};
SUM_SUB_OP:                         '+' {$$="+";}
                                    | '-' {$$="-";};

EXPRESSION_2ND:                     EXPRESSION_2ND DIV_MUL_MOD_OP EXPRESSION_1ST {$$=Cria_nodo($2, $1, $3,inferencia_de_tipo_expressao($1->tipo_do_nodo, $3->tipo_do_nodo)); Adiciona_Codigo($$, Binary_Operation($2, $1->codigo, $3->codigo));}| EXPRESSION_1ST {$$=$1;};
DIV_MUL_MOD_OP:                     '*' {$$="*";}
                                    | '/' {$$="/";}
                                    | '%'{$$="%";};

EXPRESSION_1ST:                     UNARY_OP EXPRESSION_1ST                      {$$=Cria_nodo($1, $2, NULL, $2->tipo_do_nodo);}| OPERAND  {$$=$1;};
UNARY_OP:                           '-' {$$="-";}| '!' {$$="!";};



OPERAND:                        TK_LIT_FALSE {$$=Cria_folha($1->token, BOOL); Adiciona_Codigo($$, Load_Literal("0"));}
                                | TK_LIT_TRUE {$$=Cria_folha($1->token, BOOL); Adiciona_Codigo($$, Load_Literal("1"));}
                                | TK_LIT_INT {$$=Cria_folha($1->token, INT); Adiciona_Codigo($$, Load_Literal($1->token));}
                                | TK_LIT_FLOAT {$$=Cria_folha($1->token, FLOAT);}
                                | TK_IDENTIFICADOR {REGISTRO_SIMBOLO *reg= Verifica_Uso($1->token, VARIAVEL, UNKNOWN, get_line_number(), pilha_de_tabelas); $$=Cria_folha($1->token, reg->tipo_simbolo); Adiciona_Codigo($$, Load_Var(reg->Deslocamento_Endereco, reg->escopo));} 
                                | FUNCTION_CALLING {$$=$1;}
                                | '(' EXPRESSION_7TH ')'{$$=$2;};  



DATA_TYPE:                      TK_PR_FLOAT {$$=FLOAT;}| TK_PR_INT {$$=INT;}| TK_PR_BOOL{$$=BOOL;};

EMPILHA:                        %empty {pilha_de_tabelas=Empilha_Tabela(pilha_de_tabelas);};
DESEMPILHA:                     %empty {pilha_de_tabelas=Desempilha_Tabela(pilha_de_tabelas);};

%%

void yyerror(char const *mensagem)
{
 printf("%s at line:%d\n", mensagem, get_line_number());
} 
