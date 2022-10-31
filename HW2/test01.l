%{

int yylval;
#define LETTER 1
#define DIGIT 2 
#define BLANK 3

%}

blank   [\t\n]+
letter  [a-zA-Z]
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
    while((tok=yylex())!=0){
        if(tok==LETTER){
            printf("letter: %c\n", yylval);
        }
        else if(tok==DIGIT){
            printf("digit: %d\n", yylval);
        }
        else{
            printf("blank!\n");
        }
    }
}
