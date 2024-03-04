#pragma once

#include "./ParamsListStack.h"
#include "SoupObjVar.h"

/*
Creates a new parameter list.
Input: A pointer to the old parameter list.
Output: A pointer to the new parameter list.
*/
ParamsListStack *createParamsListStack(ParamsListStack *oldParams)
{
	ParamsListStack *params = (ParamsListStack *) malloc(sizeof(ParamsListStack));

	params->objects = createListSoupObjVar();
	
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
	listSoupObjVarAppend(&params->objects, object);
}
