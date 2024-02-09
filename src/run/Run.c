#pragma once

#include "./Run.h"

/*
The function that runs the entire bytecode.
Input: The file to read the bytecode form.
Output: true if successful, false if not.
*/
bool run(char *file)
{
	// Define variables
	VirtualMachine machine = createVirtualMachine(file);

	int i = 0;

	double fVal = 0;
	long bytesBuffer = 0;

	unsigned int address = 0;

	unsigned int strBufferLen = 0;
	char *strBuffer = (char *) malloc(sizeof(char));

	bool halt = false;
	bool error = false;

	// Read the validation bytes
	if (!virtualMachineValidateBytes(&machine))
	{
		fprintf(stderr, "ERROR: Tried to run a file which is not in the correct format.\n");
		halt = true;
		error = true;
	}
	
	// Run the code
	while (!halt)
	{
		virtualMachineAdvance(&machine);

		switch (machine.thisChar)
		{
			case (unsigned char) I_NOP:
				break;

			case (unsigned char) I_PUSH_DOUBLE:
				bytesBuffer = 0;

				for (i = 0; i < sizeof(long); i++)
				{
					virtualMachineAdvance(&machine);

					bytesBuffer <<= BYTE_SIZE_IN_BITS;
					bytesBuffer += machine.thisChar;
				}

				fVal = *((double *) &bytesBuffer);
				
				machine.objectBuffer.type = OBJ_TYPE_FLOAT;
				machine.objectBuffer.fVal = fVal;

				stackPush(&machine.stack, machine.objectBuffer);
				break;

			case (unsigned char) I_PUSH_STRING:
				// Free the memory in the string buffer
				free(strBuffer);
				strBufferLen = 0;

				// Find the length of the string
				while ((virtualMachineAdvance(&machine)) != '\0')
				{
					strBufferLen++;
				}

				// Allocate memory
				strBuffer = (char *) malloc((strBufferLen + 1) * sizeof(char));

				// Go back and read the characters into strBuffer
				fseek(machine.code, -((long) strBufferLen + 1), SEEK_CUR);
				
				for (i = 0; i < strBufferLen + 1; i++)
				{
					virtualMachineAdvance(&machine);
					strBuffer[i] = machine.thisChar;
				}
				
				machine.objectBuffer.type = OBJ_TYPE_STRING;
				machine.objectBuffer.sVal = strdup(strBuffer);

				stackPush(&machine.stack, machine.objectBuffer);
				break;

			case (unsigned char) I_OP_PLUS:
				// Hard coded for now
				
				// Pop of the two top values from the stack
				machine.oprandRight	= stackPop(&machine.stack);
				machine.oprandLeft	= stackPop(&machine.stack);

				// Check if their type if the same, if not throw an error because casting doesn't exist yet
				if (machine.oprandLeft.type != machine.oprandRight.type)
				{
					fprintf(stderr, "ERROR: Tried to use the + operator on different types.");
					halt = true;
					error = true;
				}

				// Switch the type of the oprands
				machine.objectBuffer.type = machine.oprandLeft.type;
				switch (machine.oprandLeft.type)
				{
					case OBJ_TYPE_FLOAT:
						machine.objectBuffer.fVal = machine.oprandLeft.fVal + machine.oprandRight.fVal;
						break;

					case OBJ_TYPE_STRING:
						machine.objectBuffer.sVal = (char *) malloc(sizeof(char) * (strlen(machine.oprandLeft.sVal) + strlen(machine.oprandRight.sVal) + 1));
						machine.objectBuffer.sVal = strcat(machine.oprandLeft.sVal, machine.oprandRight.sVal);
						break;

					default:
						fprintf(stderr, "ERROR: Tried using + on an unsupported type.\n");
						halt = true;
						error = true;
						break;
				}
				
				stackPush(&machine.stack, machine.objectBuffer);
				break;

			case (unsigned char) I_OP_MINUS:
				// Hard coded for now
				
				// Pop of the two top values from the stack
				machine.oprandRight	= stackPop(&machine.stack);
				machine.oprandLeft	= stackPop(&machine.stack);

				// Check if their type if the same, if not throw an error because casting doesn't exist yet
				if (machine.oprandLeft.type != machine.oprandRight.type)
				{
					fprintf(stderr, "ERROR: Tried to use the - operator on different types.\n");
					halt = true;
					error = true;
				}

				// Switch the type of the oprands
				machine.objectBuffer.type = machine.oprandLeft.type;
				switch (machine.oprandLeft.type)
				{
					case OBJ_TYPE_FLOAT:
						machine.objectBuffer.fVal = machine.oprandLeft.fVal - machine.oprandRight.fVal;
						break;

					default:
						fprintf(stderr, "ERROR: Tried using - on an unsupported type.\n");
						halt = true;
						error = true;
						break;
				}

				stackPush(&machine.stack, machine.objectBuffer);
				break;

			case (unsigned char) I_OP_STAR:
				// Hard coded for now
				
				// Pop of the two top values from the stack
				machine.oprandRight	= stackPop(&machine.stack);
				machine.oprandLeft	= stackPop(&machine.stack);

				// Check if their type if the same, if not throw an error because casting doesn't exist yet
				if (machine.oprandLeft.type != machine.oprandRight.type)
				{
					fprintf(stderr, "ERROR: Tried to use the * operator on different types.\n");
					halt = true;
					error = true;
				}

				// Switch the type of the oprands
				machine.objectBuffer.type = machine.oprandLeft.type;
				switch (machine.oprandLeft.type)
				{
					case OBJ_TYPE_FLOAT:
						machine.objectBuffer.fVal = machine.oprandLeft.fVal * machine.oprandRight.fVal;
						break;

					default:
						fprintf(stderr, "ERROR: Tried using * on an unsupported type.\n");
						halt = true;
						error = true;
						break;
				}

				stackPush(&machine.stack, machine.objectBuffer);
				break;

			case (unsigned char) I_OP_SLASH:
				// Hard coded for now
				
				// Pop of the two top values from the stack
				machine.oprandRight	= stackPop(&machine.stack);
				machine.oprandLeft	= stackPop(&machine.stack);

				// Check if their type if the same, if not throw an error because casting doesn't exist yet
				if (machine.oprandLeft.type != machine.oprandRight.type)
				{
					fprintf(stderr, "ERROR: Tried to use the / operator on different types.\n");
					halt = true;
					error = true;
				}

				// Switch the type of the oprands
				machine.objectBuffer.type = machine.oprandLeft.type;
				switch (machine.oprandLeft.type)
				{
					case OBJ_TYPE_FLOAT:
						machine.objectBuffer.fVal = machine.oprandLeft.fVal / machine.oprandRight.fVal;
						break;

					default:
						fprintf(stderr, "ERROR: Tried using / on an unsupported type.\n");
						halt = true;
						error = true;
						break;
				}

				stackPush(&machine.stack, machine.objectBuffer);
				break;

			case (unsigned char) I_DEBUG_PRINT:
				machine.objectBuffer = stackPop(&machine.stack);

				switch (machine.objectBuffer.type)
				{
					case OBJ_TYPE_NONE:
						break;

					case OBJ_TYPE_FLOAT:
						printf("%lf\n", machine.objectBuffer.fVal);
						break;

					case OBJ_TYPE_STRING:
						printf("%s\n", machine.objectBuffer.sVal);
						break;
				}

				break;

			case (unsigned char) I_POP_LOCAL:
				address = 0;

				for (i = 0; i < sizeof(unsigned int); i++)
				{
					virtualMachineAdvance(&machine);

					address <<= BYTE_SIZE_IN_BITS;
					address += machine.thisChar;
				}

				machine.objectBuffer = stackPop(&machine.stack);

				frameSetObjAt(machine.frame, address, machine.objectBuffer);
				break;

			case (unsigned char) I_POP_GLOBAL:
				address = 0;

				for (i = 0; i < sizeof(unsigned int); i++)
				{
					virtualMachineAdvance(&machine);

					address <<= BYTE_SIZE_IN_BITS;
					address += machine.thisChar;
				}

				machine.objectBuffer = stackPop(&machine.stack);

				frameSetObjAt(machine.globalFrame, address, machine.objectBuffer);
				break;

			case (unsigned char) I_PUSH_LOCAL:
				address = 0;

				for (i = 0; i < sizeof(unsigned int); i++)
				{
					virtualMachineAdvance(&machine);

					address <<= BYTE_SIZE_IN_BITS;
					address += machine.thisChar;
				}

				machine.objectBuffer = frameGetObjAt(machine.frame, address);
				
				stackPush(&machine.stack, machine.objectBuffer);
				break;

			case (unsigned char) I_PUSH_GLOBAL:
				address = 0;

				for (i = 0; i < sizeof(unsigned int); i++)
				{
					virtualMachineAdvance(&machine);

					address <<= BYTE_SIZE_IN_BITS;
					address += machine.thisChar;
				}

				machine.objectBuffer = frameGetObjAt(machine.globalFrame, address);
				
				stackPush(&machine.stack, machine.objectBuffer);
				break;

			case (unsigned char) I_EXIT:
				exit(0);
				break;

			case (unsigned char) I_JMP:
				// Get the address
				address = 0;

				for (i = 0; i < sizeof(unsigned int); i++)
				{
					virtualMachineAdvance(&machine);

					address <<= BYTE_SIZE_IN_BITS;
					address += machine.thisChar;
				}

				// Jump to the address
				fseek(machine.code, (long) address + VALIDATION_BYTES_LEN, SEEK_SET);
				break;

			case (unsigned char) I_JMPF:
				// Get the address
				address = 0;

				for (i = 0; i < sizeof(unsigned int); i++)
				{
					virtualMachineAdvance(&machine);

					address <<= BYTE_SIZE_IN_BITS;
					address += machine.thisChar;
				}

				// Set the return position of the current parameter stack
				machine.paramsStack->returnPosition = ftell(machine.code);

				// Push a new frame.
				virtualMachinePushFrame(&machine);

				for (i = 0; i <= machine.paramsStack->objectsOccupied; i++)
				{
					frameSetObjAt(machine.frame, i, machine.paramsStack->objects[i]);
				}

				// Jump to the address
				fseek(machine.code, (long) address + VALIDATION_BYTES_LEN, SEEK_SET);
				break;

			case (unsigned char) I_NPL:
				virtualMachinePushParamsList(&machine);
				break;

			case (unsigned char) I_PL_APPEND:
				machine.objectBuffer = stackPop(&machine.stack);
				paramsListStackPushObject(machine.paramsStack, machine.objectBuffer);
				break;

			case (unsigned char) I_RETURN:
				// Set the position in the bytecode to the previous position
				fseek(machine.code, (long) machine.paramsStack->returnPosition, SEEK_SET);
				
				// Pop the params list
				virtualMachinePopParamsList(&machine);

				// Pop the frame
				virtualMachinePopFrame(&machine);
				break;

			default:
				fprintf(stderr, "ERROR: Encountered an unknown instruction %X.\n", machine.thisChar);
				halt = true;
				error = true;
				break;
		}
	}

	return error;
}
