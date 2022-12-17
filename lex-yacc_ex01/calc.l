%{
#include <stdlib.h>
#include "calc.tab.h"
void yyerror (char *);
%}

id          [a-z]
number      [0-9]+

%%
{id}        {yylval.name = yytext[0] - 'a'; return VARIABLE;}
{number}    {yylval.value = atoi(yytext); return INTEGER;}
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