#ifndef COMPILE_ASTNODE
#define COMPILE_ASTNODE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum NodeType
{
	TYPE_NUMBER,
	TYPE_STR,
	TYPE_BINOP,
	TYPE_ROOT,
	TYPE_IDENT,
	TYPE_VAR_DECL,
} NodeType;

typedef struct AstNode
{
	unsigned int childNodesLen;			// The length of the array
	unsigned int childNodesOccupied;	// The number of cells that are already occupied in the array
	struct AstNode *childNodes;			// The array of child nodes
	
	char *sVal;							// The sVal string

	NodeType type;						// The type of the node
} AstNode;

AstNode createNode(NodeType type, char *sVal, unsigned int childNodesLen)
{
	AstNode ret =	
	{
		childNodesLen,
		0,
		(AstNode *) malloc(sizeof(AstNode) * childNodesLen),
		
		strdup(sVal),

		type
	};
	
	return ret;
}

void nodeAddChild(AstNode *node, AstNode child)
{
	node->childNodes[node->childNodesOccupied++] = child;
}

void nodeChildResize(AstNode *node, unsigned int size)
{
	node->childNodesLen = size;
	node->childNodesOccupied = (size > node->childNodesOccupied) ? node->childNodesOccupied : size;

	// printf("%u", node->childNodesOccupied);
	
	node->childNodes = (AstNode *) realloc(node->childNodes, size * sizeof(AstNode));
}

#endif
