#pragma once

#include "./GenericList.h"

// Implement the functions that were defined in DECL_LIST_TYPE
#define IMPL_LIST_TYPE(_type, _name) \
	List ## _name createList ## _name() \
	{ \
		List ## _name ret = \
		{ \
			LIST_STARTING_LEN, \
			0, \
			(_type *) malloc(sizeof(_type) * LIST_STARTING_LEN), \
		}; \
		\
		return ret; \
	} \
	\
	List ## _name createList ## _name ## Size(unsigned int initLen) \
	{ \
		List ## _name ret = \
		{ \
			initLen, \
			0, \
			(_type *) malloc(sizeof(_type) * LIST_STARTING_LEN), \
		}; \
		\
		return ret; \
	} \
	\
	void list ## _name ## Append(List ## _name *list, _type element) \
	{ \
		if (list->listLen + 1 >= list->listCapacity) \
		{ \
			list->listCapacity += LIST_LENGTH_ADDER; \
			list->elements = (_type *) realloc(list->elements, sizeof(_type) * list->listCapacity); \
			/* "TODO: Memset all the values to 0"; */ \
		} \
		\
		list->elements[list->listLen] = element; \
		list->listLen++; \
	} \
	\
	_type *list ## _name ## GetPtrAt(List ## _name *list, unsigned int index) \
	{ \
		return &list->elements[index]; \
	} \
	\
	_type list ## _name ## GetAt(List ## _name *list, unsigned int index) \
	{ \
		return list->elements[index]; \
	} \
	\
	void list ## _name ## SetAt(List ## _name *list, unsigned int index, _type element) \
	{ \
		if (index >= list->listCapacity) \
		{ \
			list->listCapacity += (index > list->listCapacity + LIST_LENGTH_ADDER) ? index : list->listCapacity + LIST_LENGTH_ADDER; \
			list->elements = (_type *) realloc(list->elements, sizeof(_type) * list->listCapacity); \
		} \
		\
		list->elements[index] = element; \
		\
		if (index > list->listLen) \
		{ \
			list->listLen = index; \
		} \
	}
