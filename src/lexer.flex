%option noyywrap

%{
    extern "C" int fileno(FILE *stream);
    #include "parser.tab.hpp"
    #include <string>
    #include <stdio.h>

%}



%%


int {return INT;}
float {return FLOAT;}
double {return DOUBLE; }
char {return CHAR;}
unsigned {return UNSIGNED;}

if {return IF;}
else {return ELSE;}
while {return WHILE;}
for {return FOR;}
return {return RETURN;}
enum {return ENUM; }
switch {return SWITCH;}
case {return CASE;}
break {return BREAK;}
default {return DEFAULT;}
sizeof {return SIZEOF;}
struct {return STRUCT;}
typedef {return TYPEDEF;}
continue {return CONTINUE;}

[(] {return LBRACKET;}
[)] {return RBRACKET;}
[{] {return LCBRACKET;}
[<][%] {return LCBRACKET;}
[%][>] {return RCBRACKET;}
[}] {return RCBRACKET;}
[\[] {return LSBRACKET;}
[\]] {return RSBRACKET;}

[:] {return COLON;}
[;] {return SEMICOLON;}
[,] {return COMMA;}
[!] {return EXCLMARK;}
[.] {return DOT;}

[=] {return EQUAL;}
[+]{2} {return DPLUS;}
[+] {return PLUS;}
[-]{2} {return DMINUS;}
[-] {return MINUS;}
[~] {return BITFLIP;}
[*] {return TIMES;}
[/] {return DIVIDE;}
[%] {return MODULO;}
[/]{2} {return QUOTIENT;}

[&]{2} {return DAND;}
[&] {return AND;}
[|]{2} {return DOR;}
[|] {return OR;}
[\^] {return XOR;}
[<]{2} {return SHIFT_L;}
[<] {return LESS;}
[>]{2} {return SHIFT_R;}
[>] {return GREATER;}

[0-9]+ { yylval.number=std::stoi(yytext) ; return NUMBER;}
[0-9]+.[0-9]*f { yylval.number=std::stoi(std::string(yytext).substr(0,std::string(yytext).size()-1)); return NUMBER_F;}
[a-zA-Z0-9_]*[a-zA-Z_]+[a-zA-Z0-9_]* { yylval.str = new std::string(yytext) ; return IDENTIFIER;}

[ \t\r\n] {;}
. {fprintf(stderr, "not recognised token\n"); exit(1);}

%%

void yyerror (char const *s)
{
  fprintf (stderr, "Parse error : %s\n", s);
  exit(1);
}