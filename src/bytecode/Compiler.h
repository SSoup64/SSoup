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
	
	// Create the root scope
	
	// Create the scopes array
	ret.scopesLength = 1;

	ret.scopes = (Scope *) malloc(ret.scopesLength * sizeof(Scope));
	createScopeNull(&ret.scopes[0], "root", SCOPE_ROOT);


	// For some reason, whenever we pop the scopes stack and return to the root, for some reason, the name of 

	ret.scopesUsed = 1;

	ret.scope = &ret.scopes[0];

	// printf("%u", ret.scope.variablesLength);

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
	char *newScopeName = (char *) malloc(sizeof(char) * (strlen(compiler->scope->scopeName) + strlen(scopeName) + 2)); // The plus 2 is for the | and the null terminator
	
	strcpy(newScopeName, strdup(compiler->scope->scopeName));
	strcat(newScopeName, "|");
	strcat(newScopeName, strdup(scopeName));

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
	createScope(&newScope, compiler->scopesUsed, compiler->scope->scopeIndex, newScopeName, type);
	
	// Append the new scope
	compiler->scopes[compiler->scopesUsed++] = newScope;
	compiler->scope = &compiler->scopes[compiler->scopesUsed - 1];
}

// TODO: Maybe make this an inline function at some later point for optimization
void compilerPopScope(Compiler *compiler)
{
	printf("%s, %d\n", compiler->scope->scopeName, compiler->scope->prevScopeIndex);
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
	// TODO Add validation bytes 0x53 0x4F 0x55 0x50 (SOUP);
	
	for (int i = 0; i < compiler->bytecodeUsed; i++)
		putc(compiler->bytecode[i], file);
}

#ifdef DEBUG
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
		}

		putchar('\n');
	}
}
#endif
