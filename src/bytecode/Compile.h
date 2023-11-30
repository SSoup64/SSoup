#include <stdio.h>
#include <stdlib.h>

#include "../ast/AstNode.h"
#include "./Bytecode.h"
#include "./Compiler.h"

int compile(Compiler *compiler, AstNode *node)
{
	// Define variables
	long doubleBytes = 0;
	double numberNodeVal = 0.0;

	switch (node->type)
	{
		case TYPE_NUMBER:
			// TODO: Check whether the number is a double or an int
			
			// Add I_PUSH_FLOAT bytecode
			compilerAppendBytecode(compiler, I_PUSH_DOUBLE);

			// Turn the sVal of the node to a double
			numberNodeVal = strtod(node->sVal, NULL);
			
			// Gets the bytes of the double
			doubleBytes = *((long *) &numberNodeVal);

			// Add the bytes to the bytecode
			for (int i = sizeof(long) - 1; i >= 0; i--)
				compilerAppendBytecode(compiler, (unsigned char) (doubleBytes << 8 * i));
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
			compilerAppendBytecode(compiler, I_DEBUG_PRINT);
			break;
	}
}
