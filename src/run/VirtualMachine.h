#pragma once

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "./Frame.c"
#include "./SoupObjVar.c"
#include "./Stack.c"
#include "./ParamsListStack.c"

#define VALIDATION_BYTES_LEN 4
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
	
	ParamsListStack *paramsStack;
} VirtualMachine;

VirtualMachine createVirtualMachine(char *code);
void virtualMachinePushFrame(VirtualMachine *machine);
void virtualMachinePopFrame(VirtualMachine *machine);
unsigned char virtualMachineAdvance(VirtualMachine *machine);
bool virtualMachineValidateBytes(VirtualMachine *machine);
void virtualMachinePushParamsList(VirtualMachine *machine);
void virtualMachinePopParamsList(VirtualMachine *machine);
