#pragma once

#include "./Scope.h"

IMPL_LIST_TYPE(Scope, Scope);

/*
A function that creates the first scope.
Input: pointer to the new scope, the scope's name, the scope's type.
Output: None.
*/
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
	newScope->variables = createListVariable();
	
	// The functions
	newScope->funcsIndices = createListUint();
}

/*
A function that creates a scope.
Input:	A pointer to the new scope, the index of the scope in the compiler's 'scopes' array, the index of the old scope in the compiler's 'scopes' array,
		the scope's name, the path of the scope as a string, the scope's type.
Output: None.
*/
void createScope(Scope *newScope, unsigned int scopeIndex, unsigned int prevScopeIndex, char *name, char *path, ScopeType type)
{
	newScope->scopeIndex = scopeIndex;
	newScope->prevScopeIndex = prevScopeIndex;

	newScope->scopeName = strdup(name);
	newScope->scopePath = strdup(path);

	newScope->type = type;

	newScope->variables = createListVariable();
	
	newScope->funcsIndices = createListUint();
}

/*
A function that adds a variable to a scope
Input: A pointer to the scope to add the variable to, the variable's name, the variable's type.
Output: A pointer to the new variable.
*/
Variable *scopeAddVariable(Scope *scope, char *name, VariableType type)
{
	// TODO: Test if the variable is already initialized and print an error message if it is.

	listVariableAppend(&scope->variables,  createVariable(name, type, scope->variables.listLen));

	return listVariableGetPtrAt(&scope->variables, scope->variables.listLen - 1);
}

/*
Finds a variable based on it's name and the scope to check.
Input: A pointer to the scope to check, the name of the variable
Output: A pointer to the variable that was found, NULL if none was found
*/
Variable *scopeGetVariable(Scope *scope, char *name)
{
	Variable *ret = NULL;

	for (unsigned int i = 0; i < scope->variables.listLen; i++)
	{
		if (strcmp(listVariableGetAt(&scope->variables, i).name, name) == 0)
		{
			ret = listVariableGetPtrAt(&scope->variables, i);
		}
	}

	return ret;
}

/*
Adds the index of a function's scope in the compiler's 'scopes' aray.
Input: The scope to add the function to, the index of the function's scope
Output: None.
*/
void scopeAddFuncIndex(Scope *scope, unsigned int index)
{
	listUintAppend(&scope->funcsIndices, index);
}
