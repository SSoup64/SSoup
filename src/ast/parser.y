%define parse.lac full
%define parse.error detailed 
%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>

	#include "./AstNode.c"
	
#ifdef DEBUG
	#include "./DEBUG_traverseTree.c"
#endif

	#include "../bytecode/Compile.c"

	#define ALLOC \
	(struct AstNode *) malloc(sizeof(struct AstNode)); \
	
	// Create variables
	extern FILE *yyin;
	struct AstNode rootNode;
%}

%union { char *sVal; struct AstNode *nodeVal; }

%start root

%token <sVal>	TOK_LET "let" TOK_DEBUG_PRINT "print" TOK_FUNC "func" TOK_RETURN "return" TOK_GLOBAL "global"
%token <sVal>	TOK_IDENT TOK_STR TOK_NUMBER
%token <sVal>	TOK_SEMICOLONS ";" TOK_COMMA "," TOK_COLONS ":"
%token <sVal>	TOK_PLUS "+" TOK_MINUS "-" TOK_STAR "*" TOK_SLASH "/" TOK_EQ "="
%token <sVal>	TOK_LPAREN "(" TOK_RPAREN ")" TOK_LCPAREN "{" TOK_RCPAREN "}"

%type <nodeVal> root scope stmt expr varDecl varAssign funcDecl params exprs scopeAccess funcCall var

%nonassoc		TOK_NUMBER TOK_STR TOK_IDENT TOK_SEMICOLONS
%left			TOK_COLONS
%left			TOK_COMMA
%right			TOK_EQ
%left			TOK_PLUS TOK_MINUS
%left			TOK_STAR TOK_SLASH
%right			TOK_LPAREN TOK_RPAREN

%%
root:			scope YYEOF
				{
					rootNode = *$1;
				}

scope:			%empty
				{
					AstNode ret = createNode(TYPE_SCOPE, "", 0);

					$$ = ALLOC;
					*$$ = ret;
				}

	|			scope stmt
				{
					AstNode ret = *$1;

					nodeAddChild(&ret, *$2);

					$$ = ALLOC;
					*$$ = ret;
				}

	|			scope funcDecl	
				{
					AstNode ret = *$1;

					nodeAddChild(&ret, *$2);

					$$ = ALLOC;
					*$$ = ret;
				}

stmt:			varDecl ";"	 																	
				{
					$$ = ALLOC;
					*$$ = *$1;
				}

	|			expr ";"																			
				{
					$$ = ALLOC;
					*$$ = *$1;
				}

	|			varAssign ";"																		
				{
					$$ = ALLOC
					*$$ = *$1;
				}

	|			"print" "(" expr ")" ";"														
				{
					AstNode ret = createNode(TYPE_DEBUG_PRINT, "", NODE_DEBUG_PRINT_SIZE);

					nodeAddChild(&ret, *$3);

					$$ = ALLOC;
					*$$ = ret;
				}

	|			"return" expr ";"																	
				{
					AstNode ret = createNode(TYPE_RETURN, "", NODE_RETURN_SIZE);

					nodeAddChild(&ret, *$2);

					$$ = ALLOC;
					*$$ = ret;
				}

	|			"return" ";"																		
				{
					AstNode ret = createNode(TYPE_RETURN, "", NODE_RETURN_SIZE), emptyNode = createNode(TYPE_EMPTY_NODE, "", NODE_RETURN_SIZE);

					nodeAddChild(&ret, emptyNode);

					$$ = ALLOC;
					*$$ = ret;
				}

expr:			funcCall
				{
					$$ = ALLOC;
					*$$ = *$1;
				}

	|			expr "*" expr
				{
					AstNode ret = createNode(TYPE_BINOP, "*", NODE_BINOP_SIZE);

					nodeAddChild(&ret, *$1);
					nodeAddChild(&ret, *$3);

					$$ = ALLOC;
					*$$ = ret;
				}

	|			expr "/" expr
				{
					AstNode ret = createNode(TYPE_BINOP, "/", NODE_BINOP_SIZE);

					nodeAddChild(&ret, *$1);
					nodeAddChild(&ret, *$3);

					$$ = ALLOC;
					*$$ = ret;
				}

	|			expr "+" expr
				{
					AstNode ret = createNode(TYPE_BINOP, "+", NODE_BINOP_SIZE);

					nodeAddChild(&ret, *$1);
					nodeAddChild(&ret, *$3);

					$$ = ALLOC;
					*$$ = ret;
				}

	|			expr "-" expr
				{
					AstNode ret = createNode(TYPE_BINOP, "-", NODE_BINOP_SIZE);

					nodeAddChild(&ret, *$1);
					nodeAddChild(&ret, *$3);

					$$ = ALLOC;
					*$$ = ret;
				}

	|			"(" expr ")"
				{
					$$ = ALLOC;
					*$$ = *$2;
				}

	|			TOK_NUMBER
				{
					AstNode ret = createNode(TYPE_NUMBER, strdup($1), NODE_NUMBER_SIZE);

					$$ = ALLOC;
					*$$ = ret;
				}

	|			TOK_STR
				{
					AstNode ret = createNode(TYPE_STR, strdup($1), NODE_STRING_SIZE);

					$$ = ALLOC;
					*$$ = ret;
				}
	
	|			var
				{
					$$ = ALLOC;
					*$$ = *$1;
				}

varAssign:		var "=" expr
				{
					AstNode ret = createNode(TYPE_VAR_ASSIGN, "", NODE_VAR_ASSIGN_SIZE);

					nodeAddChild(&ret, *$1);
					nodeAddChild(&ret, *$3);

					$$ = ALLOC;
					*$$ = ret;
				}

