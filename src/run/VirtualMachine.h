#pragma once

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "Frame.h"
#include "SoupObjVar.h"
#include "Stack.h"

#define VIRTUAL_MACHINE_FRAMES_LENGTH_ADDER 8

typedef struct VirtualMachine
{
	FILE *code;
	unsigned char thisChar;

	unsigned int framesLen, framesOccupied;
	Frame *frames;

	Frame *frame;

	Stack stack;

	SoupObjVar oprandLeft, oprandRight, objectBuffer;
} VirtualMachine;

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

	return ret;
}

void virtualMachinePushFrame(VirtualMachine *machine)
{
	if (machine->framesOccupied >= machine->framesLen)
	{
		machine->framesLen += VIRTUAL_MACHINE_FRAMES_LENGTH_ADDER;
		machine->frames = (Frame *) realloc(machine->frames, machine->framesLen * sizeof(Frame));
	}

	machine->frames[machine->framesOccupied] = createFrame(machine->framesOccupied, machine->frame->thisFrameIndex);
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
	char validationBytes[5] = "****";
	int i = 0;

	for (i = 0; i < 4; i++)
	{
		virtualMachineAdvance(machine);
		validationBytes[i] = machine->thisChar;
	}

	return (strcmp(validationBytes, "SOUP") == 0);
}

