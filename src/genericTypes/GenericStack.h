#pragma once

#include <stdlib.h>
#include <stdbool.h>

#define DECL_STACK_TYPE(_type, _name) \
	typedef struct Stack ## _name ## Node \
	{ \
		_type val; \
		struct Stack ## _name ## Node *prevNode; \
	} \
	Stack ## _name ## Node; \
	\
	typedef struct Stack ## _name \
	{ \
		Stack ## _name ## Node *thisNode; \
	} \
	Stack ## _name; \
	\
	Stack ## _name createStack ## _name(); \
	\
	_type *stack ## _name ## TopValuePtr(Stack ## _name *stack); \
	\
	_type stack ##_name ## Pop(Stack ## _name *stack); \
	void stack ## _name ## Push(Stack ## _name *stack, _type val); \
	\
	bool stack ## _name ## CanPop(Stack ## _name *stack);

