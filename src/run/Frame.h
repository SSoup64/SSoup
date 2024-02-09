#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "./SoupObjVar.c"

#define FRAME_OBJECTS_LENGTH_ADDER 8

typedef struct Frame
{
	unsigned int thisFrameIndex;
	unsigned int lastFrameIndex;

	unsigned int objectsLength;
	SoupObjVar *objects;
}
Frame;

Frame createFrame(unsigned int thisFrameIndex, unsigned int lastFrameIndex);
SoupObjVar frameGetObjAt(Frame *frame, unsigned int index);
void frameSetObjAt(Frame *frame, unsigned int index, SoupObjVar obj);
