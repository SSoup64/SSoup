#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "./SoupObjVar.c"

#define PARAMS_LIST_STACK_OBJECT_LENGTH_ADDER 8

typedef struct ParamsListStack
{
	unsigned int objectsLength, objectsOccupied;
	SoupObjVar *objects;

	struct ParamsListStack *previousParamsList;

	unsigned int returnPosition;
}
ParamsListStack;

ParamsListStack *createParamsListStack(ParamsListStack *oldParams);
void paramsListStackPushObject(ParamsListStack *params, SoupObjVar object);
