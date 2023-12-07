#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../bytecode/Bytecode.h"
#include "./Frame.h"
#include "./Stack.h"
#include "SoupObjVar.h"

#define ADVANCE thisChar = fgetc(code)

int main(int argc, char **argv)
{
	// Define variables
	FILE *code = NULL;

	double fVal = 0;
	long bytesBuffer = 0;

	SoupObjVar objectBuffer = 
	{
		OBJ_TYPE_NONE,

		0,
		"",
	};

	unsigned int address = 0;

	unsigned char thisChar = ' ';

	Frame frame = createFrame(NULL);
	Stack stack = createStack();

	// Open the file
	assert(argc >= 2);
	code = fopen(argv[1], "rb");
	
	for (;;)
	{
		ADVANCE;

		switch (thisChar)
		{
			case (unsigned char) I_NOP:
				break;

			case (unsigned char) I_PUSH_DOUBLE:
				bytesBuffer = 0;

				for (int i = 0; i < sizeof(long); i++)
				{
					ADVANCE;

					bytesBuffer <<= 8;
					bytesBuffer += thisChar;
				}

				fVal = *((double *) &bytesBuffer);
				
				objectBuffer.type = OBJ_TYPE_FLOAT;
				objectBuffer.fVal = fVal;

				stackPush(&stack, objectBuffer);
				break;

			case (unsigned char) I_PUSH_STRING:
				while ((ADVANCE) != '\0')
				{
					putchar(thisChar);
				}
				break;

			case (unsigned char) I_OP_PLUS:
				break;

			case (unsigned char) I_OP_MINUS:
				break;

			case (unsigned char) I_OP_STAR:
				break;

			case (unsigned char) I_OP_SLASH:
				break;

			case (unsigned char) I_DEBUG_PRINT:
				objectBuffer = stackPop(&stack);

				switch (objectBuffer.type)
				{
					case OBJ_TYPE_NONE:
						break;

					case OBJ_TYPE_FLOAT:
						printf("%lf\n", objectBuffer.fVal);
						break;

					case OBJ_TYPE_STRING:
						printf("%s\n", objectBuffer.sVal);
						break;
				}
				break;

			case (unsigned char) I_POP:
				
				address = 0;

				for (int i = 0; i < sizeof(unsigned int); i++)
				{
					ADVANCE;

					address <<= 8;
					address += thisChar;
				}

				objectBuffer = stackPop(&stack);

				frameSetObjAt(&frame, address, objectBuffer);
				break;

			case (unsigned char) I_PUSH_MEM:
				address = 0;

				for (int i = 0; i < sizeof(unsigned int); i++)
				{
					ADVANCE;

					address <<= 8;
					address += thisChar;
				}

				objectBuffer = frameGetObjAt(&frame, address);
				
				stackPush(&stack, objectBuffer);
				break;

			case (unsigned char) I_EXIT:
				exit(0);
				break;

			default:
				fprintf(stderr, "ERROR: Encountered an unknown instruction %X.\n", thisChar);
				break;
		}
	}
	return 0;
}
