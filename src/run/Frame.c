#pragma once

#include "./Frame.h"

Frame createFrame(unsigned int thisFrameIndex, unsigned int lastFrameIndex)
{
	Frame ret =
	{
		thisFrameIndex,
		lastFrameIndex,

		1,
		(SoupObjVar *) malloc(sizeof(SoupObjVar))
	};

	return ret;
}

SoupObjVar frameGetObjAt(Frame *frame, unsigned int index)
{
	// fprintf(stderr, "ERROR: Tried to get unitialized object ot something idk.");

	return frame->objects[index];
}

void frameSetObjAt(Frame *frame, unsigned int index, SoupObjVar obj)
{
	// TODO:
	// If the index causes there to be gaps in the arrays, fill them with a null value or something.
	
	if (index >= frame->objectsLength)
	{
		frame->objectsLength = (frame->objectsLength + 8 > index) ? frame->objectsLength + 8 : index;
		frame->objects = (SoupObjVar *) realloc(frame->objects, frame->objectsLength * sizeof(SoupObjVar));
	}

	frame->objects[index] = obj;
}



