%{
#include <stdlib.h>
#include <string.h>
#include "generator.tab.h"
extern void yyerror(char *);
%}

id          [a-zA-Z][a-zA-Z0-9]*
integer     [1-9][0-9]*
double      (0|[1-9][0-9]*)*\.(0|[0-9]*[1-9])*

%%
("int"|"double")    {strncpy(yylval.symbol, yytext, 10);
                    yylval.symbol[strlen(yylval.symbol)] = '\0';
                    return VARTYPE;}
{id}            {strncpy(yylval.symbol, yytext, 10);
                yylval.symbol[strlen(yylval.symbol)] = '\0';
                return SYMBOL;}
{integer}       {yylval.ival = atoi(yytext); 

                return INTEGER;}
{double}        { yylval.dval = atof(yytext); return DOUBLE;}
[-+()=/*;,\n]   {return *yytext;}
[ \t\n]         ;
.               yyerror(yytext);
%%


/**
* lex library
* https://www.ibm.com/docs/en/aix/7.1?topic=information-lex-library
**/
int yywrap(void){ // return the value 1 when the end of input occurs.
    return 1;
}