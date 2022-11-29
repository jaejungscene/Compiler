%{
#include <stdlib.h>
#include "generator.tab.h"
void yyerror (char *);
%}

id          [a-zA-Z][a-zA-Z0-9]*
integer     [1-9][0-9]*
double      [1-9][0-9]*)*\.(0|[0-9]*[1-9])*

%%
{id}            {yylval.name = yytext[0] - 'a'; return VARNAME;}
{integer}       {yylval.value = atoi(yytext); return INTEGER;}
{double}        {return save_token(REAL, yytext); return DOUBLE;}

[-+()=/*\n] {return *yytext;}
[ \t]       ;
.           yyerror("invalid character");
%%

/**
* lex library
* https://www.ibm.com/docs/en/aix/7.1?topic=information-lex-library
**/
int yywrap(void){ // return the value 1 when the end of input occurs.
    return 1;
}