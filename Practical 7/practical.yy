%{
    #include "practical.tab.hh"
    // #include <stdio.h>
    #include <cstring>
    #include <map>
    #include <string>
    using namespace std;

    extern int yylineno;
    extern char* yytext;
    int yylex();
    void yyerror(char*);
    void yyerror(const string&);
    void assert_is_compatible(const char* lhs, const char* rhs);

    map<string, pair<string, string>> symb_tab;     // maps from variable name to 
                                                    // variable datatype and value
%}

%union{
    char* name;
    int ivalue;
    float fvalue;
}

%define parse.lac full
%define parse.error detailed
%define parse.trace
%right '='
%left '+''-'
%left '*''/'
%left IF_ELSE_PREC
%token SEPERATOR REL SIGN IF ELSE ASSIGN LPAREN RPAREN LBRACES RBRACES
%token <ivalue> ICONST
%token <fvalue> FCONST
%token <name> ID DATATYPE
%type Start Decl BooleanThing IfStat ElseStat EXPR MATH

%%

Start : /* EMPTY */
        | EXPR Start
        | IfStat Start

Decl : DATATYPE ID SEPERATOR      { symb_tab[$2] = make_pair($1, ""); }

BooleanThing : ID REL ID
                {
                    assert_is_compatible($1, $3);
                }

IfStat : IF LPAREN BooleanThing RPAREN LBRACES EXPR RBRACES ElseStat

ElseStat : /* EMPTY */ %prec IF_ELSE_PREC
            | ELSE LBRACES EXPR RBRACES

EXPR :  Decl
        | ID ASSIGN MATH SEPERATOR
            { 
                /* Should check for datatype */
            }

MATH :    FCONST
        | ICONST
        | ICONST SIGN ICONST
        | FCONST SIGN FCONST
        | ID SIGN ID
            {
                assert_is_compatible($1, $3);
            }

%%

void assert_is_compatible(const char* lhs, const char* rhs){
    auto f = symb_tab.find(lhs);
    auto s = symb_tab.find(rhs);
    if(f == symb_tab.end()){
        yyerror((string("Identifier ") + lhs + string(" Not Declared")));
        exit(-1);
    }
    else if(s == symb_tab.end()){
        yyerror((string("Identifier ") + rhs + string(" Not Declared")));
        exit(-1);
    }

    if(f->second.first != s->second.first){
        yyerror((string("Incompatible Datatypes ") + string(f->second.first) + " " + string(s->second.first)));
        exit(-1);
    }
}

void yyerror(const string& s){
    char* temp = (char*) malloc(sizeof(char) * s.size());
    strcpy(temp, s.c_str());
    yyerror(temp);
    free(temp);         // i don't think it reaches here
    // exit(-1);
}
void yyerror(char* s) {
    printf("ERROR: %s at line %d\n", s, yylineno, yytext);
}

int main(){
    int failure = yyparse();
    if(!failure){
        puts("Program Accepted");
    }else{
        puts("Program Rejected");
    }
    return 0;
}