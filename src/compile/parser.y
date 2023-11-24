%define parse.lac full
%define parse.error detailed 
%{
	#include "./AstNode.h"
	#include <stdio.h>
%}

%union { char *sVal; struct AstNode *nodeVal; }

%start root

%token <sVal>	TOK_LET // Keywords
%token <sVal>	TOK_IDENT TOK_STR TOK_NUMBER
%token <sVal>	TOK_SEMICOLONS

%type <nodeVal> root stmt expr var_decl

%nonassoc		TOK_NUMBER TOK_STR TOK_IDENT TOK_SEMICOLONS

%%

root:			stmt																														{}
	|			root stmt																													{}

stmt:			var_decl TOK_SEMICOLONS																										{}
	|			TOK_NUMBER TOK_SEMICOLONS																									{}
	|			TOK_STR TOK_SEMICOLONS																										{}
																																			
var_decl:		TOK_LET TOK_IDENT																											{}
%%

void yyerror(const char *s)
{
	printf("%s", s);
}

int main(int argc, char **argv)
{
	printf("Hello World!\n");
}
