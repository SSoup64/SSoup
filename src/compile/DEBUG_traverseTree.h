#pragma once

#include "./AstNode.h"

#include <stdio.h>

const char *NodeTypeToString(NodeType type)
{
	switch (type) {
		case TYPE_ROOT:
			return "Root";
			break;
		
		case TYPE_BINOP:
			return "Binary operation";
			break;
		
		case TYPE_NUMBER:
			return "Number";
			break;

		case TYPE_STR:
			return "String";
			break;

		case TYPE_IDENT:
			return "Identifier";
			break;
	
		case TYPE_VAR_DECL:
			return "Variable declaration";
			break;

		case TYPE_VAR_ASSIGN:
			return "Variable assignment";
			break;
	}
}

void insertTabs(int tabs)
{
	for (int i = 0; i < tabs; i++)
		printf("  ");
}

void traverseTree(AstNode node, int indent)
{
	// Print the current entry
	insertTabs(indent);
	printf("<%s> %s\n", NodeTypeToString(node.type), node.sVal);
	
	// Traverse the tree for all the child objects
	for (int i = 0; i < node.childNodesOccupied; i++)
		traverseTree(node.childNodes[i], indent + 1);
}
