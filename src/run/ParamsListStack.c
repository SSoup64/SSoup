#pragma once

#include "./ParamsListStack.h"

/*
Creates a new parameter list.
Input: A pointer to the old parameter list.
Output: A pointer to the new parameter list.
*/
ParamsListStack *createParamsListStack(ParamsListStack *oldParams)
{
	ParamsListStack *params = (ParamsListStack *) malloc(sizeof(ParamsListStack));

	params->objectsLength = 1;
	params->objects = (SoupObjVar *) malloc(params->objectsLength * sizeof(SoupObjVar));
	params->objectsOccupied = 0;
	
	params->previousParamsList = oldParams;

	params->returnPosition = 0;

	return params;
}

/*
Pushes an onject to the parameter list.
Input: A pointer to the parameter list, the object to pust.
Output: None.
*/
void paramsListStackPushObject(ParamsListStack *params, SoupObjVar object)
{
	if (params->objectsOccupied + 1 >= params->objectsLength)
	{
		params->objectsLength += PARAMS_LIST_STACK_OBJECT_LENGTH_ADDER;
		params->objects = (SoupObjVar *) realloc(params->objects, sizeof(SoupObjVar) * params->objectsLength);
	}

	params->objects[params->objectsOccupied++] = object;
}
