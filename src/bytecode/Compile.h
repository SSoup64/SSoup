#include <stdio.h>

#include "../ast/AstNode.h"
#include "./Bytecode.h"
#include "./Compiler.h"

int compile(Compiler *compiler, AstNode *node)
{
	switch (node->type)
	{
		case TYPE_NUMBER:
			break;
	
		case TYPE_STR:
			break;

		case TYPE_BINOP:
			break;

		case TYPE_ROOT:
			for (int i = 0; i < node->childNodesOccupied; i++)
				compile(compiler, &node->childNodes[i]);
			break;

		case TYPE_IDENT:
			break;

		case TYPE_VAR_DECL:
			break;

		case TYPE_VAR_ASSIGN:
			break;

		case TYPE_DEBUG_PRINT:
			break;
	}
}
