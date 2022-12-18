#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_SBT_LEN 100
#define MAX_SYM_LEN 11


typedef enum Type{INT=1, REAL, SYM, ASYM, VART, ASSIGN, PLUS, MINUS, MULTIP, DIVIDE, OP, UMIN, UPLU}Type;

typedef struct Node* NodePointer;
typedef struct Node{
    Type type;
    Type inhtype;
    char name[MAX_SYM_LEN*5];
    union {
        int i;
        double d;
    }val;
    NodePointer left;
    NodePointer right;
}Node;

typedef struct Record* RecordPointer;
typedef struct Record{
    char name[MAX_SYM_LEN];
    Type type;
    int offset;
    bool flag;
}Record;
Record sbt[MAX_SBT_LEN];
int sbtTop = 0;
int tnum = 1;
int linenum = 1;


int get_recordIdx(char* name, Type type)
{
    int i;
    int flag = 0;
    for(i=0; i<sbtTop; i++){
        if(strcmp(sbt[i].name,name)==0){
            flag = 1;
            break;
        }
    }
    if(flag==0 && type==ASYM){
        return -1;
    }
    else if(flag==0 && type==SYM){
        fprintf(stderr, "=> Runtime error: symbol \'%s\' has been not defined before\n", name);
        exit(1);
    }
    else{
        if(type==SYM)
            sbt[i].flag = true;
        else
            sbt[i].flag = false;
        return i;
    }
}

void push_record(char* name, Type type)
{
    strcpy(sbt[sbtTop].name, name);
    sbt[sbtTop].type = type;
    if(sbt[sbtTop-1].type==INT)
        sbt[sbtTop].offset = sbt[sbtTop-1].offset + 4;
    else if(sbt[sbtTop-1].type==REAL)
        sbt[sbtTop].offset = sbt[sbtTop-1].offset + 8;
    else{}
    sbtTop++;
    return;
}

void recur_push_recode(NodePointer node, Type type)
{   
    if(node==NULL)
        return;
    recur_push_recode(node->left, type);
    push_record(node->name, type);
    node->val.i = sbtTop-1;
}


NodePointer makeNode(Type type, void* value, NodePointer left, NodePointer right)
{
    NodePointer newnode = (NodePointer)malloc(sizeof(Node));
    newnode->type = type;
    newnode->inhtype = type;
    newnode->left = left;
    newnode->right = right;

    if(type==INT){
        newnode->val.i = *((int*)value);
    }
    else if(type==REAL){
        newnode->val.d = *((double*)value);
    }
    else if(type==SYM || type==ASYM){
        strcpy(newnode->name, (char*)value);
        newnode->val.i = get_recordIdx(newnode->name, type);
        newnode->inhtype = sbt[newnode->val.i].type;
    }
    else if(type==VART){
        if(strcmp("int",(char*)value)==0){
            recur_push_recode(newnode->left, INT);
        }
        else{
            recur_push_recode(newnode->left, REAL);
        }
    }
    else{}
    return newnode;
}


char get_op(Type type)
{
    if(type==UMIN || type==MINUS)
        return '-';
    if(type==UPLU || type==PLUS)
        return '+';
    if(type==MULTIP)
        return '*';
    if(type==DIVIDE)
        return '/';
}


