#pragma once

#include "./Frame.h"

/*
A function that creates a new frame of refrence.
Input: The ID of the frame that is being created, the ID of the last frame.
Output: A struct that represents the frame.
*/
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

/*
A function that gets the object stored in a certain address in a given frame.
Input: The frame to find the object in, the address of the object.
Output: The object as a SoupObjVar
*/
SoupObjVar frameGetObjAt(Frame *frame, unsigned int index)
{
	// fprintf(stderr, "ERROR: Tried to get unitialized object ot something idk.");

	return frame->objects[index];
}

/*
A function that sets the object stored in a certain address in a given frame to an object.
Input: The frame to find the object in, the address of the object, the object to set it to.
Output: None.
*/
void frameSetObjAt(Frame *frame, unsigned int index, SoupObjVar obj)
{
	// TODO:
	// If the index causes there to be gaps in the arrays, fill them with a null value or something.
	if (index >= frame->objectsLength)
	{
		frame->objectsLength = (frame->objectsLength + FRAME_OBJECTS_LENGTH_ADDER > index) ? frame->objectsLength + FRAME_OBJECTS_LENGTH_ADDER : index;
		frame->objects = (SoupObjVar *) realloc(frame->objects, frame->objectsLength * sizeof(SoupObjVar));
	}

	frame->objects[index] = obj;
}



