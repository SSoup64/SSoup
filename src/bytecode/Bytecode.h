#pragma once

enum Bytecode
{
	I_NOP,
	
	I_PUSH_INT,
	I_PUSH_DOUBLE,
	I_PUSH_STRING,
	I_PUSH_MEM,

	I_POP,

	I_OP_PLUS,
	I_OP_STAR,
	I_OP_MINUS,
	I_OP_SLASH,

	I_EXIT,

	I_DEBUG_PRINT,
	
	I_JMP,
	I_JMPF,

	I_NPL,
	I_PL_APPEND,
};

