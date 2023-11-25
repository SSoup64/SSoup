%define parse.lac full
%define parse.error detailed 
%{
	#include "./AstNode.h"
	#include "./DEBUG_traverseTree.h"
	
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>

	#define ALLOC \
	(struct AstNode *) malloc(sizeof(struct AstNode)); \

	extern FILE *yyin;
	struct AstNode rootNode;
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

root:			stmt																				{
																										rootNode = createNode(TYPE_ROOT, "", 1);
																										// nodeChildResize(&root, root.childNodesLen + 1);
																										nodeAddChild(&rootNode, *$1);
																									}
	|			root stmt																			{}

stmt:			var_decl ";"	 																	{
																										$$ = ALLOC;
																										*$$ = *$1;
																									}

	|			expr ";"																			{
																										$$ = ALLOC;
																										*$$ = *$1;
																									}

expr:			expr "+" expr																		{
																										AstNode ret = createNode(TYPE_BINOP, "+", 2);

																										nodeAddChild(&ret, *$1);
																										nodeAddChild(&ret, *$3);

																										$$ = ALLOC;
																										*$$ = ret;
																									}

	|			TOK_NUMBER																			{
																										AstNode ret = createNode(TYPE_NUMBER, strdup($1), 0);

																										$$ = ALLOC;
																										*$$ = ret;
																									}

	|			TOK_STR																				{
																										AstNode ret = createNode(TYPE_NUMBER, strdup($1), 0);

																										$$ = ALLOC;
																										*$$ = ret;
																									}
																																			
var_decl:		TOK_LET TOK_IDENT																	{}
%%

void yyerror(const char *s)
{
	printf("%s\n", s);
}

int main(int argc, char **argv)
{
	yyin = fopen(argv[1], "r");
	yyparse();
	
	traverseTree(rootNode, 0);
	
	return 0;
}
