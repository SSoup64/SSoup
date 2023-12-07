#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "./SoupObjVar.h"

typedef struct Frame
{
	struct Frame *lastFrame;

	unsigned int objectsLength, objectsUsed;
	SoupObjVar *objects;
} Frame;

Frame createFrame(Frame *lastFrame)
{
	Frame ret =
	{
		lastFrame,

		1, 0,
		(SoupObjVar *) malloc(sizeof(SoupObjVar))
	};

	return ret;
}

SoupObjVar frameGetObjAt(Frame *frame, unsigned int index)
{
	if (index > frame->objectsUsed)
		fprintf(stderr, "ERROR: Tried to get unitialized object ot something idk.");

	return frame->objects[index];
}

void frameSetObjAt(Frame *frame, unsigned int index, SoupObjVar obj)
{
	// TODO:
	// If the index causes there to be gaps in the arrays, fill them with a null value or something.
	
	if (index > frame->objectsLength)
	{
		frame->objectsLength = (frame->objectsLength + 8 > index) ? frame->objectsLength + 8 : index;
		frame->objects = (SoupObjVar *) realloc(frame->objects, frame->objectsLength);
	}

	frame->objects[index] = obj;
}

