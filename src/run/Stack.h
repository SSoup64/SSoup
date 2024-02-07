#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "./SoupObjVar.c"

typedef struct StackNode
{
	SoupObjVar value;
	struct StackNode *prevNode;
}
StackNode;

typedef struct Stack
{
	StackNode *top;
	unsigned int length;
}
Stack;

Stack createStack();
void stackPush(Stack *stack, SoupObjVar value);
SoupObjVar stackPop(Stack *stack);

#ifdef DEBUG
void DEBUG_stackPrint(Stack *stack);
#endif
