#pragma once

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../bytecode/Bytecode.h"
#include "./Frame.c"
#include "./Stack.c"
#include "./ParamsListStack.c"
#include "./SoupObjVar.c"
#include "./VirtualMachine.c"

int run(char *file);