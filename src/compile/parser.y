%define parse.lac full
%define parse.error detailed 
%{
	#include "./AstNode.h"
	#include <stdio.h>
	#include <stdlib.h>

	extern FILE *yyin;
%}

%union { char *sVal; struct AstNode *nodeVal; }

%start root

%token <sVal>	TOK_LET "let" // Keywords
%token <sVal>	TOK_IDENT TOK_STR TOK_NUMBER
%token <sVal>	TOK_SEMICOLONS ";"
%token <sVal>	TOK_PLUS "+"

%type <nodeVal> root stmt expr var_decl

%nonassoc		TOK_NUMBER TOK_STR TOK_IDENT TOK_SEMICOLONS
%left			TOK_PLUS

%%

root:			stmt																												{}
	|			root stmt																											{}

stmt:			var_decl ";"	 																									{}
	|			expr ";"																											{}

expr:			expr "+" expr																										{}
	|			TOK_NUMBER																											{}
	|			TOK_STR																												{}
																																			
var_decl:		TOK_LET TOK_IDENT																									{}
%%

void yyerror(const char *s)
{
	printf("%s\n", s);
}

int main(int argc, char **argv)
{
	yyin = fopen(argv[1], "r");
	yyparse();
	
	return 0;
}
