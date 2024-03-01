#pragma once

#include "./Compile.h"

void compile(Compiler *compiler, AstNode *node)
{
	// Define variables
	long doubleBytes = 0;
	double numberNodeVal = 0.0;

	unsigned int address = 0; // Mainly for variables and funcitons

	int i = 0; // For indexing like in for loops

	Variable *varPointer = NULL;

	switch (node->type)
	{
		case TYPE_EMPTY_NODE:
			break;

		case TYPE_NUMBER:
			// TODO: Check whether the number is a double or an int

			// Add I_PUSH_FLOAT bytecode
			compilerAppendBytecode(compiler, I_PUSH_DOUBLE);

			// Turn the sVal of the node to a double
			numberNodeVal = strtod(node->sVal, NULL);

			// Gets the bytes of the double
			doubleBytes = *((long *) &numberNodeVal);

			// Add the bytes to the bytecode
			for (i = sizeof(long) - 1; i >= 0; i--)
			{
				compilerAppendBytecode(compiler, (unsigned char) (doubleBytes >> BYTE_SIZE_IN_BITS * i));
			}
			break;

		case TYPE_STR:
			// Add I_PUSH_STR bytecode
			compilerAppendBytecode(compiler, I_PUSH_STRING);

			// Add the string itself to the bytecode
			i = 0;

			while (node->sVal[i] != '\0')
			{
				compilerAppendBytecode(compiler, node->sVal[i++]);
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
			// Also I just wanted to say that this code is really really REALLY fucking bad but I'll improve it later on because there are just 4 operators
			if (strcmp(node->sVal, "+") == 0)
			{
				compilerAppendBytecode(compiler, I_OP_PLUS);
			}
			else if (strcmp(node->sVal, "-") == 0)
			{
				compilerAppendBytecode(compiler, I_OP_MINUS);
			}
			else if (strcmp(node->sVal, "*") == 0)
			{
				compilerAppendBytecode(compiler, I_OP_STAR);
			}
			else if (strcmp(node->sVal, "/") == 0)
			{
				compilerAppendBytecode(compiler, I_OP_SLASH);
			}
			break;

		case TYPE_SCOPE:
			for (i = 0; i < node->childNodesOccupied; i++)
			{
				compile(compiler, &node->childNodes[i]);
			}
			break;

		case TYPE_IDENT:
			// Get the variable
			varPointer = compilerGetVariable(compiler, node->sVal);
			address = varPointer->address;

			if (LOAD_TO_STACK == compiler->variableBytecode)
			{
				switch (varPointer->type)
				{
					case VAR_TYPE_GLOBAL:
						compilerAppendBytecode(compiler, (unsigned char) I_PUSH_GLOBAL);
						break;

					case VAR_TYPE_LOCAL:
						compilerAppendBytecode(compiler, (unsigned char) I_PUSH_LOCAL);
						break;

					default:
						fprintf(stderr, "ERROR: The variable %s is illegal.", varPointer->name);
						break;
				}
			}
			else
			{
				switch (varPointer->type)
				{
					case VAR_TYPE_GLOBAL:
						compilerAppendBytecode(compiler, (unsigned char) I_POP_GLOBAL);
						break;

					case VAR_TYPE_LOCAL:
						compilerAppendBytecode(compiler, (unsigned char) I_POP_LOCAL);
						break;

					default:
						fprintf(stderr, "ERROR: The variable %s is illegal.", varPointer->name);
						break;
				}
			}

			// Add the address of the variable to the bytecode
			for (i = sizeof(unsigned int) - 1; i >= 0; i--)
			{
				compilerAppendBytecode(compiler, (unsigned char) (address >> BYTE_SIZE_IN_BITS * i));
			}
			break;

		case TYPE_VAR_DECL:
			// Add the variable to the scope
			compilerCreateVariable(compiler, compiler->scope, node->childNodes[0].sVal);
			break;

		case TYPE_VAR_ASSIGN:
			// Compile the RHS of the assignment
			compile(compiler, &node->childNodes[1]);
			
			// Make the compiler save values to identifiers and not load them onto the stack
			compiler->variableBytecode = SAVE_TO_VAR;
		
			// Compile the identifier
			compile(compiler, &node->childNodes[0]);
			
			// Make the compiler load identifiers to the stack.
			compiler->variableBytecode = LOAD_TO_STACK;
			break;

		case TYPE_DEBUG_PRINT:
			// Compile the expression inside the print
			compile(compiler, &node->childNodes[0]);

			// Add I_DEBUG_PRINT bytecode
			compilerAppendBytecode(compiler, I_DEBUG_PRINT);
			break;
		
		case TYPE_FUNC_DECL:
			// Create the new scope
			if (compiler->scope->type == SCOPE_FUNC)
			{
				// Actually, idk if we want that to not be possible.
				// It could lead to some good solutions for repetative code that only occurs in one other function.
				// But I am getting ahead of myself.
				// That may come later down the road.
				fprintf(stderr, "ERROR: Cannot create a function inside a function.\n");
				exit(1);
			}

			// Create the name for the new scope
			compilerAppendScope(compiler, strdup(node->sVal), SCOPE_FUNC);

			// JMP statement so the code doesn't run when we start the program
			// compilerAppendBytecode(compiler, (unsigned char) I_JMP);
			
			// Save the current length of the bytecode so we could rewrite it later
			address = compiler->bytecodeUsed;
			
			/*
			for (i = 0; i < sizeof(unsigned int); i++)
			{
				compilerAppendBytecode(compiler, 0);
			}
			*/

			// Set the scopes current function
			compilerSetCurScopeFunc(compiler, createFunc(compiler->bytecodeUsed, node->childNodes[0].childNodesOccupied));

			// Compile the parameter list
			compile(compiler, &node->childNodes[0]);

			// Compile the code
			compile(compiler, &node->childNodes[1]);

			// Add a default return
			compilerAppendBytecode(compiler, (unsigned char) I_RETURN);

			/*
			// Set the address to jump to
			for (i = 0; i < sizeof(unsigned int); i++)
			{
				compilerSetBytecodeAt(compiler, compiler->bytecodeUsed >> (BYTE_SIZE_IN_BITS * i), address + sizeof(unsigned int) - i - 1);
			}
			*/

			compilerPopScope(compiler);
			break;

		case TYPE_PARAMS:
			for (i = 0; i < node->childNodesOccupied; i++)
			{
				compilerCreateVariable(compiler, compiler->scope, strdup(node->childNodes[i].sVal));
			}
			break;

		case TYPE_FUNC_CALL:
			// Find the address of the function
			address = compilerFindFuncAddress(compiler, compiler->scope, strdup(node->sVal), node->childNodes[0].childNodesOccupied);

			// Create a new parameter list
			compilerAppendBytecode(compiler, (unsigned char) I_NPL);

			// Compile the expressions
			compile(compiler, &node->childNodes[0]);

			// Add a JMPF instruction
			compilerAppendBytecode(compiler, (unsigned char) I_JMPF);

			// Add the address of the function to the bytecode
			for (i = sizeof(unsigned int) - 1; i >= 0; i--)
			{
				compilerAppendBytecode(compiler, (unsigned char) (address >> BYTE_SIZE_IN_BITS * i));
			}
			break;

		case TYPE_EXPRS:
			// TODO: Make the expressions compile differently depending whether they are compiled for function calls or something else.
			for (i = 0; i < node->childNodesOccupied; i++)
			{
				compile(compiler, &node->childNodes[i]);
				compilerAppendBytecode(compiler, (unsigned char) I_PL_APPEND);
			}
			break;

		case TYPE_RETURN:
			if (compiler->scope->type != SCOPE_FUNC)
			{
				fprintf(stderr, "ERROR: Tried using the 'return' keyword outside of a function scope.\n");
				exit(1);
			}

			compile(compiler, &node->childNodes[0]);
			compilerAppendBytecode(compiler, (unsigned char) I_RETURN);
			break;

		case TYPE_VAR:
			// TODO: Add scopes
			// Get the variable
			varPointer = compilerGetVariable(compiler, node->sVal);
			address = varPointer->address;

			if (LOAD_TO_STACK == compiler->variableBytecode)
			{
				switch (varPointer->type)
				{
					case VAR_TYPE_GLOBAL:
						compilerAppendBytecode(compiler, (unsigned char) I_PUSH_GLOBAL);
						break;

					case VAR_TYPE_LOCAL:
						compilerAppendBytecode(compiler, (unsigned char) I_PUSH_LOCAL);
						break;

					default:
						fprintf(stderr, "ERROR: The variable %s is illegal.", varPointer->name);
						break;
				}
			}
			else
			{
				switch (varPointer->type)
				{
					case VAR_TYPE_GLOBAL:
						compilerAppendBytecode(compiler, (unsigned char) I_POP_GLOBAL);
						break;

					case VAR_TYPE_LOCAL:
						compilerAppendBytecode(compiler, (unsigned char) I_POP_LOCAL);
						break;

					default:
						fprintf(stderr, "ERROR: The variable %s is illegal.", varPointer->name);
						break;
				}
			}

			// Add the address of the variable to the bytecode
			for (i = sizeof(unsigned int) - 1; i >= 0; i--)
			{
				compilerAppendBytecode(compiler, (unsigned char) (address >> BYTE_SIZE_IN_BITS * i));
			}
			break;

		default:
			break;
	}
}

void startCompile(Compiler *compiler, AstNode *node)
{
	int i = 0;
	unsigned int mainFuncAddress = 0, mainCallAddress = 0;
	unsigned int jumpToExitAddress = 0;

	// Add the NPL and JMPF instructions for the main function
	compilerAppendBytecode(compiler, (unsigned char) I_NPL);
	compilerAppendBytecode(compiler, (unsigned char) I_JMPF);
	
	mainCallAddress = compiler->bytecodeUsed;

	// Write a placeholder value for the main func address
	for (i = sizeof(unsigned int) - 1; i >= 0; i--)
	{
		compilerAppendBytecode(compiler, (unsigned char) (mainFuncAddress >> BYTE_SIZE_IN_BITS * i));
	}

	// Add jump to the exit
	compilerAppendBytecode(compiler, (unsigned char) I_JMP);

	jumpToExitAddress = compiler->bytecodeUsed;

	// Write a placeholder value for the exit instruction
	for (i = sizeof(unsigned int) - 1; i >= 0; i--)
	{
		compilerAppendBytecode(compiler, (unsigned char) (mainFuncAddress >> BYTE_SIZE_IN_BITS * i));
	}
	
	// Compile thhe AST
	compile(compiler, node);
	
	// Find the address of the main function and add it to the bytecode
	mainFuncAddress = compilerFindFuncAddress(compiler, &compiler->scopes[0], strdup("main"), 0);
	
	for (i = 0; i < sizeof(unsigned int); i++)
	{
		compilerSetBytecodeAt(compiler, mainFuncAddress >> (BYTE_SIZE_IN_BITS * i), mainCallAddress + sizeof(unsigned int) - i - 1);
	}
	
	// Add an EXIT instruction just in case.
	compilerAppendBytecode(compiler, (unsigned char) I_EXIT);

	// Write the address of theh exit instruction to the bytecode
	for (i = 0; i < sizeof(unsigned int); i++)
	{
		compilerSetBytecodeAt(compiler, (compiler->bytecodeUsed - 1) >> (BYTE_SIZE_IN_BITS * i), jumpToExitAddress + sizeof(unsigned int) - i - 1);
	}
}
