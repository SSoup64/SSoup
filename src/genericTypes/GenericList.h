#pragma once

#include <stdio.h>
#include <stdlib.h>

#define LIST_LENGTH_ADDER 8
#define LIST_STARTING_LEN 1

#define DECL_LIST_TYPE(_type, _name) \
	typedef struct List ## _name \
	{ \
		unsigned int listCapacity; \
		unsigned int listLen; \
		_type *elements; \
	} \
	List ## _name; \
	\
	List ## _name createList ## _name(); \
	List ## _name createList ## _name ## Size(unsigned int initLen); \
	\
	void list ## _name ## Append(List ## _name *list, _type element); \
	\
	_type *list ## _name ## GetPtrAt(List ## _name *list, unsigned int index); \
	_type list ## _name ## GetAt(List ## _name *list, unsigned int index); \
	\
	void list ## _name ## SetAt(List ## _name *list, unsigned int index, _type element);
