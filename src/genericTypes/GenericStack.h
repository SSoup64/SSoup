#pragma once

#include <stdlib.h>
#include <stdbool.h>

#define DECL_STACK_TYPE(_type, _typeName, _nameFunc) \
	typedef struct _typeName ## Node \
	{ \
		_type val; \
		struct _typeName ## Node *prevNode; \
	} \
	_typeName ## Node; \
	\
	typedef struct _typeName \
	{ \
		_typeName ## Node *thisNode; \
	} \
	_typeName; \
	\
	_typeName create ## _typeName(); \
	\
	_type *_nameFunc ## TopValuePtr(_typeName *stack); \
	\
	_type *_nameFunc ## Pop(_typeName *stack); \
	void _nameFunc ## Push(_typeName *stack, _type val); \
	\
	bool _nameFunc ## CanPop(_typeName *stack);

