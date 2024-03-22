%{
#include "tokens.h"
int line_count=0;
%}

DIGITS [0-9]
LETTERS_ [a-z_]

%%
{DIGITS}+   {return(TK_LIT_INT);}
{DIGITS}*"."{DIGITS}+ {return(TK_LIT_FLOAT);}


%%
