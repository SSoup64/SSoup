%{
	#include <string.h>
	#include "./parser.tab.h"
%}

%%
"let"								{ return TOK_LET; }

[A-Za-z_][A-Za-z0-9_]				{ yylval.sVal = strdup(yytext); return TOK_IDENT; }
\"([^"\\]|\\.)*\"					{ yylval.sVal = strdup(yytext); return TOK_STR; }
[0-9]+(\.([0-9])+)?					{ yylval.sVal = strdup(yytext); return TOK_NUMBER; }

";"									{ return TOK_SEMICOLONS; }
%%

int yywrap()
{
	return 1;
}
