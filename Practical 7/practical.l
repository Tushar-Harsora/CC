%{
    #include "practical.tab.hh"
    #include <stdlib.h>
%}

%option yylineno

%%

"<"|">"|"<="|">="|"||"|"&&"|"=="|"!="     { return REL; }
"int"|"float"         { yylval.name = strdup(yytext); return DATATYPE; }
"if"                    { return IF; }
"else"                  { return ELSE; }
"="                     { return ASSIGN; }
"{"                     { return LBRACES; }
"}"                     { return RBRACES; }
"("                     { return LPAREN; }
")"                     { return RPAREN; }
[0-9]+.[0-9]+           { yylval.fvalue = atof(yytext); return FCONST; }
[0-9]+                  { yylval.ivalue = atoi(yytext); return ICONST; }
[a-zA-Z][a-zA-Z0-9]*    { yylval.name = strdup(yytext); return ID; }
"+"|"-"|"/"|"*"         { return SIGN; }
";"                     { return SEPERATOR; }
[ \n\t]+                {  }
.                       { return yytext[0]; }
%%