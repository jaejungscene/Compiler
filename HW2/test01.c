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

RESULT func(){
    RESULT result;
    result.tag = INT;
    result.data.i = 12;
    return result;
}

void removeLeading(char *str, char *str1)
{
    int i = 0, j, k = 0;
    while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
        i++;
    for (j = i; str[j] != '\0'; j++){
        str1[k] = str[j];
        k++;
    }
    str1[k] = '\0';
}

int save(char* token){
    char tempStr[MAX_VALUE_LEN];
    strcpy(tempStr, token);
    char* ptr;
    ptr = strtok(tempStr,"(");
    printf("%s\n",ptr);

    ptr = strtok(NULL," ,");
    printf("%s\n",ptr);

    ptr = strtok(NULL," ,");
    printf("%s\n",ptr);

    char temp[30];
    ptr = strtok(NULL,")");
    // removeLeading(ptr, temp);
    // printf("%s\n",temp);
    // printf("%s\n",ptr);
    printf("%d\n",atoi(ptr));
}

int main(void){
    double f = 3/2.2;
    char s1[10] = "abcabcabc123";
    char s2[10] = "abc";
    char str[30] = "sub(hello3, 5, 10)";

    save("sub(hello3, 5,  10)");
    // char* ptr;


    // f = 3.2;
    // sprintf(str, "-%g", f);
    // printf("%s\n", str);
    // f = atof(str);
    // printf("%f\n", f);

    // RESULT result = func();
    // if(result.tag==INT)
    //     printf("%d\n", result.data.i);
    // else
    //     printf("false\n");

    // strncpy(str, &s1[2], 3);
    // str[3] = '\0';
    // printf("%s\n", str);

    // int iter = 2;
    // char temp[30] = {NULL};
    // // printf("%s\n", strcat(temp, s2));
    // while(iter){
    //     strcpy(temp , strcat(temp, s2));
    //     printf("%s\n", temp);
    //     iter--;
    //     printf("%d\n",iter);
    // }
    // printf("%s\n", temp);


    // Node temp;
    // temp.data.i = 21;
    // printf("%d\n", temp.data.i);
    // sprintf(temp.data.str,"%d",temp.data.i);
    // printf("%s\n", temp.data.str);
    // strcpy(str,strcat(temp.data.str, s2));
    // printf("str: %s\n", str);
    



    // char result[30];
    // sprintf(str, "%.10g", .123);
    // printf("str: %s\n", str);
    // if(str[1]=='.' && str[0]=='0'){
    //     printf("processing\n");
    //     printf("%s\n", &str[1]);
    //     strcpy(result, &str[1]);
    //     strcpy(str, &str[1]);
    // }
    // printf("result: %s\n", result);
    // printf("str: %s\n", str);


    // int i = 0;
    // int len = strlen(s2);
    // int start = 0;

    // while(strncmp(&s1[start], s2, len)==0){
    //     i += 1;
    //     start += len;
    // }
    // printf("%d\n", i);


    // int x = strncmp(&s1[3], s2, 3);
    // printf("%d\n",x);

    // char store[30];
    // strcpy(store, strcat(s1, s2));

    // printf("%s\n", store);

    // printf("%f\n", f);
    // printf("%f\n", 3/2);
    // printf("%f\n", (double)3-3.2);
    return 0;
}