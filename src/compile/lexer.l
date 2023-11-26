%{
	#include <string.h>
	#include "./parser.tab.h"
%}

%%
"let"								{ return TOK_LET; }

\"([^"\\]|\\.)*\"					{ yylval.sVal = strdup(yytext); return TOK_STR; }
[0-9]+(\.([0-9])+)?					{ yylval.sVal = strdup(yytext); return TOK_NUMBER; }
[A-Za-z_][A-Za-z0-9_]*				{ yylval.sVal = strdup(yytext); return TOK_IDENT; }

"+"									{ return TOK_PLUS; }
"-"									{ return TOK_MINUS; }
"*"									{ return TOK_STAR; }
"/"									{ return TOK_SLASH; }

";"									{ return TOK_SEMICOLONS; }
%%

int yywrap()
{
	return 1;
}
