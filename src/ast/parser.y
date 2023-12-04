%define parse.lac full
%define parse.error detailed 
%{
	#define DEBUG	

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>

	#include "./AstNode.h"
	
#ifdef DEBUG
	#include "./DEBUG_traverseTree.h"
#endif

	#include "../bytecode/Compile.h"

	#define ALLOC \
	(struct AstNode *) malloc(sizeof(struct AstNode)); \
	
	// Create variables
	extern FILE *yyin;
	struct AstNode rootNode;
%}

%union { char *sVal; struct AstNode *nodeVal; }

%start root

%token <sVal>	TOK_LET "let" TOK_DEBUG_PRINT "print" // Keywords
%token <sVal>	TOK_IDENT TOK_STR TOK_NUMBER
%token <sVal>	TOK_SEMICOLONS ";"
%token <sVal>	TOK_PLUS "+" TOK_MINUS "-" TOK_STAR "*" TOK_SLASH "/" TOK_EQ "="
%token <sVal>	TOK_LPAREN "(" TOK_RPAREN ")"

%type <nodeVal> root scope stmt expr var_decl var_assign

%nonassoc		TOK_NUMBER TOK_STR TOK_IDENT TOK_SEMICOLONS
%right			TOK_EQ
%left			TOK_PLUS TOK_MINUS
%left			TOK_STAR TOK_SLASH
%right			TOK_LPAREN TOK_RPAREN

%%
root:			scope YYEOF																			{
																										rootNode = *$1;
																									}

scope:			stmt																				{
																										AstNode ret = createNode(TYPE_SCOPE, "", 1);

																										nodeAddChild(&ret, *$1);

																										$$ = ALLOC;
																										*$$ = ret;
																									}

	|			scope stmt																			{
																										AstNode ret = *$1;

																										nodeChildResize(&ret, ret.childNodesLen + 1);

																										nodeAddChild(&ret, *$2);

																										$$ = ALLOC;
																										*$$ = ret;
																									}

stmt:			var_decl ";"	 																	{
																										$$ = ALLOC;
																										*$$ = *$1;
																									}

	|			expr ";"																			{
																										$$ = ALLOC;
																										*$$ = *$1;
																									}

	|			var_assign ";"																		{
																										$$ = ALLOC
																										*$$ = *$1;
																									}

	|			"print" "(" expr ")" ";"															{
																										AstNode ret = createNode(TYPE_DEBUG_PRINT, "", 1);

																										nodeAddChild(&ret, *$3);

																										$$ = ALLOC;
																										*$$ = ret;
																									}

expr:			expr "*" expr																		{
																										AstNode ret = createNode(TYPE_BINOP, "*", 2);

																										nodeAddChild(&ret, *$1);
																										nodeAddChild(&ret, *$3);

																										$$ = ALLOC;
																										*$$ = ret;
																									}

	|			expr "/" expr																		{
																										AstNode ret = createNode(TYPE_BINOP, "/", 2);

																										nodeAddChild(&ret, *$1);
																										nodeAddChild(&ret, *$3);

																										$$ = ALLOC;
																										*$$ = ret;
																									}

	|			expr "+" expr																		{
																										AstNode ret = createNode(TYPE_BINOP, "+", 2);

																										nodeAddChild(&ret, *$1);
																										nodeAddChild(&ret, *$3);

																										$$ = ALLOC;
																										*$$ = ret;
																									}

	|			expr "-" expr																		{
																										AstNode ret = createNode(TYPE_BINOP, "-", 2);

																										nodeAddChild(&ret, *$1);
																										nodeAddChild(&ret, *$3);

																										$$ = ALLOC;
																										*$$ = ret;
																									}

	|			"(" expr ")"																		{
																										$$ = ALLOC;
																										*$$ = *$2;
																									}

	|			TOK_NUMBER																			{
																										AstNode ret = createNode(TYPE_NUMBER, strdup($1), 0);

																										$$ = ALLOC;
																										*$$ = ret;
																									}

	|			TOK_STR																				{
																										AstNode ret = createNode(TYPE_STR, strdup($1), 0);

																										$$ = ALLOC;
																										*$$ = ret;
																									}
	
	|			TOK_IDENT																			{
																										AstNode ret = createNode(TYPE_IDENT, strdup($1), 0);
																										
																										$$ = ALLOC;
																										*$$ = ret;
																									}

var_assign:		TOK_IDENT "=" expr																	{
																										AstNode ret = createNode(TYPE_VAR_ASSIGN, "", 2);

																										nodeAddChild(&ret, createNode(TYPE_IDENT, strdup($1), 0));
																										nodeAddChild(&ret, *$3);

																										$$ = ALLOC;
																										*$$ = ret;
																									}

var_decl:		"let" TOK_IDENT																		{
																										AstNode ret = createNode(TYPE_VAR_DECL, "", 1);

																										nodeAddChild(&ret, createNode(TYPE_IDENT, strdup($2), 0));

																										$$ = ALLOC;
																										*$$ = ret;
																									}
%%

void yyerror(const char *s)
{
	printf("%s\n", s);
}

int main(int argc, char **argv)
{
	Compiler compiler = createCompiler();

	yyin = fopen(argv[1], "r");
	yyparse();

#ifdef DEBUG
	printf("Parse tree:\n");
	traverseTree(rootNode, 0);
#endif

	compile(&compiler, &rootNode);

#ifdef DEBUG
	printf("\nBytecode:\n");
	DEBUG_compilerPrintBytecode(&compiler);
#endif
	
	return 0;
}
