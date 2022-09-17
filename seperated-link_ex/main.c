
#include <stdio.h>
#include <stdlib.h>

unsigned charCount = 0, wordCount = 0, lineCount = 0;
extern FILE *yyin;
extern int yylex();

int main(int argc, char* argv[]){
    if(argc > 1){
        FILE *file;
        file = fopen(argv[1], "r");
        if(!file){
            fprintf(stderr, "could not open %s\n", argv[1]);
            exit(-1);
        }
        yyin = file;
    }
    yylex();
    printf("=============================================\n");
    printf("charCount: %d\n", charCount);
    printf("wordCount: %d\n", wordCount);
    printf("lineCount: %d\n", lineCount);
    return 0;
}