#pragma once

#include "./Compiler.h"
#include "Variable.h"

Compiler createCompiler()
{
	Scope *globalScope = (Scope *) malloc(sizeof(Scope));

	Compiler ret =
	{
		.bytecode = createListUchar(),
		.scopes = createListScope(),

		.scope = NULL,

		.globalVars = createListVariablePtr(),
		.variableBytecode = LOAD_TO_STACK,
	};
	
	// Create the global scope
	createScopeNull(globalScope, "global", SCOPE_ROOT);

	// Append the global scope to the list
	listScopeAppend(&ret.scopes, *globalScope);
	
	// Set the current scope to be the global scope
	ret.scope = listScopeGetPtrAt(&ret.scopes, 0);

	return ret;
}

void compilerAppendScope(Compiler *compiler, char *scopeName, ScopeType type)
{
	Scope *newScope = (Scope *) malloc(sizeof(Scope)); // TODO Find value to initialize this variable to

	/*
	TODO: Create function "compilerScopeExists"
	if (compilerScopeExists(strdup(scopeName), type))
	{
		Do something IDK what for now
		Probably if it is a function or class just return an error but if it a namespace then just expand onto the namespace
	}
	*/

	// Create the scope's name
	// The plus 2 is for the : and the null terminator
	char *scopePath = (char *) malloc(sizeof(char) * (strlen(compiler->scope->scopeName) + strlen(compiler->scope->scopePath) + 2));
	
	strcpy(scopePath, strdup(compiler->scope->scopePath));
	strcat(scopePath, strdup(compiler->scope->scopeName));
	strcat(scopePath, ":");

	// Create the new scope
	createScope(newScope, compiler->scopes.listLen, compiler->scope->scopeIndex, strdup(scopeName), scopePath, type);
	
	// Append the new scope
	listScopeAppend(&compiler->scopes, *newScope);

	compiler->scope = listScopeGetPtrAt(&compiler->scopes, compiler->scopes.listLen - 1);
}

// TODO: Maybe make this an inline function at some later point for optimization
void compilerPopScope(Compiler *compiler)
{
	// printf("PREVIOUS SCOPE INDEX: %s\n", listScopeGetPtrAt(&compiler->scopes, compiler->scope->prevScopeIndex)->scopeName);
	compiler->scope = listScopeGetPtrAt(&compiler->scopes, compiler->scope->prevScopeIndex);
}

void compilerAppendBytecode(Compiler *compiler, unsigned char bytecode)
{
	listUcharAppend(&compiler->bytecode, bytecode);
}

void compilerSetBytecodeAt(Compiler *compiler, unsigned char bytecode, unsigned int address)
{
	listUcharSetAt(&compiler->bytecode, address, bytecode);
}

void compilerWriteToFile(Compiler *compiler, FILE *file)
{
	int i = 0;

	// Add validation bytes 0x53 0x4F 0x55 0x50 (SOUP);
	putc(0x53, file);
	putc(0x4F, file);
	putc(0x55, file);
	putc(0x50, file);
	
	for (i = 0; i < compiler->bytecode.listLen; i++)
	{
		putc(listUcharGetAt(&compiler->bytecode, i), file);
	}
}

void compilerSetCurScopeFunc(Compiler *compiler, Func func)
{
	// Make sure that the current scope is infact a function scope
	if (compiler->scope->type != SCOPE_FUNC)
	{
		fprintf(stderr, "ERROR: Tried to add a func descriptor to a scope that is not of type SCOPE_FUNC.\n");
		exit(1);
	}
	
	// Set the scope's func attr to the func given
	compiler->scope->func = func;
	
	// Add this scope's index to its parent node's funcsIndices array
	scopeAddFuncIndex(listScopeGetPtrAt(&compiler->scopes, compiler->scope->prevScopeIndex), compiler->scope->scopeIndex);
}

unsigned int compilerFindFuncAddress(Compiler *compiler, Scope *curScope, char *name, unsigned int params)
{
	unsigned int funcIndex = 0;
	int i = 0;

	Scope *thisScope = NULL;
	
	for (i = 0; i < curScope->funcsIndices.listLen; i++)
	{
		funcIndex = listUintGetAt(&curScope->funcsIndices, i);
		thisScope = listScopeGetPtrAt(&compiler->scopes, funcIndex);

		if (SCOPE_FUNC == thisScope->type && strcmp(thisScope->scopeName, name) == 0 && params == thisScope->func.paramsLen)
		{
			return thisScope->func.address;
		}
	}

	// Set the current scope to the parent of the current scope
	if (curScope->scopeIndex != 0)
	{
		return compilerFindFuncAddress(compiler, listScopeGetPtrAt(&compiler->scopes, curScope->prevScopeIndex), strdup(name), params);
	}

	fprintf(stderr, "ERROR: Could not find function %s.\n", name);
	exit(1);
}

void compilerCreateVariable(Compiler *compiler, Scope *curScope, char *name)
{
	VariableType varType = VAR_TYPE_GLOBAL;
	Variable *var = NULL;

	// Check whether or not the variable is global
	switch (curScope->type)
	{
		case SCOPE_FUNC:
			varType = VAR_TYPE_LOCAL;
			break;

		case SCOPE_CLASS:
			varType = VAR_TYPE_ATTR;
			break;

		default:
			varType = VAR_TYPE_GLOBAL;
			break;
	}

	var = scopeAddVariable(curScope, strdup(name), varType);

	if (VAR_TYPE_GLOBAL == varType)
	{
		listVariablePtrAppend(&compiler->globalVars, var);
	}
}

