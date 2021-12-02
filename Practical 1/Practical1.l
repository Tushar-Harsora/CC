%{
	#include <stdio.h>
	int tokens = 0;
%}

%%

"while"|"if"|"else"|"for"			{tokens++; printf("Keyword : %s\n", yytext);}
"int"|"float"						{tokens++; printf("Datatype : %s\n", yytext);}
"+"|"-"|"/"|"*"|"%"|"=="|"="			{tokens++; printf("Operator : %s\n", yytext);}
[a-zA-Z][a-zA-Z0-9_-]*				{tokens++; printf("Identifier: %s\n", yytext);}
[0-9]*"."[0-9]+						{tokens++; printf("Float : %s\n", yytext);}
[0-9]+								{tokens++; printf("Integer: %s\n", yytext);}
[()[];,]							{tokens++; printf("Seperator: %s\n", yytext);}
. 									{printf("thing %s", yytext);};

%%

int yywrap(){return(1);}

int main(){
	yylex();

	printf("Total Tokens : %d", tokens);
}