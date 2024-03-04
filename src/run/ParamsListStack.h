#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "./SoupObjVar.c"

typedef struct ParamsListStack
{
	ListSoupObjVar objects;

	struct ParamsListStack *previousParamsList;

	unsigned int returnPosition;
}
ParamsListStack;

ParamsListStack *createParamsListStack(ParamsListStack *oldParams);
void paramsListStackPushObject(ParamsListStack *params, SoupObjVar object);
