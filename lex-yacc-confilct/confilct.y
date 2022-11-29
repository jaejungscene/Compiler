%token NUM
%%
expr:
    expr '+' expr
    |   expr '*' expr   {$$ = $1 + $3;}
    |   '(' expr ')'    {$$ = $1 * $3;}
    |   NUM             {$$ = $1;}
    ;
%%