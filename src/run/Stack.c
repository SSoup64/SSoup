#pragma once

#include "./Stack.h"

/*
Creates a program stack.
Input: None.
Output: The struct of the program stack.
 */
Stack createStack()
{
	Stack ret =
	{
		NULL,
		0
	};

	return ret;
}

/*
Pushes an object on top of the stack.
Input: A pointer to the stack to push the value to, the value to push onto the stack.
Output: None.
*/
void stackPush(Stack *stack, SoupObjVar value)
{
	StackNode *newNode = (StackNode *) malloc(sizeof(StackNode));

	newNode->prevNode = stack->top;
	newNode->value = value;

	stack->top = newNode;
}

/*
Pops the top value from a given stack.
Input: The stack to pop the top value off.
Output: The value that was popped off the stack.
*/
SoupObjVar stackPop(Stack *stack)
{
	SoupObjVar ret = stack->top->value;

	stack->top = stack->top->prevNode;

	return ret;
}