Variable *compilerGetVariable(Compiler *compiler, char *name)
{
	Scope *curScope = compiler->scope;
	Variable *ret = scopeGetVariable(curScope, strdup(name));

	while (curScope->prevScopeIndex != curScope->scopeIndex && NULL == ret)
	{
		curScope = listScopeGetPtrAt(&compiler->scopes, curScope->prevScopeIndex);
		ret = scopeGetVariable(curScope, strdup(name));
	}

	if (NULL == ret)
	{
		fprintf(stderr, "Error: Refrence to unitialized variable %s.\n", name);
		exit(1);
	}

	return ret;
}

#ifdef DEBUG
void DEBUG_compilerPrintScopes(Compiler *compiler)
{
	int i = 0;
	Scope *thisScope = NULL;	

	for (i = 0; i < compiler->scopes.listLen; i++)
	{
		thisScope = listScopeGetPtrAt(&compiler->scopes, i);

		printf("%s%s", thisScope->scopePath, thisScope->scopeName);

		// If the scope is a function, then print the number of arguments it takes
		if (thisScope->type == SCOPE_FUNC)
		{
			printf("(%u)", thisScope->func.paramsLen);
		}

		putchar('\n');
	}
}

void DEBUG_compilerPrintBytecode(Compiler *compiler)
{
	double fVal = 0;
	long bytesBuffer = 0;
	unsigned int address = 0;

	int i = 0;

	for (i = 0; i < compiler->bytecode.listLen; i++)
	{
		printf("%d\t", i);

		switch (listUcharGetAt(&compiler->bytecode, i))
		{
			case (unsigned char) I_NOP:
				printf("NOP");
				break;

			case (unsigned char) I_PUSH_DOUBLE:
				printf("PUSH_DOUBLE\t");

				bytesBuffer = 0;

				for (int j = ++i; j < i + sizeof(long); j++)
				{
					bytesBuffer <<= 8;
					bytesBuffer += listUcharGetAt(&compiler->bytecode, j);
				}

				i += sizeof(long) - 1;

				fVal = *((double *) &bytesBuffer);

				printf("%lf", fVal);
				break;

			case (unsigned char) I_PUSH_STRING:
				printf("PUSH_STRING\t");

				while (listUcharGetAt(&compiler->bytecode, ++i) != '\0')
					putchar(listUcharGetAt(&compiler->bytecode, i));
				break;

			case (unsigned char) I_OP_PLUS:
				printf("OP_PLUS");
				break;

			case (unsigned char) I_OP_MINUS:
				printf("OP_MINUS");
				break;

			case (unsigned char) I_OP_STAR:
				printf("OP_STAR");
				break;

			case (unsigned char) I_OP_SLASH:
				printf("OP_SLASH");
				break;

			case (unsigned char) I_DEBUG_PRINT:
				printf("DEBUG_PRINT");
				break;

			case (unsigned char) I_POP_GLOBAL:
				printf("POP_GLOBAL\t");

				address = 0;

				for (int j = ++i; j < i + sizeof(unsigned int); j++)
				{
					address <<= 8;
					address += listUcharGetAt(&compiler->bytecode, j);
				}

				i += sizeof(unsigned int) - 1;

				printf("%u", address);
				break;

			case (unsigned char) I_POP_LOCAL:
				printf("POP_LOCAL\t");

				address = 0;

				for (int j = ++i; j < i + sizeof(unsigned int); j++)
				{
					address <<= 8;
					address += listUcharGetAt(&compiler->bytecode, j);
				}

				i += sizeof(unsigned int) - 1;

				printf("%u", address);
				break;

			case (unsigned char) I_PUSH_GLOBAL:
				printf("PUSH_GLOBAL\t");

				address = 0;

				for (int j = ++i; j < i + sizeof(unsigned int); j++)
				{
					address <<= 8;
					address += listUcharGetAt(&compiler->bytecode, j);
				}

				i += sizeof(unsigned int) - 1;

				printf("%u", address);
				break;

			case (unsigned char) I_PUSH_LOCAL:
				printf("PUSH_LOCAL\t");

				address = 0;

				for (int j = ++i; j < i + sizeof(unsigned int); j++)
				{
					address <<= 8;
					address += listUcharGetAt(&compiler->bytecode, j);
				}

				i += sizeof(unsigned int) - 1;

				printf("%u", address);
				break;

			case (unsigned char) I_EXIT:
				printf("EXIT");
				break;

			case (unsigned char) I_JMP:
				printf("JMP\t\t");

				address = 0;

				for (int j = ++i; j < i + sizeof(unsigned int); j++)
				{
					address <<= 8;
					address += listUcharGetAt(&compiler->bytecode, j);
				}

				i += sizeof(unsigned int) - 1;

				printf("%u", address);
				break;

			case (unsigned char) I_JMPF:
				printf("JMPF\t\t");

				address = 0;

				for (int j = ++i; j < i + sizeof(unsigned int); j++)
				{
					address <<= 8;
					address += listUcharGetAt(&compiler->bytecode, j);
				}

				i += sizeof(unsigned int) - 1;

				printf("%u", address);
				break;

			case (unsigned char) I_RETURN:
				printf("RETURN\t\t");
				break;

			case (unsigned char) I_NPL:
				printf("NPL");
				break;

			case (unsigned char) I_PL_APPEND:
				printf("PL_APPEND");
				break;
		}

		putchar('\n');
	}
}
#endif
