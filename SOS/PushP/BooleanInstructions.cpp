#include "Instruction.h"
#include "Env.h"

#include "Literal.h"
#include "BoolOps.h"
#include "NumOps.h"
#include "RandOps.h"

namespace push
{
	void initBool()
	{
		/* BOOLEAN */
		Type binaryBool = boolType + boolType;

		make_instruction((Operator)_and, "BOOLEAN.AND", binaryBool, boolType);
		make_instruction((Operator)_or, "BOOLEAN.OR", binaryBool, boolType);
		make_instruction((Operator)_not, "BOOLEAN.NOT", boolType, boolType);
		make_instruction((Operator)_nor, "BOOLEAN.NOR", binaryBool, boolType);
		make_instruction((Operator)_nand, "BOOLEAN.NAND", binaryBool, boolType);
		make_instruction((Operator)float2bool, "BOOLEAN.FROMFLOAT", floatType, boolType);
		make_instruction((Operator)int2bool, "BOOLEAN.FROMINTEGER", integerType, boolType);
//		make_instruction((Operator)rand_bool, "BOOLEAN.RAND", nullType, boolType);
		make_instruction((Operator)rand_bool, "BOOLEAN.ERC", nullType, boolType);
	}
}