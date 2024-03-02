#pragma once

#include <stdlib.h> 
#include "./GenericStack.h" 

#define IMPL_STACK_TYPE(_type, _typeName, _nameFunc) \
	_typeName create ## _typeName() \
	{ \
		_typeName ret = \
		{ \
			.thisNode = NULL, \
		}; \
		\
		return ret; \
	} \
	\
	_type *_nameFunc ## TopValuePtr(_typeName *stack) \
	{ \
		_type *topVal = &stack->thisNode->val; \
		\
		return topVal; \
	} \
	\
	_type *_nameFunc ## Pop(_typeName *stack) \
	{ \
		_typeName ## Node *topNode = stack->thisNode; \
		\
		if (topNode != NULL) \
		{ \
			stack->thisNode = topNode->prevNode; \
			\
			free(topNode); \
		} \
		\
		return &topNode->val; \
	} \
	\
	void _nameFunc ## Push(_typeName *stack, _type val) \
	{ \
		_typeName ## Node *newNode = (_typeName ## Node *) malloc(sizeof(_typeName ## Node)); \
		\
		newNode->prevNode = stack->thisNode; \
		newNode->val = val; \
		\
		stack->thisNode = newNode; \
	} \
	\
	bool _nameFunc ## CanPop(_typeName *stack) \
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

