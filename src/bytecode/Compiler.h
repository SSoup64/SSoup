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


#ifdef DEBUG
void DEBUG_compilerPrintBytecode(Compiler *compiler)
{
	double fVal = 0;
	long bytesBuffer = 0;

	for (int i = 0; i < compiler->bytecodeUsed; i++)
	{
		switch (compiler->bytecode[i])
		{
			case (unsigned char) I_NOP:
				printf("NOP    ");
				break;

			case (unsigned char) I_PUSH_DOUBLE:
				printf("PUSH_DOUBLE    ");

				i++;

				for (int j = i; j < i + sizeof(long); j++)
				{
					bytesBuffer <<= 8;
					bytesBuffer += compiler->bytecode[j];
				}

				i += sizeof(long) - 1;

				fVal = *((double *) &bytesBuffer);

				printf("%lf", fVal);
				break;
		}

		putchar('\n');
	}
}
#endif
