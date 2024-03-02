#pragma once

#include <string.h>

#include "../genericTypes/GenericList.c"

typedef struct Func
{
	// The address of the function
	unsigned int address;

	// How many parameters it takes
	unsigned int paramsLen;
}
Func;

Func createFunc(unsigned int address, unsigned int paramsLen);
