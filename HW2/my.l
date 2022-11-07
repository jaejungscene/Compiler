%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_TYPE_LEN 7
#define MAX_VALUE_LEN 64
#define MAX_NAME_LEN 11
#define MAX_SYMBOL_TABLE 100
#define MAX_TOKEN_LIST 100

typedef enum TOK_NAME{ID=1, SUB, INT, REAL, STR, ASSIGN, PLUS, MINUS,  MULTI, DIVID, LB, RB}TOK_NAME;

typedef struct Record* RecordPointer;
typedef struct Record{
    char name[MAX_NAME_LEN];
    char type[MAX_TYPE_LEN];
    char value[MAX_VALUE_LEN];
}Record;

typedef struct Node* NodePointer;
typedef struct Node{
    TOK_NAME tag;
    union{
        int i;
        double f;
        char str[MAX_VALUE_LEN];
    }data;
    NodePointer left;
    NodePointer mid;
    NodePointer right;
}Node;

NodePointer token_list[MAX_TOKEN_LIST];
int token_num = 0;
Record symbol_table[MAX_SYMBOL_TABLE];
int symbol_num = 0;
NodePointer syntax_tree_head;
char* yylval;
int LB_num = 0;

void save_token(TOK_NAME name, char* token);

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
        printf(">");
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
            runtimErr:
            token_num = 0;
            yyrestart(yyin);
            initialize_token_list();
        }
        syntax_tree_head = syntax_analyse(0);
    }
    printf("exit...\n");
}

void initialize_symbol_table(){
    for(int i=0; i<symbol_num; i++){
        memset(symbol_table[i].name, '\0', MAX_NAME_LEN);
        memset(symbol_table[i].type, '\0', MAX_TYPE_LEN);
        memset(symbol_table[i].value, '\0', MAX_VALUE_LEN);
    }
}


void initialize_token_list(){
    for(int i=0; i<token_num; i++){
        free(token_list[i]);
    }
}

TOK_NAME check_INT_REAL(char* str){
    for(int i=0; i<strlen(str); i++){
        if('.' == str[i])
            return REAL;
    }
    return INT;
}

void save_token(TOK_NAME tag, char* token){
    if(tag==SUB){
        char* ptr;
        NodePointer newNode;
        ptr = strtok(token,"(");
        newNode = (NodePointer)malloc(sizeof(Node));
        newNode->left = NULL;
        newNode->mid = NULL;
        newNode->right = NULL;
        newNode->tag = SUB;
        strcpy(newNode->data.str, ptr); //sub
        token_list[token_num] = newNode;
        token_num++;

        ptr = strtok(NULL,",");
        newNode = (NodePointer)malloc(sizeof(Node));
        newNode->left = NULL;
        newNode->mid = NULL;
        newNode->right = NULL;
        newNode->tag = STR;
        strcpy(newNode->data.str, ptr); //"string"
        token_list[token_num] = newNode;
        token_num++;

        ptr = strtok(NULL,",");
        newNode = (NodePointer)malloc(sizeof(Node));
        newNode->left = NULL;
        newNode->mid = NULL;
        newNode->right = NULL;
        newNode->tag = INT;
        if(REAL == check_INT_REAL(token)){
            return 5;
        }
        newNode->data.i = atoi(token); //int
        token_list[token_num] = newNode;
        token_num++;

        ptr = strtok(NULL,")");
        newNode = (NodePointer)malloc(sizeof(Node));
        newNode->left = NULL;
        newNode->mid = NULL;
        newNode->right = NULL;
        newNode->tag = INT;
        if(REAL == check_INT_REAL(token)){
            return 5;
        }
        newNode->data.i = atoi(token); //int
        token_list[token_num] = newNode;
        token_num++;
    }
    else{
        NodePointer newNode = (NodePointer)malloc(sizeof(Node));
        newNode->tag = tag;
        if(tag==INT)
            newNode->data.i = atoi(token);
        else if(tag==REAL)
            newNode->data.f = atof(token);
        else
            strcpy(newNode->data.str, token);
        newNode->left = NULL;
        newNode->right = NULL;
        token_list[token_num] = newNode;
        token_num++;
    }
    return 1;
}


void print_symbol_table(){
    
}

void print_token_list(){
    for(int i=0; i<token_num; i++){
        printf("%s ", token_list[i].value);
    }
    printf("\n");
}

void print_syntax_tree();



/* void print_value(char* id_name){
    for(int i=0; i<symbol_num; i++){
        if(!strcpy(symbol_table[i].name, id_name)){
            if(!strcpy(symbol_table[i].type,"string")){
                printf("\"%s\"\n", symbol_table[i].value);
            }
            else if(!strcpy(symbol_table[i].type,"int")){
                printf("%d\n", atoi(symbol_table[i].value));
            }
            else if(!strcpy(symbol_table[i].type,"real")){
                printf("%f\n", atof(symbol_table[i].value));
            }
            break;
        }
    }
} */



