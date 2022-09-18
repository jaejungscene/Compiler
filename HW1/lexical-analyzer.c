#include <stdio.h>

typedef struct Node* nodePointer;
typedef struct Node{
    int index;
    char* info;
    nodePointer next;
}Node;

nodePointer symbol_table_head;
nodePointer string_table_head;

int main(){
    char c;
    while((c=getchar())!=-1){
        printf("%c",c);
    }
    printf("\n");
    return 0;
}