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

typedef enum TOK_NAME{ID=1, SUB, INT, REAL, STR, ASSIGN, PLUS, MINUS,  MULTI, DIVID, LB, RB}TOK_NAME;

typedef struct Record* RecordPointer;
typedef struct Record{
    char name[MAX_NAME_LEN];
    // char type[MAX_TYPE_LEN];
    TOK_NAME type;
    union{
        int i;
        double f;
        char str[MAX_VALUE_LEN];
    }value;
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
    NodePointer next;
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



void initialize_symbol_table();
void initialize_token_list_and_syntax_tree();

TOK_NAME check_INT_REAL(char* str);
bool isOperator(TOK_NAME tag);

int save_token(TOK_NAME tag, char* token);
void print_token_list();

void push_symbol_table(char* lvalue, NodePointer rvalue);
void print_symbol_table();

int dfs_for_depth(NodePointer now, int depth);
void print_syntax_tree();
NodePointer make_syntax_tree();
NodePointer A();
NodePointer restA();
NodePointer E();
NodePointer restE();
NodePointer T();
NodePointer restT();
NodePointer F();
NodePointer restF();
void dfs_result(NodePointer node);


void init()
{
    // save_token(INT,"3");
    // save_token(PLUS,"+");
    // save_token(LB,"(");
    // save_token(INT,"2");
    // save_token(MINUS,"-");
    // save_token(INT,"1");
    // save_token(RB,")");

    // save_token(INT,"3");
    // save_token(PLUS,"+");
    // save_token(INT,"2");
    // save_token(MULTI,"*");
    // save_token(INT,"1");

    // save_token(ID,"val");
    // save_token(ASSIGN,"=");
    // save_token(INT,"2");
    // save_token(MULTI,"*");
    // save_token(INT,"3");
    // save_token(PLUS,"+");
    // save_token(INT,"2");

    // save_token(MINUS,"-");
    // save_token(INT,"5");
    // save_token(MULTI,"*");
    // save_token(INT,"3");

    save_token(INT,"1");
    save_token(MULTI,"*");
    save_token(LB,"(");
    save_token(INT,"2");
    save_token(PLUS,"+");
    save_token(INT,"3");
    save_token(RB,")");
}



int main(void)
{
    // NodePointer node;
    // node = -1;
    // printf("%d\n\n", node);
    // char token[30] = "sub(\'asdf\',1,2)";
    init();
    printf("check01\n");
    print_token_list();
    printf("check02\n");
    syntax_tree_head = make_syntax_tree();
    printf("check03\n");
    tree_depth = dfs_for_depth(syntax_tree_head, 0);
    printf("tree depth: %d\n", tree_depth);
    printf("check04\n");
    print_syntax_tree();
    printf("check05\n");
    result(syntax_tree_head);
    initialize_token_list_and_syntax_tree();
    printf("check06\n");
    return 0;
}




void push_symbol_table(char* lvalue, NodePointer rvalue)
{   
    strcpy(symbol_table[symbol_num].name,lvalue);
    symbol_table[symbol_num].type = rvalue->tag;
    if(rvalue->tag==INT)
        symbol_table[symbol_num].value.i = rvalue->data.i;
    if(rvalue->tag==REAL)
        symbol_table[symbol_num].value.f = rvalue->data.f;
    if(rvalue->tag==STR)
        strcpy(symbol_table[symbol_num].value.str,rvalue->data.str);
    symbol_num++;
}


void dfs_result(NodePointer node)
{
    if(node->tag==ASSIGN){
    }
    else{
    }
    printf("hello result\n");
}


void initialize_symbol_table()
{
    for(int i=0; i<symbol_num; i++){
        memset(symbol_table[i].name, '\0', MAX_NAME_LEN);
        symbol_table[i].type = 0;
        // memset(symbol_table[i].type, '\0', MAX_VALUE_LEN);
        memset(symbol_table[i].value.str, '\0', MAX_VALUE_LEN);
    }
}

void initialize_token_list_and_syntax_tree()
{
    for(int i=0; i<token_num; i++){
        free(token_list[i]);
    }
    token_num = 0;
}

TOK_NAME check_INT_REAL(char* str)
{
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
        newNode->next = NULL;
        newNode->tag = SUB;
        strcpy(newNode->data.str, ptr); //sub
        token_list[token_num] = newNode;
        token_num++;

        ptr = strtok(NULL,",");
        newNode = (NodePointer)malloc(sizeof(Node));
        newNode->left = NULL;
        newNode->mid = NULL;
        newNode->right = NULL;
        newNode->next = NULL;
        newNode->tag = STR;
        strcpy(newNode->data.str, ptr); //"string"
        token_list[token_num] = newNode;
        token_num++;

        ptr = strtok(NULL,",");
        newNode = (NodePointer)malloc(sizeof(Node));
        newNode->left = NULL;
        newNode->mid = NULL;
        newNode->right = NULL;
        newNode->next = NULL;
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
        newNode->next = NULL;
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
        newNode->mid = NULL;
        newNode->right = NULL;
        newNode->next = NULL;
        token_list[token_num] = newNode;
        token_num++;
    }
    // printf("finish save token\n");
    return 1;
}

bool isOperator(TOK_NAME tag)
{
    if(tag==PLUS || tag==MINUS || tag==MULTI || tag==DIVID ||
        tag==SUB || tag==ASSIGN)
        return true;
    else
        return false;
}