void gen(NodePointer now, NodePointer parent)
{
    if(now==NULL || now->type==INT || now->type==REAL || now->type==SYM || now->type==VART){
        return;
    }
    
    FILE *out = fopen("./ic.out", "a");
    if(now->type==ASSIGN)
    {
        gen(now->right, now);
        now->inhtype = now->right->inhtype;
        if(now->right->type==INT){
            now->type = INT;
            now->val.i = now->right->val.i;
        }
        else if(now->right->type==REAL){
            now->type = REAL;
            now->val.d = now->right->val.d;
        }
        else{
            now->type = OP;
            now->val.i = now->right->val.i;
        }
        gen(now->left, now);
    }
    else if(now->type==ASYM)
    {
        gen(now->left, parent);

        char str[MAX_SYM_LEN*11];
        if(parent->type==INT){
            sprintf(str, "%s = %d\n", now->name, parent->val.i);
            fputs(str, out);
            printf("%s", str);
        }
        else if(parent->type==REAL){
            sprintf(str, "%s = %.2f\n", now->name, parent->val.d);
            fputs(str, out);
            printf("%s", str);
        }
        else if(parent->type==SYM){
            sprintf(str, "%s = %s\n", now->name, parent->name);
            fputs(str, out);
            printf("%s", str);
        }
        else{
            sprintf(str, "%s = t%d\n", now->name, parent->val.i);
            fputs(str, out);
            printf("%s", str);
        }

        if(parent->inhtype!=now->inhtype){
            fputs("=> Runtime error: type doesn\'t mach between \'defined variable\' and \'assigned value\'\n", out);
            fprintf(stderr, "=> Runtime error: type doesn\'t mach between \'defined variable\' and \'assigned value\'\n");
            exit(1);
        }
    }
    else if(now->type==UMIN || now->type==UPLU)
    {
        gen(now->left, now);
        now->val.i = tnum;
        now->inhtype = now->left->inhtype;
        char op = get_op(now->type);

        char str[MAX_SYM_LEN*10];
        if(now->left->type==INT){
            sprintf(str, "t%d = %c%d\n", tnum, op,now->left->val.i);
            fputs(str,out);
            printf("%s", str);
        }
        else if(now->left->type==REAL){
            sprintf(str, "t%d = %c%.2f\n", tnum, op,now->left->val.d);
            fputs(str,out);
            printf("%s", str);
        }
        else if(now->left->type==SYM){
            sprintf(str, "t%d = %c%s\n", tnum, op, now->left->name);
            fputs(str,out);
            printf("%s", str);
        }
        else if(now->left->type>=PLUS){
            sprintf(str, "t%d = %ct%d\n", tnum, op,now->left->val.i);
            fputs(str,out);
            printf("%s", str);
        }
        else{}
        tnum++;
    }
    else if(now->type>=PLUS)
    {   //operation
        gen(now->left, now);
        gen(now->right, now);
        if(now->left->inhtype==REAL || now->right->inhtype==REAL)
            now->inhtype = REAL;
        else
            now->inhtype = INT;

        char str[MAX_SYM_LEN*10];
        if(now->left->inhtype != now->right->inhtype){
            if(now->left->inhtype==INT){
                sprintf(str, "t%d = inttoreal %s\n", tnum, now->left->name);
                fputs(str, out);
                printf("%s", str);
                sprintf(now->left->name, "t%d", tnum);
            }
            else{
                sprintf(str, "t%d = inttoreal %s\n", tnum, now->right->name);
                fputs(str, out);
                printf("%s", str);
                sprintf(now->right->name, "t%d", tnum);
            }
            tnum++;
        }
        char op = get_op(now->type);


        Type ltype = now->left->type;
        char temp[MAX_SYM_LEN];
        if(ltype==INT){
            sprintf(temp, "%d",now->left->val.i);
        }
        else if(ltype==REAL){
            sprintf(temp, "%.2f",now->left->val.d);
        }
        else if(ltype==SYM){
            sprintf(temp, "%s",now->left->name);
        }
        else if(ltype>=PLUS){
            sprintf(temp, "t%d",now->left->val.i);
        }
        else{}

        if(now->right->type==INT){
            sprintf(str, "t%d = %s %c %d\n", tnum, temp, op, now->right->val.i);
            fputs(str, out);
            printf("%s", str);
        }
        else if(now->right->type==REAL){
            sprintf(str, "t%d = %s %c %.2f\n", tnum, temp, op, now->right->val.d);
            fputs(str, out);
            printf("%s", str);
        }
        else if(now->right->type==SYM){
            sprintf(str, "t%d = %s %c %s\n", tnum, temp, op, now->right->name);
            fputs(str, out);
            printf("%s", str);
        }
        else if(now->right->type>=PLUS){
            sprintf(str, "t%d = %s %c t%d\n", tnum, temp, op, now->right->val.i);
            fputs(str, out);
            printf("%s", str);
        }
        else{}
        now->val.i = tnum;
        tnum++;
    }
    else{}
    fclose(out);
    return;
}


void free_tree(NodePointer now)
{
    if(now==NULL)
        return;
    free_tree(now->left);
    free_tree(now->right);
    free(now);
    return;
}

void free_all(NodePointer root){
    free_tree(root);
}


void out_symbol_table()
{   
    FILE *out = fopen("./sbt.out", "w");
    if(sbt[0].name == NULL){
        fputs("\0", out);
    }
    else{
        fputs("\0", out);
        fclose(out);

        char str[MAX_SYM_LEN*5];
        out = fopen("./sbt.out", "a");
        printf("\n\n---------symbol table---------\n");
        for(int i=0; i<sbtTop; i++){
            if(sbt[i].type==INT && sbt[i].flag==true){
                sprintf(str, "%-15s%-15s%-15d\n", sbt[i].name, "int", sbt[i].offset);
                fputs(str, out);
                printf("%s", str);
            }
            else if(sbt[i].type==REAL && sbt[i].flag==true){
                sprintf(str, "%-15s%-15s%-15d\n", sbt[i].name, "double", sbt[i].offset);
                fputs(str, out);
                printf("%s", str);
            }
        }
    }
    fclose(out);
    return;
}
