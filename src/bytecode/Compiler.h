#include <stdlib.h>

typedef struct Compiler
{
	unsigned int bytecodeLength, bytecodeUsed;
	unsigned char *bytecode;
} Compiler;

Compiler createCompiler()
{
	Compiler ret =
	{
		1, 0,
		(unsigned char *) malloc(sizeof(unsigned char)),
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
