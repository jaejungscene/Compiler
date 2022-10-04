#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#define MAX_TOKEN 16
#define MAX_LEN 50

TableHead symbol_table_head;
TableHead string_table_head;

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


void init_table()
{
    symbol_table_head.first = NULL;
    symbol_table_head.tableSize = 0;

    string_table_head.first = NULL;
    string_table_head.tableSize = 0;
    return;
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



int get_id(char* line, int start, int line_num)
{
    char c;
    char token[MAX_TOKEN] = {'\0'};
    int token_len = 0;
    int digit_flag = 0;
    int flag = 0; // 0 success, 1~9 error flag

    while(1){
        c = line[start];
        if(c==';' || isBLANK(c) || isSTRING(c) || isOPERATOR(c))
            break;
        else if(flag != 0){}
        else if(!isDIGIT(c) && !isID_or_LETTER(c)){
            flag = 1; //ERROR: There is symbol that can't exist
            token[token_len] = c;
            start++;
            break;
        }
        else if(digit_flag==0 && isDIGIT(c))        digit_flag = 1;
        else if(digit_flag==1 && isID_or_LETTER(c)) flag = 2; //ERROR: ID should end with a number

        if(token_len < 10){
            token[token_len] = c;
            token_len++;
        }
        start++;
    }

    if(flag == 0)
        printf("<ID, %d>\t\t\t%s\n", add_data_table(&symbol_table_head, token), token);
    else if(flag == 1)
        printf("ERROR: line%d\t\t%s => there is symbol that can\'t exist\n",line_num, token);
    else if(flag == 2)
        printf("ERROR: line%d\t\t%s => ID should end with a number\n", line_num, token);

    return start;
}



int get_string(char* line, int start, int line_num)
{
    char c;
    char token[MAX_TOKEN] = {'\0'};
    int token_len = 0;
    int flag = 0;
    while(1){
        start++;
        c = line[start];
        if(c=='\"'){
            start++;
            break;
        }
        else if(c=='\0'){
            flag = 1;
            start++;
            break;
        }
        else if(line[start-1]=='\\' && isESCAPE_SEQ(c)==0){
            flag = 2;
            token[token_len] = c;
            start++;
            break;
        }
        token[token_len] = c;
        token_len++;
    }
    if(flag==0)
        printf("<STRING, %d>\t\t\"%s\"\n", add_data_table(&string_table_head, token), token);
    else if(flag==1)
        printf("ERROR: line%d\t\t\"%s => there is not \" end mark\n", line_num, token);
    else if(flag==2)
        printf("ERROR: line%d\t\t\"%s => such escape sequence doesn\'t exist\n", line_num, token);
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
    int flag = 0; // 0(semicolon) 1~99 (for error), 100~199 (for token type)
    while(1){
        c = line[start];
        if(token_len==0){
            if(isOPERATOR(c))   flag = 100; //operator
            else if(isDIGIT(c))      flag = 101; //integer
            else if(c=='.')      flag = 102; //double
            else if(c==';'){
                token[token_len] = c;
                token_len++;
                start++;
                break;
            }
            else{
                flag = 3; //ERROR: there is symbol that can't exist
                token[token_len] = c;
                start++;
                break;
            } 
        }
        else if(token_len==1){
            if(line[start-1]=='0' && isDIGIT(c))   flag = 1; //ERROR: digit can't start with '0' or there should be only one '0'
            else if(flag==100)  break;
            else if(c==';' || isBLANK(c) || isSTRING(c) || isOPERATOR(c)|| (!isDIGIT(c)&&!isID_or_LETTER(c))) break;
            else if(c=='.') flag = 102; //double
            else if(isID_or_LETTER(c)) flag = 2; //ERROR: latter can't exist after digit
        }
        else if(c==';' || isBLANK(c) || isSTRING(c) || isOPERATOR(c) || (!isDIGIT(c)&&!isID_or_LETTER(c))) break;
        else if(1<=flag && flag<=99){}
        else if(c=='.') flag = 102; //double
        else if(isID_or_LETTER(c)) flag = 2; //ERROR: latter can't exist after digit

        token[token_len] = c;
        token_len++;
        start++;
    }

    if(flag==0)
        printf("<SEMICOLON, >\t\t%s\n", token);
    else if(flag==1)
        printf("ERROR: line%d\t\t%s => can not start with \'0\' OR there should be only one \'0\'\n",line_num, token);
    else if(flag==2)
        printf("ERROR: line%d\t\t%s => letter can not exist after digit in ID\n",line_num, token);
    else if(flag==3)
        printf("ERROR: line%d\t\t%s => there is symbol that can\'t exist\n",line_num, token);
    else if(flag==100)
        print_operator(token[0]);
    else if(flag==101)
        printf("<INT, %s>\t\t%s\n", token, token);
    else if(flag==102)
        printf("<DOUBLE, %s>\t\t%s\n", token, token);

    return start;
}


int add_data_table(TableHead* head, char* data)
{
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
            if(strcmp(new->data, data)==0)
                break;
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
    return;
}