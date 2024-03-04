#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "../genericTypes/GenericList.c"
#include "./SoupObjVar.c"

DECL_LIST_TYPE(struct Frame, Frame);

typedef struct Frame
{
	unsigned int thisFrameIndex;
	unsigned int lastFrameIndex;

	ListSoupObjVar objects;
}
Frame;

Frame createFrame(unsigned int thisFrameIndex, unsigned int lastFrameIndex);
SoupObjVar frameGetObjAt(Frame *frame, unsigned int index);
void frameSetObjAt(Frame *frame, unsigned int index, SoupObjVar obj);
