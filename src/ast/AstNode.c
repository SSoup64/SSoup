#pragma once

#include "./AstNode.h"

/*
Creates an AST node.
Input: The type of the node, it's value as a string, the number of children it has.
Output: The AST node as an AstNode struct
*/
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

/*
Adds a child to a given child.
Input: A pointer of the node to add the child to, the child node.
Output: None.
*/
void nodeAddChild(AstNode *node, AstNode child)
{
	node->childNodes[node->childNodesOccupied++] = child;
}

/*
Resizes the amount of children a node can have.
Input: The pointer to the node to resize, the new size.
Output: None.
*/
void nodeChildResize(AstNode *node, unsigned int size)
{
	node->childNodesLen = size;
	node->childNodesOccupied = (size > node->childNodesOccupied) ? node->childNodesOccupied : size;

	node->childNodes = (AstNode *) realloc(node->childNodes, size * sizeof(AstNode));
}










