/*Por Lucas dos Santos Bonorino e Rafael Lacerda Busatta*/
%{
#include "parser.tab.h"
%}

%option yylineno
%option noyywrap
%x comment

DIGITS                  [0-9]
LETTERS_                [a-z_]
SPECIAL_CHARACTERS      [-!*/%+<>{}()=,;]
INTEGER_LITERAL         {DIGITS}+
FLOAT_LITERAL           {DIGITS}*"."{DIGITS}+
IDENTIFIER              {LETTERS_}({LETTERS_}|{DIGITS})*
COMMENT 		        "//".*
IGNORE                  "\n"|{COMMENT}|" "|"\t"

%%
"/*"                    {BEGIN(comment);}
<comment>"*/"           {BEGIN(INITIAL);}
<comment>.              {}
<comment>\n             {}
true                    {yylval.valor_lex=tokendata(yytext, yylineno, LITERAL); return(TK_LIT_TRUE);}
false                   {yylval.valor_lex=tokendata(yytext, yylineno, LITERAL); return(TK_LIT_FALSE);}
int                     {return(TK_PR_INT);}
float                   {return(TK_PR_FLOAT);}
bool                    {return(TK_PR_BOOL);}
if                      {return(TK_PR_IF);}
else                    {return(TK_PR_ELSE);}
while                   {return(TK_PR_WHILE);}
return                  {return(TK_PR_RETURN);}
"<="                    {return(TK_OC_LE);}
">="                    {return(TK_OC_GE);}
"=="                    {return(TK_OC_EQ);}
"!="                    {return(TK_OC_NE);}
"&"                     {return(TK_OC_AND);}
"|"                     {return(TK_OC_OR);}
{IGNORE}                {}
{INTEGER_LITERAL}       {yylval.valor_lex=tokendata(yytext, yylineno, LITERAL); return(TK_LIT_INT);}
{FLOAT_LITERAL}         {yylval.valor_lex=tokendata(yytext, yylineno, LITERAL); return(TK_LIT_FLOAT);}
{SPECIAL_CHARACTERS}    {return(yytext[0]);}
{IDENTIFIER}            {yylval.valor_lex=tokendata(yytext, yylineno, IDENTIFICADOR); return(TK_IDENTIFICADOR);}
.                       {return(TK_ERRO);}
%%


int get_line_number()
{
    return(yylineno);
}
