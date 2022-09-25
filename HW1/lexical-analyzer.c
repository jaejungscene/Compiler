#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_TOKEN 16
#define MAX_LEN 50


typedef struct Record* RecordPointer;
typedef struct Record{
    int index;
    char* data;
    RecordPointer next;
}Record;

typedef struct TableHead{
    int tableSize;
    RecordPointer first;
    // RecordPointer tail;
}TableHead;

// typedef enum Type{Integer, Double, Operators}Type;

TableHead symbol_table_head;
TableHead string_table_head;
TableHead err_table_head;

void init_table();
int isSTRING(char c);
int isDIGIT(char c);
int isID_or_LETTER(char c);
int get_id(char* line, int start, int line_num);
void analyze_line(char* line, int line_num);
int add_data_table(TableHead* head, char* data);
void print_table(TableHead* head);
void print_all();
void free_table(TableHead* head);
void free_all();


int main(){
    init_table();

    int line_num= 1;
    char line[MAX_LEN];
    while(fgets(line, MAX_LEN, stdin) != NULL){
        if(line[strlen(line)-1] == '\n'){
            line[strlen(line)-1] = '\0';
        }
        analyze_line(line, line_num);
        line_num++;
    }

    print_all();
    free_all();
    return 0;
}


int isID_or_LETTER(char c){
    if((c>='a' && c<='z') || (c>='A' && c<='Z'))
        return 1;
    else
        return 0;
}

int isDIGIT(char c){
    if((c>='0' && c<='9'))
        return 1;
    else
        return 0;
}

int isSTRING(char c){
    if((c=='\"') || (c=='\'')){
        return 1;
    }
    else{
        return 0;
    }
}

int isOPERATOR(char c){
    if(c=='+')  return 1;
    else if(c=='-') return 2;
    else if(c=='*') return 3;
    else if(c=='/') return 4;
    else if(c=='=') return 5;
    else if(c==':') return 6;
    else return 0;
}


int get_id(char* line, int start, int line_num)
{
    char c;
    char token[MAX_TOKEN] = {'\0'};
    int token_len = 0;
    // int final_flag = 0;
    int digit_flag = 0;
    int error_flag = 0;

    while(1){
        c = line[start];
        if(c==' ' || c=='\0' || c=='\n'  || c==';' || isSTRING(c) || isOPERATOR(c))
            break;
        else if(error_flag != 0){}
        else if(isDIGIT(c)){
            if(token_len == 0)
                error_flag = 1;
            else
                digit_flag = 1;
        }
        else if(isID_or_LETTER(c)){
            if(digit_flag==1)
                error_flag = 2; //can not exist letter after digit   
        }

        if(token_len < 10){
            token[token_len] = c;
            token_len++;
        }
        start++;
    }

    if(error_flag == 0){
        int index = add_data_table(&symbol_table_head, token);
        printf("<ID, %d>\t\t\t%s\n", index, token);
    }
    else if(error_flag == 1){
        printf("ERROR: line%d\t\t%s: ID can not exist digit at first\n",line_num, token);
    }
    else if(error_flag == 2){
        printf("ERROR: line%d\t\t%s: ID can not exist letter after digit\n", line_num, token);
    }

    return start;
}



int get_string(char* line, int start, int line_num)
{
    char c;
    char token[MAX_TOKEN] = {'\0'};
    int token_len = 0;
    int flag = 0;
    // token[token_len] = line[start];
    while(1){
        start++;
        c = line[start];
        if(c=='\"'){
            // token[token_len] = c;
            start++;
            break;
        }
        else if(c=='\0'){
            flag = 1;
            start++;
            break;
        }
        token[token_len] = c;
        token_len++;
    }
    if(flag==0)
        printf("<STRING, %d>\t\t\"%s\"\n", add_data_table(&string_table_head, token), token);
    else if(flag==1)
        printf("ERROR: line%d\t\t%s: there is not \" mark\n", line_num, token);
    return start;
}



void print_operator(char c){
    char str[10] = {'\0'};
    if(c=='+') strcpy(str, "PLUS");
    else if(c=='-') strcpy(str, "MINUS");
    else if(c=='*') strcpy(str, "MULTIPLE");
    else if(c=='/') strcpy(str, "DIVIDE");
    else if(c=='=') strcpy(str, "ASSIGN");
    else if(c==':') strcpy(str, "COLONE");
    printf("<%s, >\t\t%c\n", str, c);
}

