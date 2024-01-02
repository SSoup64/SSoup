#pragma once

#include "./AstNode.h"

#include <stdio.h>

const char *NodeTypeToString(NodeType type)
{
	switch (type)
    {
		case TYPE_SCOPE:
			return "Scope";
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
	
		case TYPE_DEBUG_PRINT:
			return "Debug print";
			break;

		case TYPE_FUNC_DECL:
			return "Function declaration";
			break;
		
		case TYPE_EMPTY_NODE:
			return "Empty";
			break;

		case TYPE_PARAMS:
			return "Params";
			break;
		
		case TYPE_FUNC_CALL:
			return "Function call";
			break;

		case TYPE_EXPRS:
			return "Expressions";
			break;
	}

	return "Unknown type";
}

void insertTabs(int tabs)
{
    int i = 0;

	for (i = 0; i < tabs; i++)
		printf("  ");
}

void traverseTree(AstNode node, int indent)
{
	int i = 0;

	// Print the current entry
	insertTabs(indent);
	printf("<%s> %s\n", NodeTypeToString(node.type), node.sVal);
	
	// Traverse the tree for all the child objects
	for (i = 0; i < node.childNodesOccupied; i++)
		traverseTree(node.childNodes[i], indent + 1);
}