varDecl:		"let" TOK_IDENT
				{
					AstNode ret = createNode(TYPE_VAR_DECL, "", NODE_VAR_DECL_SIZE);

					nodeAddChild(&ret, createNode(TYPE_IDENT, strdup($2), 0));

					$$ = ALLOC;
					*$$ = ret;
				}

funcDecl:		"func" TOK_IDENT "(" ")" "{" scope "}"
				{
					AstNode	ret			= createNode(TYPE_FUNC_DECL, strdup($2), NODE_FUNC_DECL_SIZE),
							emptyNode	= createNode(TYPE_EMPTY_NODE, "", 0);

					nodeAddChild(&ret, emptyNode);
					nodeAddChild(&ret, *$6);

					$$ = ALLOC;
					*$$ = ret;
				}

	|			"func" TOK_IDENT "(" params ")" "{" scope "}"
				{
					AstNode	ret = createNode(TYPE_FUNC_DECL, strdup($2), NODE_FUNC_DECL_SIZE);

					nodeAddChild(&ret, *$4);
					nodeAddChild(&ret, *$7);

					$$ = ALLOC;
					*$$ = ret;
				}

params:			TOK_IDENT
				{
					AstNode	ret		= createNode(TYPE_PARAMS, "", NODE_PARAMS_DEFAULT_SIZE),
							iden	= createNode(TYPE_IDENT, strdup($1), 0);

					nodeAddChild(&ret, iden);

					$$ = ALLOC;
					*$$ = ret;
				}

	|			params "," TOK_IDENT
				{
					AstNode	ret		= *$1,
							iden	= createNode(TYPE_IDENT, strdup($3), 0);
					
					nodeAddChild(&ret, iden);

					$$ = ALLOC;
					*$$ = ret;
				}

exprs:			expr
				{
					AstNode ret = createNode(TYPE_EXPRS, "", NODE_EXPRS_DEFAULT_SIZE);

					nodeAddChild(&ret, *$1);

					$$ = ALLOC;
					*$$ = ret;
				}
	 
	|			exprs "," expr
				{
					AstNode	ret	= *$1;

					nodeAddChild(&ret, *$3);

					$$ = ALLOC;
					*$$ = ret;
				}

funcCall:		TOK_IDENT "(" ")"
				{
					AstNode	ret = createNode(TYPE_FUNC_CALL, strdup($1), NODE_FUNC_CALL_SIZE), emptyNode = createNode(TYPE_EMPTY_NODE, "", 0);

					nodeAddChild(&ret, emptyNode);
					nodeAddChild(&ret, emptyNode);

					$$ = ALLOC;
					*$$ = ret;
				}

	|			TOK_IDENT "(" exprs ")"																
				{
					AstNode	ret = createNode(TYPE_FUNC_CALL, strdup($1), NODE_FUNC_CALL_SIZE);
					AstNode emptyNode = createNode(TYPE_EMPTY_NODE, "", 0);

					nodeAddChild(&ret, *$3);
					nodeAddChild(&ret, emptyNode);

					$$ = ALLOC;
					*$$ = ret;
				}

	|			scopeAccess funcCall
				{
					listAstNodeSetAt(&$2->childNodes, NODE_FUNC_CALL_ARG_SCOPE, *$1);

					$$ = ALLOC;
					*$$ = *$2;
				}

scopeAccess:	TOK_IDENT ":"
				{
					AstNode ret = createNode(TYPE_ACCESS_SCOPE, $1, NODE_ACCESS_SCOPE_SIZE);
					AstNode empty = createNode(TYPE_EMPTY_NODE, "", NODE_EMPTY_SIZE);
					
					nodeAddChild(&ret, empty);

					$$ = ALLOC;
					*$$ = ret;
				}

	|			"global" ":"
				{
					AstNode ret = createNode(TYPE_ACCESS_SCOPE, strdup("global"), NODE_ACCESS_SCOPE_SIZE);
					AstNode empty = createNode(TYPE_EMPTY_NODE, "", NODE_EMPTY_SIZE);
					
					nodeAddChild(&ret, empty);

					$$ = ALLOC;
					*$$ = ret;
				}

	|			scopeAccess ":" TOK_IDENT
				{
					AstNode ret = createNode(TYPE_ACCESS_SCOPE, strdup($3), NODE_ACCESS_SCOPE_SIZE);

					if (strcmp($1->sVal, "global") == 0)
					{
						fprintf(stderr, "ERROR: Unexpected \"global\" token.\n");
						YYABORT;
					}

					nodeAddChild(&ret, *$1);

					$$ = ALLOC;
					*$$ = ret;
				}

var:			TOK_IDENT
				{
					AstNode ret = createNode(TYPE_VAR, strdup($1), NODE_VAR_SIZE);
					AstNode emptyNode = createNode(TYPE_EMPTY_NODE, "", NODE_EMPTY_SIZE);
					
					nodeAddChild(&ret, emptyNode);

					$$ = ALLOC;
					*$$ = ret;
				}

	|			scopeAccess TOK_IDENT
				{
					AstNode ret = createNode(TYPE_VAR, strdup($2), NODE_VAR_SIZE);
					
					nodeAddChild(&ret, *$1);

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
	FILE *out = fopen("./out.spe", "wb");
	Compiler compiler = createCompiler();

	yyin = fopen(argv[1], "r");
	yyparse();

#ifdef DEBUG
	printf("Parse tree:\n");
	traverseTree(rootNode, 0);
#endif

	startCompile(&compiler, &rootNode);

#ifdef DEBUG
	printf("\nScopes:\n");
	DEBUG_compilerPrintScopes(&compiler);

	printf("\nBytecode:\n");
	DEBUG_compilerPrintBytecode(&compiler);
#endif

	compilerWriteToFile(&compiler, out);
	
	return 0;
}