int get_other(char* line, int start, int line_num)
{
    char c;
    char token[MAX_TOKEN] = {'\0'};
    int token_len = 0;
    // Type flag = Integer;
    int flag = 0; // 0(semicolon) 1~99 (for error), 100~199 (for token type)
    while(1){
        c = line[start];
        if(token_len==0){
            if(isOPERATOR(c))   flag = 100; //operator
            else if(isDIGIT(c))      flag = 101; //integer
            else if(c==';'){
                token[token_len] = c;
                token_len++;
                start++;
                break;
            }
            else{//정의되어 있지 않은 문자
                flag = 3; 
                token[token_len] = c;
                start++;
                break;
            } 
        }
        else if(token_len==1){
            if((isOPERATOR(line[start-1])==1||isOPERATOR(line[start-1])==2)){
                if(c=='0')  flag = 4;   // 0에 부호가 있는 경우 ERROR
                else if(isDIGIT(c))  flag = 101; //integer
                else if(c=='.') flag = 102; //double
            }
            else if(line[start-1]=='0' && isDIGIT(c)){ //맨 앞에 0이왔을 때 다음 digit이 온 경우
                flag = 1; //can't start with '0'    ERROR
            }
            else if(flag==100)  break;
            else if(c==' ' || c=='\0' || c=='\n' || c==';' || isSTRING(c) || isOPERATOR(c)) break;
            else if(isID_or_LETTER(c)) flag = 2; //can't exist letter   ERROR
        }
        else if(c==' ' || c=='\0' || c=='\n' || c==';' || isSTRING(c) || isOPERATOR(c)) break;
        else if(1<=flag && flag<=99){}
        else if(c=='.') flag = 102; //double
        else if(isID_or_LETTER(c)) flag = 2; //can't exist letter   ERROR

        token[token_len] = c;
        token_len++;
        start++;
    }

    if(flag==0){
        printf("<SEMICOLON, >\t\t%s\n", token);
    }
    else if(flag==1){
        printf("ERROR: line%d\t\t%s: can not start with \'0\'\n",line_num, token);
    }
    else if(flag==2){
        printf("ERROR: line%d\t\t%s: can not exist letter after digit 01\n",line_num, token);
    }
    else if(flag==3){
        printf("ERROR: line%d\t\t%s: undefined token\n",line_num, token);
    }
    else if(flag==4){
        printf("ERROR: line%d\t\t%s: 0 can\'t take sign\n",line_num, token);
    }
    else if(flag==100){
        print_operator(token[0]);
    }
    else if(flag==101){
        printf("<INT, %s>\t\t%s\n", token, token);
    }
    else if(flag==102){
        printf("<DOUBLE, %s>\t\t%s\n", token, token);
    }

    return start;
}


void analyze_line(char* line, int line_num)
{
    int start = 0;
    char c;
    while(start < strlen(line)){
        c = line[start];
        if(isID_or_LETTER(c)){
            start = get_id(line, start, line_num);
        }
        else if(isSTRING(c)){
            start = get_string(line, start, line_num);
        }
        else if(c==' '){
            start++;
        }
        else{
            start = get_other(line, start, line_num);
        }
    }
}


void init_table()
{
    symbol_table_head.first = NULL;
    symbol_table_head.tableSize = 0;

    string_table_head.first = NULL;
    string_table_head.tableSize = 0;
    return;
}


int add_data_table(TableHead* head, char* data)
{ // return index of added item
    RecordPointer new = head->first;
    RecordPointer prev = NULL;

    if(head->first==NULL){
        new = (RecordPointer)malloc(sizeof(*new));
        head->first = new;
        new->data = (char*)malloc(sizeof(strlen(data)));
        strcpy(new->data, data);
        head->tableSize++;
        new->index = head->tableSize;
    }
    else{
        while(new){
            if(strcmp(new->data, data)==0){
                break;
            }
            else{
                prev = new;
                new = new->next;
            }
        }
        if(new==NULL){
            new = (RecordPointer)malloc(sizeof(*new));
            prev->next = new;
            new->data = (char*)malloc(sizeof(strlen(data)));
            strcpy(new->data, data);
            head->tableSize++;
            new->index = head->tableSize;
        }
    }
    return new->index;
}


void print_table(TableHead* head)
{
    RecordPointer now = head->first;
    while(now){
        if(head == &string_table_head)
            printf("   %3d\t\t|\t\"%s\"\n", now->index, now->data);
        else
            printf("   %3d\t\t|\t%s\n", now->index, now->data);
        now = now->next;
    }
    return;
}

void print_all()
{
    printf("\n================< symbol table >===============\n");
    printf("   index\t|\tsymbol\n");
    printf("-----------------------------------------------\n");
    print_table(&symbol_table_head);
    printf("===============================================\n\n");

    printf("================< string table >===============\n");
    printf("   index\t|\tstring\n");
    printf("-----------------------------------------------\n");
    print_table(&string_table_head);
    printf("===============================================\n\n");
    // printf("================= error table ================\n");
    // print_table(&err_table_head);
    return;
}


void free_table(TableHead* head)
{
    RecordPointer now, prev;
    now = head->first;
    while(now){
        prev = now;
        now = now->next;
        free(prev->data);
        free(prev);
    }
    return;
}

void free_all()
{
    free_table(&symbol_table_head);
    free_table(&string_table_head);
    // free_table(&err_table_head);
    return;
}