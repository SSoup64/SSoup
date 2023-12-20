#pragma once

#include <string.h>

typedef struct Func
{
	// The address of the function
	unsigned int address;

	// How many parameters it takes
	unsigned int paramsLen;
} Func;

Func createFunc(unsigned int address, unsigned int paramsLen)
{
	Func ret = 
	{
		address,
		paramsLen
	};

	return ret;
}
