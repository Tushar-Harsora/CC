%{
    #include "y.tab.h"
    #include <stdio.h>
    char* make_temp_name();
    int yylex();
    void yyerror(char*);
%}

%union{
    char* name;
    float value;
}

// %define parse.error detailed
%right '='
%left '+''-'
%left '*''/'
%token <value> CONSTANT
%token <name> VARIABLE
%token SEPERATOR
%type <name> E
%type S

%%

S : E SEPERATOR
        {
            printf("ANSWER = %s\n", $1); fflush(stdout);
        }

E : E '+' E
        {
            $$ = make_temp_name();
            printf("%s = %s + %s\n", $$, $1, $3); fflush(stdout);
        }
    | E '-' E
        {
            $$ = make_temp_name();
            printf("%s = %s - %s\n", $$, $1, $3); fflush(stdout);
        }
    | E '*' E
        {
            $$ = make_temp_name();
            printf("%s = %s * %s\n", $$, $1, $3); fflush(stdout);
        }
    | E '/' E
        {
            $$ = make_temp_name();
            printf("%s = %s / %s\n", $$, $1, $3); fflush(stdout);
        }
    | CONSTANT
        {
            int required = snprintf(NULL, 0, "%f", $1);
            char* buff = (char*) malloc(required + 1);
            snprintf(buff, required + 1, "%f", $1);
            $$ = buff;
        }
    | VARIABLE
        {
            $$ = $1;
        }
    | '('E')'
        {
            $$ = $2;
        }
    | '-'E
        {
            printf("%s = -%s\n", $$, $2); fflush(stdout);
            $$ = make_temp_name();
        }
    | E'='E
        {
            printf("%s = %s\n", $$, $3); fflush(stdout);
            $$ = $3;
        }

%%

char* make_temp_name(){
    static int counter = 0;
    char* name = malloc(sizeof(char) * 10);
    sprintf(name, "temp%d", counter);
    counter++;
    return name;
}

void yyerror(char* s) {
    printf("ERROR: %s\n", s);
}

int main(){
    yyparse();
    return 0;
}