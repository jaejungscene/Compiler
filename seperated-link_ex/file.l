%{

extern unsigned charCount, wordCount, lineCount;

%}

word   [^\t\n]+
eol     \n

%%

{word} {
    wordCount++;
    charCount+=yyleng;
}
{eol} {
    charCount++;
    lineCount++;
}
.   charCount;

%%
