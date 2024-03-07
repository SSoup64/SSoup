#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "../genericTypes/utilities.c"

#include "./Scope.c"
#include "./Bytecode.h"
#include "./Variable.c"

typedef enum VariableBytecode
{
	LOAD_TO_STACK,
	SAVE_TO_VAR,
}
VariableBytecode;

typedef struct Compiler
{
	ListUchar bytecode;
	ListScope scopes;

	Scope *scope;

	ListVariablePtr globalVars;

	VariableBytecode variableBytecode;
}
Compiler;

Compiler createCompiler(void);

void compilerAppendScope(Compiler *compiler, char *scopeName, ScopeType type);
void compilerPopScope(Compiler *compiler);

void compilerAppendBytecode(Compiler *compiler, unsigned char bytecode);
void compilerSetBytecodeAt(Compiler *compiler, unsigned char bytecode, unsigned int address);

void compilerWriteToFile(Compiler *compiler, FILE *file);

void compilerSetCurScopeFunc(Compiler *compiler, Func func);
unsigned int compilerFindFuncAddress(Compiler *compiler, Scope *curScope, char *name, unsigned int params);

void compilerCreateVariable(Compiler *compiler, Scope *curScope, char *name);
Variable *compilerGetVariable(Compiler *compiler, char *name);

#ifdef DEBUG
void DEBUG_compilerPrintScopes(Compiler *compiler);
#endif
