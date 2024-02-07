#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "./Variable.c"
#include "./Func.c"

#define SCOPE_VARIABLE_LENGTH_ADDER 8
#define SCOPE_FUNC_LENGTH_ADDER 8

typedef enum ScopeType
{
	SCOPE_ROOT,
	SCOPE_CLASS,
	SCOPE_FUNC,
}
ScopeType;

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
}
Scope;

void createScopeNull(Scope *newScope, char *name, ScopeType type);
void createScope(Scope *newScope, unsigned int scopeIndex, unsigned int prevScopeIndex, char *name, char *path, ScopeType type);
Variable *scopeAddVariable(Scope *scope, char *name, VariableType type);
Variable *scopeGetVariable(Scope *scope, char *name);
void scopeAddFuncIndex(Scope *scope, unsigned int index);
