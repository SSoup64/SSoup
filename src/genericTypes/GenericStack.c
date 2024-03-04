#pragma once

#include "./GenericStack.h" 

#define IMPL_STACK_TYPE(_type, _name) \
	Stack ## _name create ## Stack ## _name() \
	{ \
		Stack ## _name ret = \
		{ \
			.thisNode = NULL, \
		}; \
		\
		return ret; \
	} \
	\
	_type *stack ## _name ## TopValuePtr(Stack ## _name *stack) \
	{ \
		_type *topVal = &stack->thisNode->val; \
		\
		return topVal; \
	} \
	\
	_type stack ## _name ## Pop(Stack ## _name *stack) \
	{ \
		Stack ## _name ## Node *topNode = stack->thisNode; \
		_type ret = topNode->val; \
		\
		if (topNode != NULL) \
		{ \
			stack->thisNode = topNode->prevNode; \
			\
			free(topNode); \
		} \
		\
		return ret; \
	} \
	\
	void stack ## _name ## Push(Stack ## _name *stack, _type val) \
	{ \
		Stack ## _name ## Node *newNode = (Stack ## _name ## Node *) malloc(sizeof(Stack ## _name ## Node)); \
		\
		newNode->prevNode = stack->thisNode; \
		newNode->val = val; \
		\
		stack->thisNode = newNode; \
	} \
	\
	bool stack ## _name ## CanPop(Stack ## _name *stack) \
	{ \
		bool ret = true; \
		\
		if (NULL == stack->thisNode) \
		{ \
			ret = false; \
		} \
		\
		return ret; \
	} \

