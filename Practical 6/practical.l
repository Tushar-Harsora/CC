%{
    #include "y.tab.h"
%}

%%

[0-9]+  |
[0-9]*.[0-9]+           { yylval.value = atof(yytext); return CONSTANT; }
[a-zA-Z][a-zA-Z0-9]*    { yylval.name = strdup(yytext); return VARIABLE; }
";"                     { return SEPERATOR; }
[ \n\t]+                {  }
.                       { return yytext[0]; }
%%