%{

int yylval;
#define LETTER 1
#define DIGIT 2 
#define BLANK 3

%}
id      [a-zA-Z][a-zA-Z0-9]*
integer
realNum
string

blank   [\t\n]+
digit   [0-9]

%%

{blank} {
    yylval = yytext[0];
    return BLANK;
}
{letter} {
    yylval = yytext[0];
    return LETTER;
}
{digit} {
    yylval = yytext[0]-'0';
    return DIGIT;
}

%%

int main(void)
{
    int tok;
    while(!feof(stdin)){
        printf(">");
        yyin = stdin;
        tok = yylex();
        if(tok==LETTER){
            printf("letter: %c\n", yylval);
        }
        else if(tok==DIGIT){
            printf("digit: %d\n", yylval);
        }
        else if(tok==-1){
            printf("finish!\n");
        }
        else{
            printf("blank!\n");
        }
    }
}
