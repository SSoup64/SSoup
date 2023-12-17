#pragma once

#define SCOPE_VARIABLE_LENGTH_ADDER 8

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
	// The index of this scope.
	unsigned int scopeIndex;

	// Previous scope index
	unsigned int prevScopeIndex;

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

void createScopeNull(Scope *newScope, char *name, ScopeType type)
{
	newScope->scopeIndex = 0;
	newScope->prevScopeIndex = 0;

	newScope->scopeName = strdup(name);

	newScope->type = type;

	newScope->variablesLength = 1;
	newScope->variablesOccupied = 0;
	newScope->variables = (Variable *) malloc(sizeof(Variable));
}

void createScope(Scope *newScope, unsigned int scopeIndex, unsigned int prevScopeIndex, char *name, ScopeType type)
{
	newScope->scopeIndex = scopeIndex;
	newScope->prevScopeIndex = prevScopeIndex;

	newScope->scopeName = strdup(name);

	newScope->type = type;

	newScope->variablesLength = 1;
	newScope->variablesOccupied = 0;
	newScope->variables = (Variable *) malloc(sizeof(Variable));
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

