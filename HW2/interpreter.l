%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 
#include "interpreter.h"

%}

id          [a-zA-Z][a-zA-Z0-9]*
idx         [0-9]*
integer     [1-9][0-9]*
realNum     (0|[1-9][0-9]*)*\.(0|[0-9]*[1-9])*
string      \".*\"

%%


"sub"       {return save_token(SUB, yytext);}
{idx}       {return save_token(INT, yytext);}
{id}        {return save_token(ID, yytext);}
{integer}   {return save_token(INT, yytext);}
{realNum}   {return save_token(REAL, yytext);}
{string}    {return save_token(STR, yytext);}
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
"$ast"      {return 2;}
"$symbol"   {return 3;}
.           {return -5;}


%%

int main(void)
{
    int tok;
    int memory;
    initialize_symbol_table();
    while(!feof(stdin)){
        printf("\n>");
        repeat_here:
        tok = yylex();
        if(tok == -1)   {}
        else if(tok==0) {break;} //EOF
        else if(tok==1) {memory=1; goto repeat_here;}
        else if(tok==2) {
            memory = 2;
            print_syntax_tree();
            goto repeat_here;
        }
        else if(tok==3){
            memory = 3;
            print_symbol_table();
            goto repeat_here;
        }
        else if(tok==5){
            memory = 5;
            printf("runtime error: not real number, need integer\n");
            continue;
        }
        else{
            memory = -2;
            printf("lexical error: %s\n", yytext);
            free_temp_token_list();
            yyrestart(yyin);
            continue;
        }
        if(memory==1){
            move_token_list();
            backup_syntax_tree_head = syntax_tree_head;
            syntax_tree_head = make_syntax_tree();
            if(syntax_tree_head==NULL){
                syntax_tree_head = backup_syntax_tree_head;
            }
            tree_depth = dfs_for_depth(syntax_tree_head, 0);
            RESULT result = dfs_result(syntax_tree_head);
            if(result.tag==INT){
                printf("%d\n",result.data.i);
            }
            else if(result.tag==REAL){
                printf("%.4f\n",result.data.f);
            }
            else if(result.tag==STR){
                printf("\"%s\"\n",result.data.str);
            }
            else{
                syntax_tree_head = backup_syntax_tree_head;
            }
            token_num = 0;
            token_count = 0;
        }
    }
    initialize_token_list_and_free_syntax_tree(); 
    printf("\nexit...\n");
}