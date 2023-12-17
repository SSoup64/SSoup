#pragma once

#include <string.h>

typedef struct Variable
{
	// TODO: Add more fields to denote if a variable is a const and it's accessibility level (public, private, etc.)
	char *name;
} Variable;

Variable createVariable(char *name)
{
	Variable ret =
	{
		strdup(name),
	};

	return ret;
}
