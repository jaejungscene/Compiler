#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_TYPE_LEN 7
#define MAX_VALUE_LEN 64
#define MAX_NAME_LEN 11
#define MAX_SYMBOL_TABLE 100
#define MAX_TOKEN_LIST 100

typedef enum TOK_NAME{ID=1, INT, REAL, STR, SUB, ASSIGN, PLUS, MINUS,  MULTI, DIVID, LB, RB}TOK_NAME;

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
int tree_depth = 0;
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

void initialize_token_list_and_syntax_tree(){
    for(int i=0; i<token_num; i++){
        free(token_list[i]);
    }
    token_num = 0;
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
    // printf("save_token\n");
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
    // printf("finish save token\n");
    return 1;
}

bool isOperator(TOK_NAME tag){
    if(tag==PLUS || tag==MINUS || tag==MULTI || tag==DIVID ||
        tag==SUB || tag==ASSIGN)
        return true;
    else
        return false;
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


void print_syntax_tree(){
    NodePointer parent = syntax_tree_head;
    NodePointer now = syntax_tree_head;
    int count = 0;
    int node_num = 0;

    // root node
    if(now->left != NULL) count++;
    if(now->mid != NULL) count++;
    if(now->right != NULL) count++;
    printf("%s%d\n",now->data.str, count);
    node_num = count;
    now = now->left;

    // othres node

    for(int i=1; i<tree_depth; i++){
        count = 0;
        while(node_num<=0){
            if(isOperator(now->tag)){
                if(now->left != NULL) count++;
                if(now->mid != NULL) count++;
                if(now->right != NULL) count++;
                printf("%s%d\n",now->data.str, count);
            }
            else{
                printf("");
                count++;
            }
            node_num--;
        }
        node_num = count;
        now = now->left;
    }
};





NodePointer A(){
    // printf("enter A\n"); //sleep(1);
    if(k>=token_num) return NULL;
    if(token_list[k]->tag==ID){
        // printf("%s\n", token_list[k]->data.str);
        NodePointer temp = token_list[k];
        k++;
        NodePointer parent = restA();
        NodePointer iter = parent;
        whlie((iter->left)!=NULL){
            iter = iter->left;
        }
        iter->left = temp;
        return parent;
    }
    else{
        NodePointer temp1 = restF();
        NodePointer temp2 = restT();
        NodePointer temp3 = restE();
        NodePointer iter = temp3;
        while((iter->left)!=NULL){
            iter = iter->left;
        }
        iter->left = temp2;
        NodePointer iter = temp2;
        while((iter->left)!=NULL){
            iter = iter->left;
        }
        iter->left = temp1;
        return temp3;
    }
}

NodePointer restA(){
    // printf("enter restA\n"); //sleep(1);
    if(k>=token_num) return NULL;
    if(token_list[k]->tag==ASSIGN){
        // printf("%s\n", token_list[k]->data.str);
        // if(syntax_tree_head == NULL) syntax_tree_head = token_list[k];
        NodePointer temp = token_list[k];
        k++;
        temp->right = A();
        return temp;
    }
    else{ 
        NodePointer parent1 = restT();
        NodePointer parent2 = restE();
        if(parent1==NULL && parent2==NULL){
            return NULL;
        }
        else if(parent1==NULL){
            return parent2;
        }
        else if(parent2==NULL){
            return parent1;
        }
        else{
            NodePointer temp = parent2;
            while((temp->left)!=NULL){
                temp = temp->left;
            }
            temp->left = parent1;
            return parent2;
        }
    }
}

NodePointer E(){
    // printf("enter E\n"); //sleep(1);
    if(k>=token_num) return NULL;
    NodePointer child = T();
    NodePointer parent restE();
    NodePointer temp = parent;
    while((temp->left)!=NULL){
        temp = temp->left;
    }
    temp->left = child;
    return parent;
}

NodePointer restE(){
    // printf("enter restE\n"); //sleep(1);
    if(k>=token_num) return NULL;
    if(token_list[k]->tag==PLUS || token_list[k]->tag==MINUS){
        // printf("%s\n", token_list[k]->data.str);
        NodePointer temp = token_list[k];
        k++;
        temp->right = T();
        NodePointer parent = restE();
        if(parent!=NULL){
            parent->left = temp;
            return parent;
        }
        else{
            return temp;
        }
    }
    // else if(token_list[k]->tag==MINUS){
    //     printf("%s\n", token_list[k]->data.str);
    //     k++;
    //     T();
    //     restE();
    // }
    else return NULL;
}


NodePointer T(){
    // printf("enter T\n"); //sleep(1);
    if(k>=token_num) return NULL;
    NodePointer child = F();
    NodePointer parent = restT();
    NodePointer temp = parent;
    while((temp->left)!=NULL){
        temp = temp->left;
    }
    temp->left = child;
    return parent;
}


NodePointer restT(){
    printf("enter restT\n"); //sleep(1);
    if(k>=token_num) return NULL;
    if(token_list[k]->tag==MULTI || token_list[k]->tag==DIVID){
        // printf("%s\n", token_list[k]->data.str);
        NodePointer temp = token_list[k];
        k++;
        temp->right = F();
        NodePointer parent = restT();
        if(parent!=NULL){
            parent->left = temp;
            return parent;
        }
        else{
            return temp;
        }
    }
    // else if(token_list[k]->tag==DIVID){
    //     // printf("%s\n", token_list[k]->data.str);
    //     NodePointer temp = token_list[k];
    //     k++;
    //     temp->right = F();
    //     NodePointer parent = restT();
    //     parent->left = temp;
    //     return parent;
    // }
    else NULL;
}


NodePointer F(){
    printf("enter F\n"); //sleep(1);
    if(k>=token_num) return NULL;
    if(token_list[k]->tag==ID){
        // printf("%s\n", token_list[k]->data.str);
        NodePointer temp = token_list[k];
        k++;
        return temp;
    }
    else{
        return restF();
    }
}

NodePointer restF(){
    printf("enter restF\n"); //sleep(1);
    if(k>=token_num) return NULL;
    if(token_list[k]->tag==LB){
        // printf("%s\n", token_list[k]->data.str);
        k++;
        NodePointer temp = A();
        if(token_list[k]->tag==RB){
            // printf("%s\n", token_list[k]->data.str);
            k++;
            return temp;
        }
        else{
            printf("\n\nsytax error: there should be \')\'\n\n");
            return (NodePointer)-1;
        }
    }
    else if(token_list[k]->tag==INT){
        // printf("%d\n", token_list[k]->data.i);
        NodePointer temp = token_list[k];
        k++;
        return temp;
    }
    else if(token_list[k]->tag==REAL){
        // printf("%f\n", token_list[k]->data.f);
        NodePointer temp = token_list[k];
        k++;
        return temp;
    }
    else if(token_list[k]->tag==STR){
        // printf("%s\n", token_list[k]->data.str);
        NodePointer temp = token_list[k];
        k++;
        return temp;
    }
    else if(token_list[k]->tag==MINUS){ //unary operator
        // printf("%s\n", token_list[k]->data.str);
        NodePointer temp = token_list[k];
        k++;
        temp->left = F();
    }
    else if(token_list[k]->tag==PLUS){
        // printf("%s\n", token_list[k]->data.str);
        NodePointer temp = token_list[k];
        k++;
        temp->left = F();
    }
    else if(token_list[k]->tag==SUB){
        // printf("%s\n", token_list[k]->data.str);
        NodePointer temp = token_list[k];
        k++;
        if(token_list[k]->tag==STR){
            // printf("%s\n", token_list[k]->data.str);
            temp->left = token_list[k];
            k++;
            temp->mid = E();
            temp->right = E();
            return temp;
        }
        else{
            // printf("\n\nsyntax error: 01\n\n");
            return (NodePointer)-1;
        }
    }
    else{
        // printf("\n\nsyntax error: 02\n\n");
        return (NodePointer)-1;
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


// void add_symbol_table(NodePointer now){
//     int i = 0;
//     while(i<=token_num){
//         if(token_list[i]==ID){
            
//         }

//     }
// }




