#pragma once

#include "./AstNode.h"

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

	node->childNodes = (AstNode *) realloc(node->childNodes, size * sizeof(AstNode));
}
