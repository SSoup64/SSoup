#include <string.h>
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
	int i; // For indexing like in for loops
	unsigned int address; // Mainly for variables and funcitons

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
				compilerAppendBytecode(compiler, (unsigned char) (doubleBytes >> 8 * i));
			break;
	
		case TYPE_STR:
			// Add I_PUSH_STR bytecode
			compilerAppendBytecode(compiler, I_PUSH_STRING);

			// Add the string itself to the bytecode
			i = 0;

			while (node->sVal[i] != '\0')
			{
				compilerAppendBytecode(compiler, node->sVal[i]);
				i++;
			}
			
			// Add the null terminator
			compilerAppendBytecode(compiler, '\0');
			break;

		case TYPE_BINOP:
			// Compile the left side
			compile(compiler, &node->childNodes[0]);

			// Compile the right side
			compile(compiler, &node->childNodes[1]);

			// Add the binop to the bytecode
			if (strcmp(node->sVal, "+") == 0)
				compilerAppendBytecode(compiler, I_OP_PLUS);
			else if (strcmp(node->sVal, "-") == 0)
				compilerAppendBytecode(compiler, I_OP_MINUS);
			else if (strcmp(node->sVal, "*") == 0)
				compilerAppendBytecode(compiler, I_OP_STAR);
			else if (strcmp(node->sVal, "/") == 0)
				compilerAppendBytecode(compiler, I_OP_SLASH);
			break;

		case TYPE_ROOT:
			for (int i = 0; i < node->childNodesOccupied; i++)
				compile(compiler, &node->childNodes[i]);
			break;

		case TYPE_IDENT:
			// Add I_PUSH_MEM bytecode
			compilerAppendBytecode(compiler, (unsigned char) I_PUSH_MEM);
			
			// Get the address of the variable
			address = scopeGetVariable(&compiler->scope, node->sVal);

			// Add the address of the variable to the bytecode
			for (int i = sizeof(unsigned int) - 1; i >= 0; i--)
				compilerAppendBytecode(compiler, (unsigned char) (address >> 8 * i));
			break;

		case TYPE_VAR_DECL:
			// Add the variable to the scope
			scopeAddVariable(&compiler->scope, node->childNodes[0].sVal);
			break;

		case TYPE_VAR_ASSIGN:
			// Compile the RHS of the assignment
			compile(compiler, &node->childNodes[1]);
			
			// Add I_POP bytecode
			compilerAppendBytecode(compiler, (unsigned char) I_POP);

			// Get the address of the variable to save to
			address = scopeGetVariable(&compiler->scope, node->childNodes[0].sVal);

			// Add the address of the variable to the bytecode
			for (int i = sizeof(unsigned int) - 1; i >= 0; i--)
				compilerAppendBytecode(compiler, (unsigned char) (address >> 8 * i));
			break;

		case TYPE_DEBUG_PRINT:
			// Compile the expression inside the print
			compile(compiler, &node->childNodes[0]);

			// Add I_DEBUG_PRINT bytecode
			compilerAppendBytecode(compiler, I_DEBUG_PRINT);
			break;
	}
}
