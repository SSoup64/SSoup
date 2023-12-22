#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>

#include "./Scope.h"
#include "./Bytecode.h"

typedef struct Compiler
{
	unsigned int bytecodeLength, bytecodeUsed;
	unsigned char *bytecode;

	unsigned int scopesLength, scopesUsed;
	Scope *scopes;

	Scope *scope;
} Compiler;

Compiler createCompiler()
{
	Compiler ret =
	{
		1, 0,
		(unsigned char *) malloc(sizeof(unsigned char)),
	};
	
	// Create the scopes array
	ret.scopesLength = 1;

	ret.scopes = (Scope *) malloc(ret.scopesLength * sizeof(Scope));
	
	// Create the root scope
	createScopeNull(&ret.scopes[0], "root", SCOPE_ROOT);
	
	// Set the current scope to be the root scope
	ret.scope = &ret.scopes[0];
	ret.scopesUsed = 1;

	return ret;
}

void compilerAppendScope(Compiler *compiler, char *scopeName, ScopeType type)
{
	unsigned int curScopeIndex = compiler->scope->scopeIndex;
	Scope newScope;

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

	// Append the new scope to the array of scopes
	if (compiler->scopesUsed >= compiler->scopesLength)
	{
		compiler->scopesLength += 8;
		compiler->scopes = (Scope *) realloc(compiler->scopes, compiler->scopesLength * sizeof(Scope));

		// The pointer compiler->scope needs to change it's value because realloc sometimes changes the address
		// This comment is here so my future self doesn't delete this code because I forgor about this
		// Damn, just gotta love C sometimes.
		compiler->scope = &compiler->scopes[curScopeIndex];
	}

	// Create the new scope
	createScope(&newScope, compiler->scopesUsed, compiler->scope->scopeIndex, strdup(scopeName), scopePath, type);
	
	// Append the new scope
	compiler->scopes[compiler->scopesUsed++] = newScope;
	compiler->scope = &compiler->scopes[compiler->scopesUsed - 1];
}

// TODO: Maybe make this an inline function at some later point for optimization
void compilerPopScope(Compiler *compiler)
{
	compiler->scope = &compiler->scopes[compiler->scope->prevScopeIndex];
}

void compilerAppendBytecode(Compiler *compiler, unsigned char bytecode)
{
	if (compiler->bytecodeUsed + 1 >= compiler->bytecodeLength)
	{
		compiler->bytecodeLength *= 2;
		compiler->bytecode = (unsigned char *) realloc(compiler->bytecode, compiler->bytecodeLength);
	}

	compiler->bytecode[compiler->bytecodeUsed++] = bytecode;
}

void compilerSetBytecodeAt(Compiler *compiler, unsigned char bytecode, unsigned int address)
{
	compiler->bytecode[address] = bytecode;
}

void compilerWriteToFile(Compiler *compiler, FILE *file)
{
	// Add validation bytes 0x53 0x4F 0x55 0x50 (SOUP);
	putc(0x53, file);
	putc(0x4F, file);
	putc(0x55, file);
	putc(0x50, file);
	
	for (int i = 0; i < compiler->bytecodeUsed; i++)
		putc(compiler->bytecode[i], file);
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
	scopeAddFuncIndex(&compiler->scopes[compiler->scope->prevScopeIndex], compiler->scope->scopeIndex);
}

unsigned int compilerFindFuncAddress(Compiler *compiler, Scope *curScope, char *name, unsigned int params)
{
	unsigned int funcIndex = 0;
	
	for (int i = 0; i < curScope->funcsOccupied; i++)
	{
		funcIndex = curScope->funcsIndices[i];

		if (SCOPE_FUNC == compiler->scopes[funcIndex].type && strcmp(compiler->scopes[funcIndex].scopeName, name) == 0 && params == compiler->scopes[funcIndex].func.paramsLen)
		{
			return compiler->scopes[funcIndex].func.address;
		}
	}

	// Set the current scope to the parent of the current scope
	if (curScope->scopeIndex != 0)
	{
		return compilerFindFuncAddress(compiler, &compiler->scopes[curScope->prevScopeIndex], strdup(name), params);
	}

	fprintf(stderr, "ERROR: Could not find function %s.\n", name);
	exit(1);
}

#ifdef DEBUG
void DEBUG_compilerPrintScopes(Compiler *compiler)
{
	for (int i = 0; i < compiler->scopesUsed; i++)
	{
		printf("%s%s", compiler->scopes[i].scopePath, compiler->scopes[i].scopeName);

		// If the scope is a function, then print the number of arguments it takes
		if (compiler->scopes[i].type == SCOPE_FUNC)
		{
			printf("(%u)", compiler->scopes[i].func.paramsLen);
		}

		putchar('\n');
	}
}

void DEBUG_compilerPrintBytecode(Compiler *compiler)
{
	double fVal = 0;
	long bytesBuffer = 0;
	unsigned int address = 0;

	for (int i = 0; i < compiler->bytecodeUsed; i++)
	{
		printf("%d\t", i);

		switch (compiler->bytecode[i])
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
					bytesBuffer += compiler->bytecode[j];
				}

				i += sizeof(long) - 1;

				fVal = *((double *) &bytesBuffer);

				printf("%lf", fVal);
				break;

			case (unsigned char) I_PUSH_STRING:
				printf("PUSH_STRING\t");

				while (compiler->bytecode[++i] != '\0')
					putchar(compiler->bytecode[i]);
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

			case (unsigned char) I_POP:
				printf("POP\t\t");

				address = 0;

				for (int j = ++i; j < i + sizeof(unsigned int); j++)
				{
					address <<= 8;
					address += compiler->bytecode[j];
				}

				i += sizeof(unsigned int) - 1;

				printf("%u", address);
				break;

			case (unsigned char) I_PUSH_MEM:
				printf("PUSH_MEM\t");

				address = 0;

				for (int j = ++i; j < i + sizeof(unsigned int); j++)
				{
					address <<= 8;
					address += compiler->bytecode[j];
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
					address += compiler->bytecode[j];
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
					address += compiler->bytecode[j];
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
