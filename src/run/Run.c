#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../bytecode/Bytecode.h"
#include "./Frame.h"
#include "./Stack.h"
#include "SoupObjVar.h"

#define ADVANCE thisChar = fgetc(code)

int main(int argc, char **argv)
{
	// Define variables
	FILE *code = NULL;
	unsigned char thisChar = ' ';

	int i = 0;

	double fVal = 0;
	long bytesBuffer = 0;

	unsigned int address = 0;

	unsigned int strBufferLen = 0;
	char *strBuffer = (char *) malloc(sizeof(char));

	SoupObjVar	objectBuffer = 
	{
		OBJ_TYPE_NONE,

		0,
		"",
	},
				oprandLeft =
	{
		OBJ_TYPE_NONE,

		0,
		"",
	},
				oprandRight =
	{
		OBJ_TYPE_NONE,

		0,
		"",
	};

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

				for (i = 0; i < sizeof(long); i++)
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
				// Free the memory in the string buffer
				free(strBuffer);
				strBufferLen = 0;

				// Find the length of the string
				while ((ADVANCE) != '\0')
				{
					strBufferLen++;
				}

				// Allocate memory
				strBuffer = (char *) malloc((strBufferLen + 1) * sizeof(char));

				// Go back and read the characters into strBuffer
				fseek(code, -((long) strBufferLen + 1), SEEK_CUR);
				
				for (i = 0; i < strBufferLen + 1; i++)
				{
					ADVANCE;
					strBuffer[i] = thisChar;
				}
				
				objectBuffer.type = OBJ_TYPE_STRING;
				objectBuffer.sVal = strdup(strBuffer);

				stackPush(&stack, objectBuffer);
				break;

			case (unsigned char) I_OP_PLUS:
				// Hard coded for now
				
				// Pop of the two top values from the stack
				oprandRight	= stackPop(&stack);
				oprandLeft	= stackPop(&stack);

				// Check if their type if the same, if not throw an error because casting doesn't exist yet
				if (oprandLeft.type != oprandRight.type)
				{
					fprintf(stderr, "ERROR: Tried to use the + operator on different types.");
					exit(1);
				}

				// Switch the type of the oprands
				objectBuffer.type = oprandLeft.type;
				switch (oprandLeft.type)
				{
					case OBJ_TYPE_FLOAT:
						objectBuffer.fVal = oprandLeft.fVal + oprandRight.fVal;
						break;

					case OBJ_TYPE_STRING:
						objectBuffer.sVal = (char *) malloc(sizeof(char) * (strlen(oprandLeft.sVal) + strlen(oprandRight.sVal) + 1));
						objectBuffer.sVal = strcat(oprandLeft.sVal, oprandRight.sVal);
						break;

					default:
						fprintf(stderr, "ERROR: Tried using + on an unsupported type.");
						exit(1);
						break;
				}
				
				stackPush(&stack, objectBuffer);
				break;

			case (unsigned char) I_OP_MINUS:
				// Hard coded for now
				
				// Pop of the two top values from the stack
				oprandRight	= stackPop(&stack);
				oprandLeft	= stackPop(&stack);

				// Check if their type if the same, if not throw an error because casting doesn't exist yet
				if (oprandLeft.type != oprandRight.type)
				{
					fprintf(stderr, "ERROR: Tried to use the - operator on different types.");
					exit(1);
				}

				// Switch the type of the oprands
				objectBuffer.type = oprandLeft.type;
				switch (oprandLeft.type)
				{
					case OBJ_TYPE_FLOAT:
						objectBuffer.fVal = oprandLeft.fVal - oprandRight.fVal;
						break;

					default:
						fprintf(stderr, "ERROR: Tried using - on an unsupported type.");
						exit(1);
						break;
				}

				stackPush(&stack, objectBuffer);
				break;

			case (unsigned char) I_OP_STAR:
				// Hard coded for now
				
				// Pop of the two top values from the stack
				oprandRight	= stackPop(&stack);
				oprandLeft	= stackPop(&stack);

				// Check if their type if the same, if not throw an error because casting doesn't exist yet
				if (oprandLeft.type != oprandRight.type)
				{
					fprintf(stderr, "ERROR: Tried to use the * operator on different types.");
					exit(1);
				}

				// Switch the type of the oprands
				objectBuffer.type = oprandLeft.type;
				switch (oprandLeft.type)
				{
					case OBJ_TYPE_FLOAT:
						objectBuffer.fVal = oprandLeft.fVal * oprandRight.fVal;
						break;

					default:
						fprintf(stderr, "ERROR: Tried using * on an unsupported type.");
						exit(1);
						break;
				}

				stackPush(&stack, objectBuffer);
				break;

			case (unsigned char) I_OP_SLASH:
				// Hard coded for now
				
				// Pop of the two top values from the stack
				oprandRight	= stackPop(&stack);
				oprandLeft	= stackPop(&stack);

				// Check if their type if the same, if not throw an error because casting doesn't exist yet
				if (oprandLeft.type != oprandRight.type)
				{
					fprintf(stderr, "ERROR: Tried to use the / operator on different types.");
					exit(1);
				}

				// Switch the type of the oprands
				objectBuffer.type = oprandLeft.type;
				switch (oprandLeft.type)
				{
					case OBJ_TYPE_FLOAT:
						objectBuffer.fVal = oprandLeft.fVal / oprandRight.fVal;
						break;

					default:
						fprintf(stderr, "ERROR: Tried using / on an unsupported type.");
						exit(1);
						break;
				}

				stackPush(&stack, objectBuffer);
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

				for (i = 0; i < sizeof(unsigned int); i++)
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

				for (i = 0; i < sizeof(unsigned int); i++)
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

			case (unsigned char) I_JMP:
				// Get the address
				address = 0;

				for (i = 0; i < sizeof(unsigned int); i++)
				{
					ADVANCE;

					address <<= 8;
					address += thisChar;
				}

				// Jump to the address
				fseek(code, (long) address, SEEK_SET);
				break;

			default:
				fprintf(stderr, "ERROR: Encountered an unknown instruction %X.\n", thisChar);
				break;
		}
	}
	return 0;
}
