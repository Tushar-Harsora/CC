%{
    #include <stdio.h>
    #include "y.tab.h"
%}

%%

[0-9]+          |
[0-9]*.[0-9]+           { yylval.value = atof(yytext); return NUMBER; }
";"                     { return SEPERATOR; }
[ \n\t]+                {  }
.                       { return yytext[0]; }
%%