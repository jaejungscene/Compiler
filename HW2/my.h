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

typedef struct RESULT* ResultPointer;
typedef struct RESULT{
    TOK_NAME tag;
    union{
        int i;
        double f;
        char str[MAX_VALUE_LEN];
    }data;
}RESULT;

typedef struct Record* RecordPointer;
typedef struct Record{
    char name[MAX_NAME_LEN];
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
int k = 0; // using when syntax analyse


////////////////////// all function definition //////////////////////
void initialize_symbol_table();
void initialize_token_list(); // syntax tree가 완성 되었을 때
void initialize_token_list_and_free_syntax_tree(); // syntax tree가 완성되지도 못 했을 때
void free_syntax_tree(NodePointer node);


TOK_NAME check_INT_REAL(char* str);
bool isOperator(TOK_NAME tag);

int save_token(TOK_NAME tag, char* token);
void print_token_list();

RESULT get_value_from_table(char* id);
void push_symbol_table(char* lvalue, RESULT* rvalue);
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

RESULT calculation(RESULT x, RESULT y, TOK_NAME operator);
RESULT dfs_result(NodePointer node);
////////////////////////////////////////////////////////////////////



void print_symbol_table()
{   
    printf("######################################\n");
    if(symbol_num==0){
        printf("symbol table is empty.\n");
        return;
    }
    printf("name\t\ttype\t\tvalue\n");
    printf("--------------------------------------\n");
    for(int i=0; i<symbol_num; i++){
        printf("%s\t\t", symbol_table[i].name);
        if(symbol_table[i].type==INT)
            printf("int\t\t%d\n", symbol_table[i].value.i);
        else if(symbol_table[i].type==REAL)
            printf("real\t\t%.4f\n", symbol_table[i].value.f);
        else if(symbol_table[i].type==STR)
            printf("string\t\t\"%s\"\n", symbol_table[i].value.str);
    }
    printf("######################################\n");
    // printf("\n");
}



void push_symbol_table(char* lvalue, RESULT* rvalue)
{   
    int flag = 0;
    int i = 0;

    for(i=0; i<symbol_num; i++){
        if(0==strcmp(symbol_table[i].name,lvalue)){
            flag = 1;
            break;
        }
    }
    if(flag == 1){
        strcpy(symbol_table[i].name,lvalue);
        symbol_table[i].type = rvalue->tag;
        if(rvalue->tag==INT)
            symbol_table[i].value.i = rvalue->data.i;
        if(rvalue->tag==REAL)
            symbol_table[i].value.f = rvalue->data.f;
        if(rvalue->tag==STR)
            strcpy(symbol_table[i].value.str,rvalue->data.str);
    }
    else{
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
}



RESULT get_value_from_table(char* id)
{
    int i=0;
    for(i=0; i<symbol_num; i++){
        if(0==strcmp(symbol_table[i].name, id)){
            break;
        }
    }
    RESULT returnValue;
    returnValue.tag = symbol_table[i].type;
    if(returnValue.tag==INT){
        returnValue.data.i = symbol_table[i].value.i;
    }
    if(returnValue.tag==REAL){
        returnValue.data.f = symbol_table[i].value.f;
    }
    if(returnValue.tag==STR){
        strcpy(returnValue.data.str,symbol_table[i].value.str);
    }
    return returnValue;
}



RESULT calculation(RESULT x, RESULT y, TOK_NAME operator)
{
    RESULT result;
    if(x.tag==INT && y.tag==INT){
        if(operator == PLUS){
            result.tag = INT;
            result.data.i = x.data.i + y.data.i;
        }
        else if(operator == MINUS){
            result.tag = INT;
            result.data.i = x.data.i - y.data.i;
        }
        else if(operator == MULTI){
            result.tag = INT;
            result.data.i = x.data.i * y.data.i;
        }
        else if(operator == DIVID){
            result.tag = REAL;
            result.data.f = (double)x.data.i / y.data.i;
        }
        return result;
    }
    else if(x.tag==INT && y.tag==REAL){
        result.tag = REAL;
        if(operator == PLUS){
            result.data.f = x.data.i + y.data.f;
        }
        else if(operator == MINUS){
            result.data.f = x.data.i - y.data.f;
        }
        else if(operator == MULTI){
            result.data.f = x.data.i * y.data.f;
        }
        else if(operator == DIVID){
            result.data.f = x.data.i / y.data.f;
        }
        return result;
    }
    else if(x.tag==REAL && y.tag==INT){
        result.tag = REAL;
        if(operator == PLUS){
            result.data.f = x.data.f + y.data.i;
        }
        else if(operator == MINUS){
            result.data.f = x.data.f - y.data.i;
        }
        else if(operator == MULTI){
            result.data.f = x.data.f * y.data.i;
        }
        else if(operator == DIVID){
            result.data.f = x.data.f / y.data.i;
        }
        return result;
    }
    else if(x.tag==REAL && y.tag==REAL){
        result.tag = REAL;
        if(operator == PLUS){
            result.data.f = x.data.f + y.data.f;
        }
        else if(operator == MINUS){
            result.data.f = x.data.f - y.data.f;
        }
        else if(operator == MULTI){
            result.data.f = x.data.f * y.data.f;
        }
        else if(operator == DIVID){
            result.data.f = x.data.f / y.data.f;
        }
        return result;
    }
    else if(x.tag==STR && y.tag==STR){
        if(operator == PLUS){
            result.tag = STR;
            strcpy( result.data.str, strcat(x.data.str, y.data.str) );
        }
        else if(operator == DIVID){
            result.tag = INT;
            result.data.i = 0;
            int len = strlen(y.data.str);
            int start = 0;

            while(strncmp(&x.data.str[start], y.data.str, len)==0){
                result.data.i += 1;
                start += len;
            }
        }
        return result;
    }
    else if(x.tag==STR && y.tag==INT){
        result.tag = STR;
        if(operator == PLUS){
            sprintf(y.data.str,"%d",y.data.i);
            strcpy(result.data.str, strcat(x.data.str, y.data.str));
        }
        if(operator == MULTI){
            int iter = y.data.i;
            memset(result.data.str, '\0', MAX_VALUE_LEN);
            while(iter){
                strcpy(result.data.str, strcat(result.data.str, x.data.str));
                iter--;
            }
        }
        return result;
    }
    else if(x.tag==INT && y.tag==STR){
        result.tag = STR;
        if(operator == PLUS){
            sprintf(x.data.str,"%d",x.data.i);
            strcpy(result.data.str, strcat(x.data.str, y.data.str));
        }
        return result;
    }
    else if(x.tag==STR && y.tag==REAL){
        result.tag = STR;
        if(operator == PLUS){
            sprintf(y.data.str,"%.10g",y.data.f);
            if(y.data.str[0]=='0' && y.data.str[1]=='.'){
                strcpy(y.data.str,&y.data.str[1]);
            }
            strcpy(result.data.str, strcat(x.data.str, y.data.str));
        }
        return result;
    }
    else if(x.tag==REAL && y.tag==STR){
        result.tag = STR;
        if(operator == PLUS){
            sprintf(x.data.str,"%.10g",x.data.f);
            if(x.data.str[0]=='0' && x.data.str[1]=='.'){
                strcpy(x.data.str,&x.data.str[1]);
            }
            strcpy(result.data.str, strcat(x.data.str, y.data.str));
        }
        return result;
    }

}



RESULT dfs_result(NodePointer node)
{   
    if(node==NULL) { RESULT result; result.tag=-1; return result; }
    if(node->tag==ASSIGN){
        RESULT result = dfs_result(node->right);
        push_symbol_table((node->left)->data.str, &result);
        return result;
    }
    else if(node->tag==SUB){
        RESULT temp1 = dfs_result(node->left);
        RESULT temp2 = dfs_result(node->mid);
        RESULT temp3 = dfs_result(node->right);
        RESULT result;
        if(temp1.tag==STR && temp2.tag==INT && temp3.tag==INT){
            strncpy(result.data.str, &temp1.data.str[temp2.data.i], temp3.data.i);
            // int j = 0;
            // for(int i=temp2.data.i; i<temp3.data.i; i++){
            //     if(i>=strlen(temp1.data.str)){
            //         i = i%strlen(temp1.data.str);
            //     }
            //     result.data.str[j] = temp1.data.str[i];
            //     j++;
            // }
            result.tag = STR;
            result.data.str[temp3.data.i] = '\0';
            return result;
        }
        else{
            printf("runtime error:\n");
            result.tag = -1;
            return result;
        }
    }
    else if((node->tag==PLUS || node->tag==MINUS ||
            node->tag==MULTI || node->tag==DIVID) &&
            (node->right!=NULL && node->left!=NULL))
    {
        RESULT temp1 = dfs_result(node->left);
        RESULT temp2 = dfs_result(node->right);
        RESULT result = calculation(temp1, temp2, node->tag);
        return result;
    }
    else if((node->tag==MINUS || node->tag==PLUS))
    {   //uary
        RESULT result;
        if(node->left!=NULL){
            result = dfs_result(node->left);
        }
        else{
            result = dfs_result(node->right);
        }
        if(node->tag==MINUS){
            char temp[MAX_VALUE_LEN];
            if(result.tag==REAL){
                sprintf(temp,"-%g",result.data.f);
                result.data.f = atof(temp);
            }
            else{
                sprintf(temp,"-%d",result.data.i);
                result.data.i = atof(temp);
            }
        }
        return result;
    }
    else if(node->tag==INT){
        RESULT temp;
        temp.tag = INT;
        temp.data.i = node->data.i;
        return temp;
    }
    else if(node->tag==REAL){
        RESULT temp;
        temp.tag = REAL;
        temp.data.f = node->data.f;
        return temp;
    }
    else if(node->tag==STR){
        RESULT temp;
        temp.tag = STR;
        strcpy(temp.data.str, node->data.str);
        return temp;
    }
    else if(node->tag==ID){
        return get_value_from_table(node->data.str);
    }
    RESULT result;
    result.tag = -1;
    printf("runtim error:\n");
    return result;
}



void initialize_symbol_table()
{
    for(int i=0; i<symbol_num; i++){
        memset(symbol_table[i].name, '\0', MAX_NAME_LEN);
        symbol_table[i].type = 0;
        memset(symbol_table[i].value.str, '\0', MAX_VALUE_LEN);
    }
}


void free_syntax_tree(NodePointer node)
{
    if(node==NULL){
        return;
    }
    else{
        free_syntax_tree(node->left);
        free_syntax_tree(node->mid);
        free_syntax_tree(node->right);
        free_syntax_tree(node->next);
        free(node);
    }
}


void initialize_token_list_and_free_syntax_tree()
{// syntax tree가 완성되지도 못 했을 때
    for(int i=0; i<token_num; i++){
        free(token_list[i]);
        token_list[i] = NULL;
    }
    token_num = 0;
    k = 0;
}

void initialize_token_list()
{// syntax tree가 완성 되었을 때
    for(int i=0; i<token_num; i++){
        token_list[i] = NULL;
    }
    token_num = 0;
    k = 0;
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
        char tempStr[MAX_VALUE_LEN];
        strcpy(tempStr, token);
        char* ptr;
        NodePointer newNode;
        ptr = strtok(tempStr,"(");
        newNode = (NodePointer)malloc(sizeof(Node));
        newNode->left = NULL;
        newNode->mid = NULL;
        newNode->right = NULL;
        newNode->next = NULL;
        newNode->tag = SUB;
        strcpy(newNode->data.str, ptr); //sub
        token_list[token_num] = newNode;
        token_num++;

        ptr = strtok(NULL,", ");
        newNode = (NodePointer)malloc(sizeof(Node));
        newNode->left = NULL;
        newNode->mid = NULL;
        newNode->right = NULL;
        newNode->next = NULL;
        if(ptr[0]=='\"'){
            newNode->tag = STR;
            strncpy(newNode->data.str, &ptr[1], strlen(ptr)-2);
            newNode->data.str[strlen(ptr)-2] = '\0'; //"string"
        }
        else{ //ID이면 미리 바꿈.
            newNode->tag = ID;
            strcpy(newNode->data.str, ptr); //"string"
        }
        token_list[token_num] = newNode;
        token_num++;

        ptr = strtok(NULL," ,");
        newNode = (NodePointer)malloc(sizeof(Node));
        newNode->left = NULL;
        newNode->mid = NULL;
        newNode->right = NULL;
        newNode->next = NULL;
        newNode->tag = INT;
        if(REAL == check_INT_REAL(ptr)){
            return 5;
        }
        newNode->data.i = atoi(ptr); //int
        token_list[token_num] = newNode;
        token_num++;

        ptr = strtok(NULL,")");
        newNode = (NodePointer)malloc(sizeof(Node));
        newNode->left = NULL;
        newNode->mid = NULL;
        newNode->right = NULL;
        newNode->next = NULL;
        newNode->tag = INT;
        if(REAL == check_INT_REAL(ptr)){
            return 5;
        }
        newNode->data.i = atoi(ptr); //int
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
        else if(tag==STR){
            // printf("token: %s\n", token);
            // printf("token: %c\n", token[0]);
            // printf("token: %c\n", token[strlen(token)-1]);
            // token[strlen(token)-1] = '\0';
            // printf("check\n");
            strncpy(newNode->data.str, &token[1], strlen(token)-2);
            newNode->data.str[strlen(token)-2] = '\0';
            printf("newNode->data.str: %s\n",newNode->data.str);
        }
        else
            strcpy(newNode->data.str, token);
        newNode->left = NULL;
        newNode->mid = NULL;
        newNode->right = NULL;
        newNode->next = NULL;
        token_list[token_num] = newNode;
        token_num++;
    }
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



void print_token_list(){
    for(int i=0; i<token_num; i++){
        if(token_list[i]->tag==INT)
            printf("%d ", token_list[i]->data.i);
        else if(token_list[i]->tag==REAL)
            printf("%.3f ", token_list[i]->data.f);
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
                    printf("%.3f\t", frontNode->data.f);
                else if(frontNode->tag==STR)
                    printf("\"%s\"\t", frontNode->data.str);
                else if(frontNode->tag==ID)
                    printf("%s\t", frontNode->data.str);
            }
        }
        printf("\n");
    }
}



NodePointer make_syntax_tree(){
    if(token_list[k]->tag==ID && token_list[k+1]==NULL){
        token_list[k]->left = NULL;
        token_list[k]->right = NULL;
        return token_list[k];
    }

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
        else{
            return temp1;
        }
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
        return temp;
    }
    else if(token_list[k]->tag==PLUS){
        // printf("%s\n", token_list[k]->data.str);
        NodePointer temp = token_list[k];
        k++;
        temp->left = F();
        return temp;
    }
    else if(token_list[k]->tag==SUB){
        // printf("%s\n", token_list[k]->data.str);
        NodePointer temp = token_list[k];
        k++;
        if(token_list[k]->tag==STR || token_list[k]->tag==ID){
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