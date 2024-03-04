#pragma once

#include "./Frame.h"
#include "SoupObjVar.h"

IMPL_LIST_TYPE(Frame, Frame);

/*
A function that creates a new frame of refrence.
Input: The ID of the frame that is being created, the ID of the last frame.
Output: A struct that represents the frame.
*/
Frame createFrame(unsigned int thisFrameIndex, unsigned int lastFrameIndex)
{
	Frame ret =
	{
		.thisFrameIndex = thisFrameIndex,
		.lastFrameIndex = lastFrameIndex,
		
		.objects = createListSoupObjVar(),
	};

	return ret;
}

/*
A function that gets the object stored in a certain address in a given frame.
Input: The frame to find the object in, the address of the object.
Output: The object as a SoupObjVar
*/
SoupObjVar frameGetObjAt(Frame *frame, unsigned int index)
{
	// fprintf(stderr, "ERROR: Tried to get unitialized object ot something idk.");

	return listSoupObjVarGetAt(&frame->objects, index);
}

/*
A function that sets the object stored in a certain address in a given frame to an object.
Input: The frame to find the object in, the address of the object, the object to set it to.
Output: None.
*/
void frameSetObjAt(Frame *frame, unsigned int index, SoupObjVar obj)
{
	listSoupObjVarSetAt(&frame->objects, index, obj);
}



