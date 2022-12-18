%{
#include <stdio.h>
#include "header.h"
void yyerror (char *);
extern int yylex();
%}


%union{
    int ival;
    double dval;
    char symbol[11];
    struct Node* node;
}

%type <node> program statement expr assign_vars
%token <symbol> SYMBOL
%token <symbol> VARTYPE
%token <ival> INTEGER
%token <dval> DOUBLE

%left '+' '-'
%left '*' '/'
%nonassoc UMINUS
%nonassoc UPLUS


%%


program:
    statement ';'                   {gen($1,NULL); free_all($1);}
    | statement ';' '\n'            {gen($1,NULL); free_all($1); linenum++;}
    | program statement ';'         {gen($2,NULL); free_all($2);}
    | program statement ';' '\n'    {gen($2,NULL); free_all($2); linenum++;}
    ;

statement:
    expr
    | VARTYPE assign_vars           {$$ = makeNode(VART, $1, $2, NULL);}
    | assign_vars '=' expr          {$$ = makeNode(ASSIGN, NULL, $1, $3);}
    ;

expr:
    SYMBOL                          {$$ = makeNode(SYM, $1, NULL, NULL);}
    | INTEGER                       {$$ = makeNode(INT, &$1, NULL, NULL);}
    | DOUBLE                        {$$ = makeNode(REAL, &$1, NULL, NULL);}
    | expr '+' expr                 {$$ = makeNode(PLUS, NULL, $1, $3);}
    | expr '-' expr                 {$$ = makeNode(MINUS, NULL, $1, $3);}
    | expr '*' expr                 {$$ = makeNode(MULTIP, NULL, $1, $3);}
    | expr '/' expr                 {$$ = makeNode(DIVIDE, NULL, $1, $3);}
    | '-' expr %prec UMINUS         {$$ = makeNode(UMIN, NULL, $2, NULL);}
    | '+' expr %prec UPLUS          {$$ = makeNode(UPLU, NULL, $2, NULL);}
    | '(' expr ')'                  {$$ = $2;}
    ;

assign_vars:
    SYMBOL                            {$$ = makeNode(ASYM, $1, NULL, NULL);}
    | assign_vars ',' SYMBOL          {$$ = makeNode(ASYM, $3, $1, NULL);}
    ;


%%



void yyerror(char *s) {
    FILE* out = fopen("./ic.out","a");
    char str[50];
    if(strcmp("syntax error",s)==0){
        sprintf(str, "=> %s(%d)\n", s, linenum);
        fputs(str,out);
        fprintf(stderr, "=> %s(%d)\n", s, linenum);
        fclose(out);
    }
    else{
        sprintf(str, "=> lexical error(%d) %s\n", linenum, s);
        fputs(str,out);
        fprintf(stderr, "=> lexical error(%d) %s\n", linenum, s);
        fclose(out);
        exit(1);
    }
}

int main(void) {
    FILE *out = fopen("./ic.out", "w");
    fputs("\0", out);
    fclose(out);

    yyparse();
    out_symbol_table();
    printf("\nexit the program...\n");
    return 0;
}