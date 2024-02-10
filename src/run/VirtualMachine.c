#pragma once

#include "./VirtualMachine.h"
#include "Frame.h"

/*
Creates a virtual machine struct.
Input: The path to the file containing the code.
Output: The virtual machine struct.
*/
VirtualMachine createVirtualMachine(char *code)
{
	VirtualMachine ret;

	ret.code = fopen(code, "rb");
	assert(ret.code != NULL);

	ret.thisChar = ' ';

	ret.framesLen = 1;
	ret.frames = (Frame *) malloc(sizeof(Frame));

	ret.frames[0] = createFrame(0, 0);
	
	ret.framesOccupied = 1;

	ret.frame = &ret.frames[0];
	ret.globalFrame = &ret.frames[0];

	ret.stack = createStack();

	ret.objectBuffer.type = OBJ_TYPE_NONE;
	ret.objectBuffer.fVal = 0;
	ret.objectBuffer.sVal = "";

	ret.oprandLeft.type = OBJ_TYPE_NONE;
	ret.oprandLeft.fVal = 0;
	ret.oprandLeft.sVal = "";

	ret.oprandRight.type = OBJ_TYPE_NONE;
	ret.oprandRight.fVal = 0;
	ret.oprandRight.sVal = "";

	ret.paramsStack = NULL; 

	return ret;
}

/*
Pushes a new frame of refrence to a given virtual machine.
Input: A pointer to the virtual machine.
Output: None.
*/
void virtualMachinePushFrame(VirtualMachine *machine)
{
	unsigned int thisFrameIndex = machine->frame->thisFrameIndex;

	if (machine->framesOccupied + 1 >= machine->framesLen)
	{
		machine->framesLen += VIRTUAL_MACHINE_FRAMES_LENGTH_ADDER;
		machine->frames = (Frame *) realloc(machine->frames, machine->framesLen * sizeof(Frame));

		// Since we did a realloc, the address of the array may change, so we need to reconifgure the virtual machine's globalFrame
		machine->globalFrame = &machine->frames[0];
	}

	machine->frames[machine->framesOccupied] = createFrame(machine->framesOccupied, thisFrameIndex);
	machine->frame = &machine->frames[machine->framesOccupied];

	machine->framesOccupied++;
}

/*
Pops the top most frame from a virtual machine
Input: A pointer to the virtual machine.
Output: None.
*/
void virtualMachinePopFrame(VirtualMachine *machine)
{
	machine->frame = &machine->frames[machine->frame->lastFrameIndex];
}

/*
Reads the next byte from the code file of a given virtual machine.
Input: A pointer to the virtual machine.
Output: The byte that was read.
*/
unsigned char virtualMachineAdvance(VirtualMachine *machine)
{
	machine->thisChar = fgetc(machine->code);
	return machine->thisChar;
}

/*
Determines whether or not the source file of a given virtual machine is a SSoup executable.
Input: A pointer to the virtual machine.
Output: 'true' if the file is a SSoup executable, 'false' if not.
*/
bool virtualMachineValidateBytes(VirtualMachine *machine)
{
	char validationBytes[VALIDATION_BYTES_LEN + 1] = "****";
	int i = 0;

	for (i = 0; i < VALIDATION_BYTES_LEN; i++)
	{
		virtualMachineAdvance(machine);
		validationBytes[i] = machine->thisChar;
	}

	return (strcmp(validationBytes, "SOUP") == 0);
}

/*
Pushes a new parameter list to a given virtual machine.
Input: The pointer to the virtual machine.
Output: None.
 */
void virtualMachinePushParamsList(VirtualMachine *machine)
{
	machine->paramsStack = createParamsListStack(machine->paramsStack);
}

/*
Pops the parameter list of a given virtual machine.
Input: The pointer to the virtual machine.
Output: None.
 */
void virtualMachinePopParamsList(VirtualMachine *machine)
{
	machine->paramsStack = machine->paramsStack->previousParamsList;
}
