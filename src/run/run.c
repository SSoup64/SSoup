#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../bytecode/Bytecode.h"

#define ADVANCE thisChar = (unsigned char) getc(code)

int main(int argc, char **argv)
{
	// Define variables
	double fVal = 0;
	long bytesBuffer = 0;
	unsigned int address = 0;

	unsigned char thisChar = ' ';
	
	FILE *code = NULL;

	// Open the file
	assert(argc >= 2);
	code = fopen(argv[1], "rb");

	while ((ADVANCE) != (unsigned char) I_EXIT)
	{
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
				break;

			case (unsigned char) I_PUSH_STRING:
				while ((ADVANCE) != '\0')
					putchar(thisChar);
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
				break;

			case (unsigned char) I_POP:
				address = 0;

				for (int i = 0; i < sizeof(unsigned int); i++)
				{
					ADVANCE;

					address <<= 8;
					address += thisChar;
				}
				break;

			case (unsigned char) I_PUSH_MEM:
				address = 0;

				for (int i = 0; i < sizeof(unsigned int); i++)
				{
					ADVANCE;

					address <<= 8;
					address += thisChar;
				}
				break;

			default:
				fprintf(stderr, "ERROR: Encountered an unknown instruction.");
				break;
		}
	}
	return 0;
}
