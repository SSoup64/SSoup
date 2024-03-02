#pragma once

#include "./Variable.h"

IMPL_LIST_TYPE(Variable *, VariablePtr);

/*
This function creates a new variable struct and returns it.
Input: The variable's name, the variable's type (global, local, attribute), the variable's address
Output: The variable struct.
*/
Variable createVariable(char *name, VariableType type, unsigned int address)
{
	Variable ret =
	{
		strdup(name),
		type,
		address,
	};

	return ret;
}
