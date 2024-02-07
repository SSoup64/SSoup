#pragma once

#include "./VirtualMachine.h"

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

void virtualMachinePushFrame(VirtualMachine *machine)
{
	unsigned int thisFrameIndex = machine->frame->thisFrameIndex;

	if (machine->framesOccupied + 1 >= machine->framesLen)
	{
		machine->framesLen += VIRTUAL_MACHINE_FRAMES_LENGTH_ADDER;
		machine->frames = (Frame *) realloc(machine->frames, machine->framesLen * sizeof(Frame));
	}

	machine->frames[machine->framesOccupied] = createFrame(machine->framesOccupied, thisFrameIndex);
	machine->frame = &machine->frames[machine->framesOccupied];

	machine->framesOccupied++;
}

void virtualMachinePopFrame(VirtualMachine *machine)
{
	machine->frame = &machine->frames[machine->frame->lastFrameIndex];
}

unsigned char virtualMachineAdvance(VirtualMachine *machine)
{
	machine->thisChar = fgetc(machine->code);
	return machine->thisChar;
}

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

void virtualMachinePushParamsList(VirtualMachine *machine)
{
	machine->paramsStack = createParamsListStack(machine->paramsStack);
}

void virtualMachinePopParamsList(VirtualMachine *machine)
{
	machine->paramsStack = machine->paramsStack->previousParamsList;
}
