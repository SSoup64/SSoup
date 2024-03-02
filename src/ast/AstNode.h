#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../genericTypes/GenericList.c"

#define NODE_DEBUG_PRINT_SIZE								1
#define NODE_DEBUG_PRINT_ARG_ARG							0

#define NODE_RETURN_SIZE									1
#define NODE_RETURN_ARG_RETURNED							0

#define NODE_EMPTY_SIZE										0

#define NODE_BINOP_SIZE										2
#define NODE_BINOP_LVAL										0
#define NODE_BINOP_RVAL										1

#define NODE_NUMBER_SIZE									0

#define NODE_STRING_SIZE									0

#define NODE_INDENTIFIER_SIZE								0

#define NODE_VAR_ASSIGN_SIZE								2
#define NODE_VAR_ASSIGN_ARG_VAR								0
#define NODE_VAR_ASSIGN_ARG_EXPR							1

#define NODE_VAR_DECL_SIZE									1
#define NODE_VAR_DECL_ARG_VAR								0

#define NODE_FUNC_DECL_SIZE									2
#define NODE_FUNC_DECL_ARG_ARGS								0
#define NODE_FUNC_DECL_ARG_CODE								1

#define NODE_PARAMS_DEFAULT_SIZE							1

#define NODE_EXPRS_DEFAULT_SIZE								1

#define NODE_FUNC_CALL_SIZE									2
#define NODE_FUNC_CALL_ARG_ARGS								0
#define NODE_FUNC_CALL_ARG_SCOPE							1

#define NODE_ACCESS_SCOPE_SIZE								1
#define NODE_ACCESS_SCOPE_ARG_NEXT_LAYER					0

#define NODE_VAR_SIZE										2
#define NODE_VAR_ARG_IDENT									0
#define NODE_VAR_ARG_SCOPE									1

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
	TYPE_ACCESS_SCOPE,
	TYPE_VAR,
}
NodeType;

DECL_LIST_TYPE(struct AstNode, AstNode);

typedef struct AstNode
{
	ListAstNode childNodes;				// The child nodes
	
	char *sVal;							// The sVal string

	NodeType type;						// The type of the node
}
AstNode;

AstNode createNode(NodeType type, char *sVal, unsigned int childNodesLen);
void nodeAddChild(AstNode *node, AstNode child);


