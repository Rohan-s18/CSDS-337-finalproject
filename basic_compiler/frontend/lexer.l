/* Lexer written for Basic*/
%{
#include "parser.tab.hh"
#include <sstream>

static std::stringstream ss;
%}

%option noyywrap

%x strlit

%%
[ \t\n]               ;  /* Ignores Whitespace */
"INTEGER"                 {return INT_TYPE;} // These were adjusted for BASIC
"FLOAT"                   {return FLOAT_TYPE;}
"STRING"                  {return STRING_TYPE;}
"VOID"                    {return VOID_TYPE;} 
"BOOL"                    {return BOOL_TYPE}
";"                       {return SEMICOLON;}
"("                       {return LPAREN;}
")"                       {return RPAREN;}
","                       {return COMMA;}
"{"                       {return LBRACE;}
"}"                       {return RBRACE;}
"IF"                      {return IF;}
"THEN"                    {return THEN;} // Added for BASIC
"ELSE"                    {return ELSE;}
"ENDIF"                   {return ENDIF;} // Added for BASIC
"WHILE"                   {return WHILE;}
"WEND"                    {return WEND;} // Added for BASIC
"FOR"                     {return FOR;}
"NEXT"                    {return NEXT;} // Added for BASIC
"TO"                      {return TO;} // Added for BASIC
"STEP"                    {return STEP;} // Added for BASIC
"BREAK"                   {return BREAK;}
"RETURN"                  {return RETURN;}
"="                       {return EQUALS_SIGN;}
"OR"                      {return LOGICAL_OR;} // Changed for BASIC (no longer using ||)
"AND"                     {return LOGICAL_AND;} // Changed for BASIC
"NOT"                     {return LOGICAL_NOT;} // Changed for BASIC
">"                       {return RELOP_GT;}
"<"                       {return RELOP_LT;}
">="                      {return RELOP_GE;}
"<="                      {return RELOP_LE;}
"=="                      {return RELOP_EQ;} // Changed for BASIC
"!="                      {return RELOP_NE;} // Changed for BASIC
"+"                       {return ARITH_PLUS;}
"-"                       {return ARITH_MINUS;}
"*"                       {return ARITH_MULT;}
"/"                       {return ARITH_DIV;}
"MOD"                     {return ARITH_MOD;} // Changed for BASIC
"..."                     {return VARIADIC;}
"STEP"                    {return STEP;} // Added for BASIC
"FALSE" | "TRUE" {yylval.boolval = strcmp(yytext, "TRUE") == 0; return BOOL_LITERAL;} // Adjusted for BASIC (instead of lowercase)
[0-9]+ {yylval.intval = atoi(yytext); return INT_LITERAL;}
([0-9]+[.])?[0-9]+ {yylval.fltval = atof(yytext); return FLOAT_LITERAL;}

\"                  { BEGIN strlit; ss.str(std::string()); }
<strlit>[^\\"\n]*   { ss << yytext;}
<strlit>\\n         { ss << '\n';}
<strlit>\\t         { ss << '\t';}
<strlit>\\[\\"]     { ss << yytext[1]; /*escaped quote or backslash*/ }
<strlit>\"          { yylval.strval = strdup((char *) ss.str().c_str()); BEGIN 0; return STRING_LITERAL; }
<strlit>\\.         { printf("Invalid escape character '%s'\n", yytext); }
<strlit>\n          { printf("Found newline in string\n"); }

[_a-zA-Z][_a-zA-Z0-9]* {yylval.strval = strdup(yytext); return ID;}
. {printf("Unrecognized character %c\n", *yytext);}
%%

  /*int main(int argc, char **argv) {
  int tok;

  while((tok = yylex())) {
    printf("%d", tok);
    if(tok == INT_LITERAL) printf(" = %d\n", yyintval);
    else if (tok == FLOAT_LITERAL) printf(" = %0.3f\n", yyfltval);
    else if (tok == STRING_LITERAL) printf(" = %s\n", yytext);
    else if (tok == ID) printf(" called %s\n", yytext);
    else printf("\n");
  }
}
  */
