#pragma once

#include <stdio.h>

#include "./AstNode.c"

const char *NodeTypeToString(NodeType type);
void insertTabs(int tabs);
void traverseTree(AstNode node, int indent);
