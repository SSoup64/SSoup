%{
	#include "./AstNode.h"
	#include <stdio.h>
%}

%union { char *sVal; AstNode nodeVal; }

%start root

%token <sVal>	TOK_LET // Keywords
%token <sVal>	TOK_IDENT TOK_STR TOK_NUMBER
%token <sVal>	TOK_SEMICOLONS

%type <nodeVal> root stmt expr

%nonassoc		TOK_NUMBER TOK_STR TOK_IDENT TOK_SEMICOLONS

%%

%%

int main(int argc, char **argv)
{
	printf("Hello World!\n");
}
