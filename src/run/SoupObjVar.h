#pragma once

typedef enum SoupObjVarType
{
	OBJ_TYPE_NONE,
	OBJ_TYPE_FLOAT,
	OBJ_TYPE_STRING,
}
SoupObjVarType;

typedef struct SoupObjVar
{
	// For now, the double and string values of the SoupObjVar are hardcoded.
	SoupObjVarType type;

	double fVal;
	char *sVal;
}
SoupObjVar;
