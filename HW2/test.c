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
int k = 0;



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


int save_token(TOK_NAME tag, char* token)
{
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
        if(token_list[i]->tag==INT)
            printf("%d ", token_list[i]->data.i);
        else if(token_list[i]->tag==REAL)
            printf("%f ", token_list[i]->data.f);
        else
            printf("%s ", token_list[i]->data.str);
    }
    printf("\n");
}

void print_syntax_tree();


void free_sytax_tree();


void A(){
    if(token_list[k]->tag==ID){
        printf("%s ", token_list[k]->data.str);
        k++;
        restA();
    }
    else{
        restF();
        restT();
        restE();
    }
}

void restA(){
    if(token_list[k]->tag==ASSIGN){
        printf("%s ", token_list[k]->data.str);
        k++;
        A();
    }
    else{
        restT();
        restE();
    }
}

void E(){
    T();
    restE();
}

void restE(){
    if(token_list[k]->tag==PLUS){
        printf("%s ", token_list[k]->data.str);
        k++;
        T();
        restE();
    }
    else;
}

void T(){
    F();
    restT();
}

void restT(){
    if(token_list[k]->tag==MULTI){
        printf("%s ", token_list[k]->data.str);
        k++;
        F();
        restT();
    }
    else;
}

void F(){
    if(token_list[k]->tag==ID){
        printf("%s ", token_list[k]->data.str);
        k++;
    }
    else{
        restF();
    }
}

void restF(){
    if(token_list[k]->tag==LB){
        printf("%s ", token_list[k]->data.str);
        k++;
        A();
        if(token_list[k]->tag==RB){
            printf("%s ", token_list[k]->data.str);
            k++;
        }
        else
            printf("\n\n**error**\n\n");
    }
    else if(token_list[k]->tag==INT){
        printf("%d ", token_list[k]->data.i);
        k++;
    }
    else if(token_list[k]->tag==REAL){
        printf("%f ", token_list[k]->data.f);
        k++;
    }
    else if(token_list[k]->tag==STR){
        printf("%s ", token_list[k]->data.str);
        k++;
    }
    else if(token_list[k]->tag==SUB){
        if(token_list[k]->tag==STR){
            printf("%s ", token_list[k]->data.str);
            k++;
        }
        else
            printf("\n\n**error**\n\n");
        E();
        E();
    }

}



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


// void add_symbol_table(NodePointer node){
//     int i = 0;
//     while(i<=token_num){
//         if(token_list[i]==ID){
            
//         }

//     }
// }


bool isOperator(TOK_NAME tag){
    if(tag==PLUS, tag==MINUS, tag==MULTI, tag==DIVID)
        return true;
    else
        return false;
}



// NodePointer syntax_analyse(int i){
//     if((token_list[i]->tag==ID || token_list[i]->tag==INT || token_list[i]->tag==REAL|| token_list[i]->tag==STR)
//     && token_list[i+1]->tag == 0){ //바로 처음 token만 있을 때
//         return token_list[i];
//     }
//     else if(token_list[i]->tag==ID && token_list[i+1]->tag==ASSIGN){
//         NodePointer root = token_list[i+1];
//         root->left = token_list[i];
//         i+=2;
//         root->right = syntax_analyse(i);
//         return root;
//     }
//     else if(0==i || token_list[i-1]->tag==ID){
//         if((token_list[i]->tag==PLUS || token_list[i]->tag==MINUS)){ //부호 있는 숫자
//             NodePointer temp = token_list[i];
//             token_list[i] = token_list[i+1];
//             token_list[i+1] = temp;
//             token_list[i+1]->left = token_list[i];
//             i+=2;
//         }
//         else if(token_list[i+1]->tag==REAL || token_list[i+1]->tag==INT || token_list[i+1]->tag==STR || token_list[i+1]->tag==ID){
//             i++;
//         }
//         token_list[i]->left = token_list[i-1]; //i은 operator

//         if((token_list[i]->tag==MULTI || token_list[i]->tag==DIVID) || ((token_list[i]->tag==PLUS || token_list[i]->tag==MINUS)&&((token_list[i+2]->tag==PLUS || token_list[i+2]->tag==MINUS)))){
//             token_list[i]->right = token_list[i+1]; // 일단 숫자 저장
//             i+=2;
//             NodePointer root = syntax_analyse(i);
//             root->left = token_list[i-2];
//             return root;
//         }
//         else if(token_list[i+1]->tag==LB || token_list[i+1]->tag==SUB){
//             i++;
//             token_list[i-1]->right = syntax_analyse(i);
//             return token_list[i-1];
//         }
//     }
//     else if(token_list[i]->tag==LB){ // ( 나왔을 시
//         while(token_list[i]->tag==LB){
//             LB_num++;
//             i++;
//         }
//         i++;
//         token_list[i+1]->left = token_list[i];
//         if(token_list[i+2]->tag==INT || token_list[i+2]->tag==REAL || token_list[i+2]->tag==ID){
//             token_list[i+1]->right = token_list[i+2];
//         }
//         else if(token_list[i+2]->tag==LB || token_list[i+2]->tag==SUB){
//             i+=2;
//             token_list[i-1]->right = syntax_analyse(i);
//             return token_list[i-1];
//         }
//     }
//     else if(token_list[i]->tag==INT || token_list[i]->tag==REAL){
//         token_list[i+1]->left = token_list[i];
//         i++;
//         if((token_list[i]->tag==MULTI || token_list[i]->tag==DIVID) || ((token_list[i]->tag==PLUS || token_list[i]->tag==MINUS)&&((token_list[i+2]->tag==PLUS || token_list[i+2]->tag==MINUS)))){
//             token_list[i]->right = token_list[i+1]; // 일단 숫자 저장
//             i+=2;
//             NodePointer root = syntax_analyse(i);
//             root->left = token_list[i-2];
//             return root;
//         }
//         else if(token_list[i+1]->tag==LB || token_list[i+1]->tag==SUB || 
//         ((token_list[i]->tag==PLUS || token_list[i]->tag==MINUS) && (token_list[i+2]->tag==MULTI || token_list[i+2]->tag==DIVID))){
//             i++;
//             token_list[i-1]->right = syntax_analyse(i);
//             return token_list[i-1];
//         }
//     }
//     else if(i!=0 && (token_list[i]->tag==PLUS || token_list[i]->tag==MINUS)){
//         if((token_list[i+1]->tag==INT || token_list[i+1]->tag==REAL || token_list[i+1]->tag==ID) && token_list[i+1]->tag==0){
//             token_list[i]->right = token_list[i+1];
//             return token_list[i];
//         }
//         else if((token_list[i+2]->tag==PLUS || token_list[i+2]->tag==MINUS)){
//             i+=2;
//             NodePointer root = syntax_analyse(i);
//             root->left = token_list[i-2];
//             return root;
//         }
//         else if(token_list[i+1]->tag==LB || token_list[i+1]->tag==SUB ||
//         ((token_list[i+2]->tag==MULTI || token_list[i+2]->tag==DIVID))){
//             i++;
//             token_list[i-1]->right = syntax_analyse(i);
//             return token_list[i-1];
//         }
//     }
// }



int main(void){
    char token[30] = "sub(\'asdf\',1,2)";
    char* ptr;
    printf("check\n");
    ptr = strtok(token,"(");
    printf("%s\n", ptr);
    ptr = strtok(NULL,",");
    printf("%s\n", ptr);
    ptr = strtok(NULL,",");
    printf("%s\n", ptr);
    ptr = strtok(NULL,")");
    printf("%s\n", ptr);


    // save_token(ID, "")
    return 0;
}