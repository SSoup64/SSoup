#pragma once

#define SCOPE_VARIABLE_LENGTH_ADDER 8
#define SCOPE_FUNC_LENGTH_ADDER 8

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "./Variable.h"
#include "./Func.h"

typedef enum ScopeType
{
	SCOPE_ROOT,
	SCOPE_CLASS,
	SCOPE_FUNC,
} ScopeType;

// Basically stores things about the current scope like variables.
typedef struct Scope
{
	// The index of this scope.
	unsigned int scopeIndex;

	// Previous scope index
	unsigned int prevScopeIndex;

	// The scope's name and its path
	char *scopeName;
	char *scopePath;

	// The type of the scope
	// This is needed because the different scopes may act differently
	// For example, if you are in a root scope, you can create classes, but if you are in a class scope, you cannot create new classes.
	ScopeType type;

	// The variables in the scope.
	unsigned int variablesLength, variablesOccupied;
	Variable *variables;
	
	// The scope indices of the functions in the scope
	unsigned int funcsLength, funcsOccupied;
	unsigned int *funcsIndices;

	// SCOPE_FUNC specific variable address.
	Func func;
} Scope;

void createScopeNull(Scope *newScope, char *name, ScopeType type)
{
	// Scope indices
	newScope->scopeIndex = 0;
	newScope->prevScopeIndex = 0;
	
	// The name of the scope
	newScope->scopeName = strdup(name);
	newScope->scopePath = "";
	
	// The type of the scope
	newScope->type = type;

	// The variables
	newScope->variablesLength = 1;
	newScope->variablesOccupied = 0;
	newScope->variables = (Variable *) malloc(sizeof(Variable));
	
	// The functions
	newScope->funcsLength = 1;
	newScope->funcsOccupied = 0;
	newScope->funcsIndices = (unsigned int *) malloc(sizeof(unsigned int));
}

void createScope(Scope *newScope, unsigned int scopeIndex, unsigned int prevScopeIndex, char *name, char *path, ScopeType type)
{
	newScope->scopeIndex = scopeIndex;
	newScope->prevScopeIndex = prevScopeIndex;

	newScope->scopeName = strdup(name);
	newScope->scopePath = strdup(path);

	newScope->type = type;

	newScope->variablesLength = 1;
	newScope->variablesOccupied = 0;
	newScope->variables = (Variable *) malloc(sizeof(Variable));

	newScope->funcsLength = 1;
	newScope->funcsOccupied = 0;
	newScope->funcsIndices = (unsigned int *) malloc(sizeof(unsigned int));
}

void scopeAddVariable(Scope *scope, char *name)
{
	// TODO: Test if the variable is already initialized and print an error message if it is.
	
	if (scope->variablesOccupied + 1 >= scope->variablesLength)
	{
		scope->variablesLength += SCOPE_VARIABLE_LENGTH_ADDER;
		scope->variables = (Variable *) realloc(scope->variables, sizeof(Variable) * scope->variablesLength);
	}

	scope->variables[scope->variablesOccupied++] = createVariable(name);
}

unsigned int scopeGetVariable(Scope *scope, char *name)
{
	for (unsigned int i = 0; i < scope->variablesOccupied; i++)
	{
		if (strcmp(scope->variables[i].name, name) == 0)
			return i;
	}

	fprintf(stderr, "Error: Refrence to unitialized variable %s.\n", name);

	exit(1);
}

void scopeAddFuncIndex(Scope *scope, unsigned int index)
{
	if (scope->funcsOccupied + 1 >= scope->funcsLength)
	{
		scope->funcsLength += SCOPE_FUNC_LENGTH_ADDER;
		scope->funcsIndices = (unsigned int *) realloc(scope->funcsIndices, sizeof(unsigned int) * scope->funcsLength);
	}

	scope->funcsIndices[scope->funcsOccupied++] = index;
}
