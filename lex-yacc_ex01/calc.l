%{
#include <stdlib.h>
#include "calc.tab.h"
void yyerror (char *);
%}
%%
[a-z]       {yylval = *yytext - 'a'; return VARIABLE;}
[0-9]+      {yylval = atoi(yytext); return INTEGER;}
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