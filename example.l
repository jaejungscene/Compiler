%{

#define LETTER 1
#define DIGIT 2 

%}

blank [\t\n]+
letter [a-zA-Z]
digit[0-9]

%%

{blank};
{letter} {return LETTER;}
{digit} {return DIGIT;}

%%

int main(void)
{
    int tok;
    while((tok=yylex())!=0){
        if(tok==LETTER)
            printf("letter!\n");
        else if(tok==DIGIT)
            printf("digit!\n");
        else
            printf("blank!\n");
    }
}
