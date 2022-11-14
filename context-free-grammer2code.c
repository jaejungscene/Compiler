#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int i=0;
char str[50];

typedef struct Node{
    char c;
}Node;

void expr(){ 
    term();
    restexp();
}
void restexp(){
    if(str[i] == '+'){
        printf("+"); i++; term(); restexp();
    }
    else if(str[i] == '-'){
        printf("-"); i++; term(); restexp();
    }
}
void term(){ factor(); restterm(); }
void restterm() {
    if(str[i] == '*'){
        printf("*"); i++; factor(); restterm();
    }
    else;
}
void factor(){
    if(str[i] == '('){
        printf("("); i++; expr();
        if(str[i]==')'){
            printf(")");
            i++;
        }
        else{
            printf("\n\nerror\n");
        }
    }
    else if('0'<=str[i] && str[i]<='9'){
        printf("%c", str[i]); i++;
    }
    else printf("\n\n**error**\n");
}

int main(){
    while(1){
        printf("\n>");
        scanf("%s",str);
        expr();
        if(i!=strlen(str)){
            printf("\n\nerror: %s\n", &str[i]);
        }
    }
}