%{
#include <stdio.h>
#include <ctype.h>
extern int yylex();
extern void yyerror(const char *);
%}
%token DIGIT
%%

line:
    expr '\n'           {printf("%d\n", $1);}
    ;

expr:
    expr '+' term       {$$ = $1 + $3;}
    | term              
    ;

term:
    term '*' factor     {$$ = $1 * $3;}
    | factor            
    ;

factor:
    '(' expr ')'        {$$ = $2;}
    | DIGIT             
    ;

%%

int yylex()
{
    int c;
    c = getchar();
    printf("c: %c\n", c);
    if(isdigit(c)){
        yylval = c - '0';
        return DIGIT;
    }
    return c;
}

void yyerror(const char *msg)
{
    fprintf(stderr, "jgh %s\n", msg);
}

int main()
{
    yyparse();
    return 0;
}
