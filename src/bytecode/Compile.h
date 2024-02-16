#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../ast/AstNode.h"
#include "./Compiler.c"
#include "./Bytecode.h"
#include "./Func.c"
#include "./Scope.c"
#include "./Variable.c"
#include "./Compiler.h"

void compile(Compiler *compiler, AstNode *node);
void startCompile(Compiler *compiler, AstNode *node);