void print_symbol_table(){
    printf("name\t\ttype\t\tvalue\n");
    printf("--------------------------------------------\n");
    for(int i=0; i<symbol_num; i++){
        printf("%s\t\t", symbol_table[i].name);
        if(symbol_table[i].type==INT)
            printf("int\t\t%d\n", symbol_table[i].value.i);
        else if(symbol_table[i].type==REAL)
            printf("real\t\t%f\n", symbol_table[i].value.f);
        else if(symbol_table[i].type==STR)
            printf("string\t\t\"%s\"\n", symbol_table[i].value.str);
    }
    printf("\n");
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

int dfs_for_depth(NodePointer now, int depth)
{ // function for finding the syntax tree depth
    if(now!=NULL){
        depth++;
        int d1, d2;
        d1 = dfs_for_depth(now->left, depth);
        d2 = dfs_for_depth(now->right, depth);
        if(d1<d2) return d2;
        else return d1;
    }
    else{
        return depth;
    }
}

void push_Q(NodePointer* front, NodePointer* tail, NodePointer element, int* Qnum){
    if(*front==NULL){
        *front = element;
        *tail = element;
    }
    else{
        (*tail)->next = element;
        *tail = element;
    }
    *Qnum += 1;
}

NodePointer pop_Q(NodePointer* front, int*Qnum){
    NodePointer temp = *front;
    *front = (*front)->next;
    *Qnum -= 1;
    return temp;
}

void print_syntax_tree()
{ // breath first travel using linked list structure(queue)
    NodePointer now = syntax_tree_head;
    NodePointer front = NULL; // queue front
    NodePointer tail = NULL; // queue tail
    int Qnum = 0;
    int count = 0;

    // root node
    if(now->left != NULL) {count++; push_Q(&front,&tail,now->left,&Qnum);}
    if(now->mid != NULL) {count++; push_Q(&front,&tail,now->mid,&Qnum);}
    if(now->right != NULL) {count++; push_Q(&front,&tail,now->right,&Qnum);}
    printf("%s%d\n",now->data.str, count);

    // othres node
    for(int i=1; i<tree_depth; i++){
        int QSize = Qnum;
        for(int j=0; j<QSize; j++){
            count = 0;
            NodePointer frontNode = pop_Q(&front, &Qnum);
            if(isOperator(frontNode->tag)){
                if(frontNode->left != NULL) {count++; push_Q(&front,&tail,frontNode->left,&Qnum);}
                if(frontNode->mid != NULL) {count++; push_Q(&front,&tail,frontNode->mid,&Qnum);}
                if(frontNode->right != NULL) {count++; push_Q(&front,&tail,frontNode->right,&Qnum);}
                printf("%s%d\t", frontNode->data.str, count);
            }
            else{
                if(frontNode->tag==INT)
                    printf("%d\t", frontNode->data.i);
                else if(frontNode->tag==REAL)
                    printf("%f\t", frontNode->data.f);
                else if(frontNode->tag==STR)
                    printf("%s\t", frontNode->data.str);
                else if(frontNode->tag==ID)
                    printf("%s\t", frontNode->data.str);
            }
        }
        printf("\n");
    }
}


NodePointer make_syntax_tree(){
    return A();
}

NodePointer A(){
    // printf("enter A\n"); //sleep(1);
    if(k>=token_num) return NULL;
    if(token_list[k]->tag==ID){
        // printf("%s\n", token_list[k]->data.str);
        NodePointer temp = token_list[k];
        k++;
        NodePointer parent = restA();
        NodePointer iter = parent;
        while((iter->left)!=NULL){
            iter = iter->left;
        }
        iter->left = temp;
        return parent;
    }
    else{
        NodePointer temp1 = restF();
        NodePointer temp2 = restT();
        NodePointer temp3 = restE();
        if(temp3!=NULL && temp2!=NULL){
            NodePointer iter = temp3;
            while((iter->left)!=NULL){
                iter = iter->left;
            }
            iter->left = temp2;
            iter = temp2;
            while((iter->left)!=NULL){
                iter = iter->left;
            }
            iter->left = temp1;
            return temp3;
        }
        else if(temp3!=NULL){
            NodePointer iter = temp3;
            iter = temp3;
            while((iter->left)!=NULL){
                iter = iter->left;
            }
            iter->left = temp1;
            return temp3;
        }
        else if(temp2!=NULL){
            NodePointer iter = temp2;
            iter = temp2;
            while((iter->left)!=NULL){
                iter = iter->left;
            }
            iter->left = temp1;
            return temp2;
        }
        // else{
        //     printf("\nsyntax error A:\n");
        //     return (NodePointer)-1;
        // }
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
    NodePointer parent = restE();
    if(parent!=NULL){
        NodePointer temp = parent;
        while((temp->left)!=NULL){
            temp = temp->left;
        }
        temp->left = child;
        return parent;
    }
    else{
        return child;
    }
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
    if(parent!=NULL){
        NodePointer temp = parent;
        while((temp->left)!=NULL){
            temp = temp->left;
        }
        temp->left = child;
        return parent;
    }
    else{
        return child;
    }
}


NodePointer restT(){
    // printf("enter restT\n"); //sleep(1);
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
    else return NULL;
}


NodePointer F(){
    // printf("enter F\n"); //sleep(1);
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
    // printf("enter restF\n"); //sleep(1);
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
            printf("\n\nsyntax error: 01\n\n");
            return (NodePointer)-1;
        }
    }
    else{
        printf("\n\nsyntax error: 02\n\n");
        return (NodePointer)-1;
    }
}







