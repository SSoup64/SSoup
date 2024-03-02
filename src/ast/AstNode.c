#pragma once

#include "./AstNode.h"

IMPL_LIST_TYPE(struct AstNode, AstNode);

/*
Creates an AST node.
Input: The type of the node, it's value as a string, the number of children it has.
Output: The AST node as an AstNode struct
*/
AstNode createNode(NodeType type, char *sVal, unsigned int childNodesLen)
{
	AstNode ret =	
	{
		.childNodes = createListAstNodeSize(childNodesLen),
		.sVal = strdup(sVal),
		.type = type
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
	listAstNodeAppend(&node->childNodes, child);
}

