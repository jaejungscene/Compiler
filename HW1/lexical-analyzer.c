#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Record* RecordPointer;
typedef struct Record{
    int index;
    char* data;
    RecordPointer next;
}Record;

typedef struct TableHead{
    int tableSize;
    RecordPointer first;
    RecordPointer tail;
}TableHead;

typedef enum Type{Eof, Symbol, String, Err}Type;

TableHead symbol_table_head;
TableHead string_table_head;
TableHead err_table_head;

void init_table();
Type get_token(char* str);
void add_data_table(TableHead* head, char* data);
void print_table(TableHead* head);
void print_all();
void free_table(TableHead* head);
void free_all();


int main(){
    init_table();
    char str[30];
    Type tokenType;

    while((tokenType=get_token(str)) != Eof){
        if(tokenType==Symbol){
            add_data_table(&symbol_table_head, str);
        }
        else if(tokenType==String){
            add_data_table(&string_table_head, str);
        }
        else{
            add_data_table(&err_table_head, str);
        }
    }
    print_all();
    printf("\n");

    free_all();
    return 0;
}


Type get_token(char* str){
    char c;
    int i = 0;
    while(1){
        if((c=getchar())==-1){
            break;
        }
        str[i] = c;
        i++;
    }
    return Symbol;
}


void init_table(){
    symbol_table_head.first = NULL;
    symbol_table_head.tail = NULL;
    symbol_table_head.tableSize = 0;

    string_table_head.first = NULL;
    string_table_head.tail = NULL;
    string_table_head.tableSize = 0;

    err_table_head.first = NULL;
    err_table_head.tail = NULL;
    err_table_head.tableSize = 0;
    return;
}


void add_data_table(TableHead* head, char* data){
    RecordPointer new;
    new = (RecordPointer)malloc(sizeof(*new));
    new->data = (char*)malloc(sizeof(strlen(data)));
    strcpy(new->data, data);
    new->index = 12;

    if(head->tail == NULL){
        head->first = new;
        head->tail = new;
        head->tableSize++;
    }
    else{
        new->next = head->tail->next;
        head->tail->next = new;
    }
    return;
}


void print_table(TableHead* head){
    RecordPointer now = head->first;
    while(now){
        printf("%s\n",now->data);
        now = now->next;
    }
    return;
}

void print_all(){
    printf("================= symbol table ===============\n");
    print_table(&symbol_table_head);
    printf("================= string table ===============\n");
    print_table(&string_table_head);
    printf("================= error table ================\n");
    print_table(&err_table_head);
    return;
}


void free_table(TableHead* head){
    RecordPointer now, prev;
    now = head->first;
    while(now){
        prev = now;
        now = now->next;
        free(prev);
    }
    return;
}

void free_all(){
    free_table(&symbol_table_head);
    free_table(&string_table_head);
    free_table(&err_table_head);
    return;
}