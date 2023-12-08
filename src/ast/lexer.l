%{
	#include <string.h>
	#include "./parser.tab.h"
%}

%%
"let"								{ return TOK_LET; }
"func"								{ return TOK_FUNC; }
"print"								{ return TOK_DEBUG_PRINT; }

\"([^"\\]|\\.)*\"					{	yylval.sVal = strdup(yytext); 

										yylval.sVal[strlen(yylval.sVal) - 1] = '\0'; 
										yylval.sVal++;

										return TOK_STR;
									}
[0-9]+(\.([0-9])+)?					{ yylval.sVal = strdup(yytext); return TOK_NUMBER; }
[A-Za-z_][A-Za-z0-9_]*				{ yylval.sVal = strdup(yytext); return TOK_IDENT; }

"("									{ return TOK_LPAREN; }
")"									{ return TOK_RPAREN; }

"\{"								{ return TOK_LCPAREN; }
"\}"								{ return TOK_RCPAREN; }

"+"									{ return TOK_PLUS; }
"-"									{ return TOK_MINUS; }
"*"									{ return TOK_STAR; }
"/"									{ return TOK_SLASH; }

"="									{ return TOK_EQ; }

";"									{ return TOK_SEMICOLONS; }

","									{ return TOK_COMMA; }

[\t\n]								;
" "									;
.									printf("Unexpected character %s\n", yytext);
%%

int yywrap()
{
// <<EOF>>								{ return TOK_EOF; }
	return 1;
}
