#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum NodeType
{
	TYPE_EMPTY_NODE,
	TYPE_NUMBER,
	TYPE_STR,
	TYPE_BINOP,
	TYPE_SCOPE,
	TYPE_IDENT,
	TYPE_VAR_DECL,
	TYPE_VAR_ASSIGN,
	TYPE_DEBUG_PRINT,
	TYPE_FUNC_DECL,
	TYPE_PARAMS,
	TYPE_FUNC_CALL,
	TYPE_EXPRS,
	TYPE_RETURN,
}
NodeType;

typedef struct AstNode
{
	unsigned int childNodesLen;			// The length of the array
	unsigned int childNodesOccupied;	// The number of cells that are already occupied in the array
	struct AstNode *childNodes;			// The array of child nodes
	
	char *sVal;							// The sVal string

	NodeType type;						// The type of the node
}
AstNode;

AstNode createNode(NodeType type, char *sVal, unsigned int childNodesLen);
void nodeAddChild(AstNode *node, AstNode child);
void nodeChildResize(AstNode *node, unsigned int size);


