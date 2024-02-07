#pragma once

#include "./Stack.h"

Stack createStack()
{
	Stack ret =
	{
		NULL,
		0
	};

	return ret;
}

void stackPush(Stack *stack, SoupObjVar value)
{
	StackNode *newNode = (StackNode *) malloc(sizeof(StackNode));

	newNode->prevNode = stack->top;
	newNode->value = value;

	stack->top = newNode;

	// printf("PUSHING %lf\n", stack->top->value.fVal);
}

SoupObjVar stackPop(Stack *stack)
{
	SoupObjVar ret = stack->top->value;

	stack->top = stack->top->prevNode;

	return ret;
}

#ifdef DEBUG
void DEBUG_stackPrint(Stack *stack)
{
	StackNode *curNode = stack->top;

	printf("\n\nSTACK:\n");

	while (curNode != NULL)
	{
		printf("%lf\n", curNode->value.fVal);
		
		curNode = curNode->prevNode;
	}
}
#endif
