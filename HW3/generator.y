%{
#include <stdio.h>
void yyerror(char *);
int yylex(void);
int sym[26];
%}

%union{
    int ival;
    double dval;
    int name;
}

%token <name> VARNAME
%token <ival> INTEGER
%token <dval> DOUBLE
%type <integer> expr

%left '*' '/'
%left '+' '-'
%nonassoc UMINUS
%nonassoc UPLUS
%%

program:
    statement '\n'
    | program statement '\n'
    ;

statement: 
    expr                        {printf("> %d\n", $1);}
    | VARNAME '=' expr          {sym[$1] = $3;}
    ;

expr:
    INTEGER
    | VARNAME                   {$$ = sym[$1];}
    | expr '+' expr             {$$ = $1 + $3;}
    | expr '-' expr             {$$ = $1 - $3;}
    | expr '*' expr             {$$ = $1 * $3;}
    | expr '/' expr             {$$ = $1 / $3;}
    | '-' expr %prec UMINUS     {$$ = -$2;}
    | '+' expr %prec UPLUS      {$$ = $2;}
    | '(' expr ')'              {$$ = $2;}
    ;

%%

void yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
}
int main(void) {
    yyparse();
    printf("\nexit the program...\n");
    return 0;
}