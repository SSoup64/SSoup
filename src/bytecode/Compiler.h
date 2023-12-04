#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "./Scope.h"
#include "./Bytecode.h"

typedef struct Compiler
{
	unsigned int bytecodeLength, bytecodeUsed;
	unsigned char *bytecode;

	Scope scope;
} Compiler;

Compiler createCompiler()
{
	Compiler ret =
	{
		1, 0,
		(unsigned char *) malloc(sizeof(unsigned char)),

		createScope(NULL, "root", SCOPE_ROOT)
	};

	return ret;
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

void compilerWriteToFile(Compiler *compiler, FILE *file)
{
	// TODO Add validation bytes 0x53 0x4F 0x55 0x50 (SOUP);
	
	for (int i = 0; i < compiler->bytecodeUsed; i++)
		putc(compiler->bytecode[i], file);

	putc((unsigned char) I_EXIT, file);
}


#ifdef DEBUG
void DEBUG_compilerPrintBytecode(Compiler *compiler)
{
	double fVal = 0;
	long bytesBuffer = 0;
	unsigned int address = 0;

	for (int i = 0; i < compiler->bytecodeUsed; i++)
	{
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
		}

		putchar('\n');
	}
}
#endif
