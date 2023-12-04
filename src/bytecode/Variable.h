#pragma once

#include <string.h>

typedef struct Variable
{
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
