#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "./Variable.h"

typedef enum ScopeType
{
	SCOPE_ROOT,
	SCOPE_CLASS,
	SCOPE_FUNC,
} ScopeType;

// Basically stores things about the current scope like variables.
typedef struct Scope
{
	// The previous scope
	struct Scope *prevScope;

	// The scope's name
	char *scopeName;

	// The type of the scope
	// This is needed because the different scopes may act differently
	// For example, if you are in a root scope, you can create classes, but if you are in a class scope, you cannot create new classes.
	ScopeType type;

	// The variables in the scope.
	unsigned int variablesLength, variablesOccupied;
	Variable *variables;
} Scope;

Scope createScope(Scope *oldScope, char *name, ScopeType type)
{
	Scope ret =
	{
		oldScope,
		strdup(name),
		type,
		1, 0,
		(Variable *) malloc(sizeof(Variable))
	};

	return ret;
}

void scopeAddVariable(Scope *scope, char *name)
{
	// TODO: Test if the variable is already initialized and print an error message if it is.
	
	if (scope->variablesOccupied + 1 >= scope->variablesLength)
	{
		scope->variablesLength += 8; // TODO Add that to a const or something later
		scope->variables = (Variable *) realloc(scope->variables, sizeof(Variable) * scope->variablesLength);
	}

	scope->variables[scope->variablesOccupied++] = createVariable(name);
}

unsigned int scopeGetVariable(Scope *scope, char *name)
{
	for (unsigned int i = 0; i < scope->variablesOccupied; i++)
	{
		// printf("%s\n", scope->variables[i].name);
		if (strcmp(scope->variables[i].name, name) == 0)
			return i;
	}

	fprintf(stderr, "Error: Refrence to unitialized variable %s.\n", name);

	exit(1);
}
