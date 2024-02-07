#pragma once

#include "./Func.h"

/*
A function that creates a new function struct.
Input: the function's address, how many parameters it takes.
Output: The function struct.
*/
Func createFunc(unsigned int address, unsigned int paramsLen)
{
	Func ret = 
	{
		address,
		paramsLen
	};

	return ret;
}