void token_traval(){
    int i=0;
    i

}


bool isOperator(TOK_NAME tag){
    if(tag==PLUS, tag==MINUS, tag==MULTI, tag==DIVID)
        return true;
    else
        return false;
}


NodePointer syntax_analyse(int i){
    if((token_list[i]==ID || token_list[i]->tag==INT || token_list[i]->tag==REAL|| token_list[i]->tag==STR)
    && token_list[i+1]->tag == 0){ //바로 처음 token만 있을 때
        return token_list[i];
    }
    else if(token_list[i]==ID && token_list[i+1]->tag==ASSIGN){
        NodePointer root = token_list[i+1];
        root->left = token_list[i];
        i+=2;
        root->right = syntax_analyse(i);
        return root;
    }
    else if(i==0 || token_list[i-1]->tag=ID){
        if((token_list[i]==PLUS || token_list[i]==MINUS)){ //부호 있는 숫자
            NodePointer temp = token_list[i];
            token_list[i] = token_list[i+1];
            token_list[i+1] = temp;
            token_list[i+1]->left = token_list[i];
            i+=2;
        }
        else if(token_list[i+1]->tag==REAL || token_list[i+1]->tag==INT || token_list[i+1]->tag==STR || token_list[i+1]->tag==ID){
            i++;
        }
        token_list[i]->left = token_list[i-1]; //i은 operator

        if((token_list[i]->tag==MULTI || token_list[i]->tag==DIVID) || ((token_list[i]->tag==PLUS || token_list[i]->tag==MINUS)&&((token_list[i+2]->tag==PLUS || token_list[i+2]->tag==MINUS)))){
            token_list[i]->right = token_list[i+1]; // 일단 숫자 저장
            i+=2;
            NodePointer root = syntax_analyse(i);
            root->left = token_list[i-2];
            return root;
        }
        else if(token_list[i+1]->tag==LB || token_list[i+1]->tag==SUB){
            i++;
            token_list[i-1]->right = syntax_analyse(i);
            return token_list[i-1];
        }
    }
    else if(token_list[i]->tag==LB){ // ( 나왔을 시
        while(token_list[i]->tag==LB){
            LB_num++;
            i++;
        }
        i++;
        token_list[i+1]->left = token_list[i];
        if(token_list[i+2]->tag==INT || token_list[i+2]->tag==REAL || token_list[i+2]->tag==ID){
            token_list[i+1]->right = token_list[i+2];
        }
        else if(token_list[i+2]->tag==LB || token_list[i+2]->tag==SUB){
            i+=2;
            token_list[i-1]->right = syntax_analyse(i);
            return token_list[i-1];
        }
    }
    else if(token_list[i]->tag==INT || token_list[i]->tag==REAL){
        token_list[i+1]->left = token_list[i];
        i++;
        if((token_list[i]->tag==MULTI || token_list[i]->tag==DIVID) || ((token_list[i]->tag==PLUS || token_list[i]->tag==MINUS)&&((token_list[i+2]->tag==PLUS || token_list[i+2]->tag==MINUS)))){
            token_list[i]->right = token_list[i+1]; // 일단 숫자 저장
            i+=2;
            NodePointer root = syntax_analyse(i);
            root->left = token_list[i-2];
            return root;
        }
        else if(token_list[i+1]->tag==LB || token_list[i+1]->tag==SUB || 
        ((token_list[i]->tag==PLUS || token_list[i]->tag==MINUS) && (token_list[i+2]->tag==MULTI || token_list[i+2]->tag==DIVID))){
            i++;
            token_list[i-1]->right = syntax_analyse(i);
            return token_list[i-1];
        }
    }
    else if(i!=0 && (token_list[i]==PLUS || token_list[i]==MINUS)){
        if((token_list[i+1]->tag==INT || token_list[i+1]->tag==REAL || token_list[i+1]->tag==ID) && token_list[i+1]->tag==0){
            token_list[i]->right = token_list[i+1];
            return token_list[i];
        }
        else if((token_list[i+2]->tag==PLUS || token_list[i+2]->tag==MINUS)){
            i+=2;
            NodePointer root = syntax_analyse(i);
            root->left = token_list[i-2];
            return root;
        }
        else if(token_list[i+1]->tag==LB || token_list[i+1]->tag==SUB ||
        ((token_list[i+2]->tag==MULTI || token_list[i+2]->tag==DIVID))){
            i++;
            token_list[i-1]->right = syntax_analyse(i);
            return token_list[i-1];
        }
    }
}
