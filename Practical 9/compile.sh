bison -y -d practical9.y
flex practical9.l
gcc practical9.c lex.yy.c y.tab.c -o practical9