#pragma once

#include <string.h>
#include <stdbool.h>

typedef enum VariableType
{
	VAR_TYPE_LOCAL,		// Functions
	VAR_TYPE_GLOBAL,	// Global scope, namespaces, etc.
	VAR_TYPE_ATTR,		// Classes
}
VariableType;

typedef struct Variable
{
	char *name;
	
	VariableType type;
	unsigned int address;
}
Variable;

Variable createVariable(char *name, VariableType type, unsigned int address);
