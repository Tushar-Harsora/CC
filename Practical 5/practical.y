%{
    #define YYDEBUG 1
    #include "y.tab.h"
    #include <stdio.h>
    int yylex();
    void yyerror(char*);
%}

%union{
    float value;
}

%define parse.error detailed
// %define parse.trace
%left '+''-'
%left '*''/'
%left MINUS
%token <value> NUMBER
%token SEPERATOR
%type <value> E
%type <value> S

%%

list : S |
        list S
        {
            
        }

S : E SEPERATOR
        {
            $$ = $1;
            printf("Result of Equation is %f\n", $$);
        }

E : E '+' E
        {
            $$ = $1 + $3;
        }
    | E '-' E
        {
            $$ = $1 - $3;
        }
    | E '*' E
        {
            $$ = $1 * $3;
        }
    | E '/' E
        {
            $$ = $1 / $3;
        }
    | NUMBER
        {
            $$ = $1;
        }
    | '('E')'
        {
            $$ = $2;
        }
    | '-'E %prec MINUS
        {
            $$ = -$2;
        }

%%

void yyerror(char* s) {
    printf("ERROR: %s\n", s);
}

int main(){
    yyparse();
    return 0;
}