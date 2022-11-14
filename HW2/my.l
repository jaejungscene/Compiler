%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 
#include "my.h"

// void initialize_symbol_table();
// void initialize_token_list();
// int save_token(TOK_NAME tag, char* token);
// void print_token_list();
// NodePointer syntax_analyse(int i);


%}

id          [a-zA-Z][a-zA-Z0-9]*
integer     [1-9][0-9]*
realNum     (0|[1-9][0-9]*)\.(0|[0-9]*[1-9])*
string      \".*\"
sub         (sub\(\".*\"\,[0-9]+\,[0-9]+\))

%%

{id}        {return save_token(ID, yytext);}
{integer}   {return save_token(INT, yytext);}
{realNum}   {return save_token(REAL, yytext);}
{string}    {return save_token(STR, yytext);}
{sub}       {return save_token(SUB, yytext);}
"+"         {return save_token(PLUS, yytext);}
"-"         {return save_token(MINUS, yytext);}
"*"         {return save_token(MULTI, yytext);}
"/"         {return save_token(DIVID, yytext);}
"="         {return save_token(ASSIGN, yytext);}
"("         {return save_token(LB, yytext);}
")"         {return save_token(RB, yytext);}
"\n"        {return -1;}
" "         {return 1;}
","         {return 1;}
"$asp"      {return 2;}
"$symbol"   {return 3;}
"$tok"      {return 4;}
.           {return -5;}


%%


int main(void)
{
    int tok;
    int count = 0;
    initialize_symbol_table();
    while(!feof(stdin)){
        printf("\n>");
        repeat_here:
        tok = yylex();
        if(tok == -1)   {}
        else if(tok==0) {break;} //EOF
        else if(tok==1) {goto repeat_here;}
        else if(tok==2) {
            printf("syntax tree\n");
            /* print_syntax_tree(); */
        }
        else if(tok==3){
            printf("symbol table\n");
            /* print_symbol_table(); */
        }
        else if(tok==4){
            print_token_list();
        }
        else if(tok==5){
            printf("runtime error: not real number, need integer\n");
        }
        else{
            printf("lexical error: %s\n", yytext);
            token_num = 0;
            yyrestart(yyin);
            initialize_token_list();
        }
        /* free_sytax_tree(); */
        /* syntax_tree_head = syntax_analyse(0); */
        A();
        /* initialize_token_list(); */
    }
    printf("exit...\n");
}