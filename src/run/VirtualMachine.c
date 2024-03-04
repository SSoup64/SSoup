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
	VirtualMachine ret =
	{
		.code = fopen(code, "rb"),
		.thisChar = ' ',

		.frames = createListFrame(),

		.frame = NULL,
		.globalFrame = NULL,

		.stack = createStackSoupObjVar(),

		.objectBuffer.type = OBJ_TYPE_NONE,
		.objectBuffer.fVal = 0,
		.objectBuffer.sVal = "",

		.oprandLeft.type = OBJ_TYPE_NONE,
		.oprandLeft.fVal = 0,
		.oprandLeft.sVal = "",

		.oprandRight.type = OBJ_TYPE_NONE,
		.oprandRight.fVal = 0,
		.oprandRight.sVal = "",

		.paramsStack = NULL, 
	};
	
	listFrameAppend(&ret.frames, createFrame(0, 0));
	ret.frame = listFrameGetPtrAt(&ret.frames, 0);
	ret.globalFrame = listFrameGetPtrAt(&ret.frames, 0);

	assert(ret.code != NULL);

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

	listFrameAppend(&machine->frames, createFrame(machine->frames.listLen, thisFrameIndex));

	machine->globalFrame	= listFrameGetPtrAt(&machine->frames, 0);
	machine->frame			= listFrameGetPtrAt(&machine->frames, machine->frames.listLen - 1);
}

/*
Pops the top most frame from a virtual machine
Input: A pointer to the virtual machine.
Output: None.
*/
void virtualMachinePopFrame(VirtualMachine *machine)
{
	machine->frame = listFrameGetPtrAt(&machine->frames, machine->frame->lastFrameIndex);
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
