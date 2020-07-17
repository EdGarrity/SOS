#include "Plush.BooleanInstructions.h"
#include "Plush.BoolOps.h"
#include "Processor.h"
#include "Plush.StaticInit.h"
#include "Plush.NumOps.h"

namespace Plush
{
	void initBool()
	{
		//make_instruction((Operator)_and, "BOOLEAN.AND", binaryBool, boolType);
		//make_instruction((Operator)_or, "BOOLEAN.OR", binaryBool, boolType);
		//make_instruction((Operator)_not, "BOOLEAN.NOT", boolType, boolType);
		//make_instruction((Operator)_nor, "BOOLEAN.NOR", binaryBool, boolType);
		//make_instruction((Operator)_nand, "BOOLEAN.NAND", binaryBool, boolType);
		//make_instruction((Operator)float2bool, "BOOLEAN.FROMFLOAT", floatType, boolType);
		//make_instruction((Operator)int2bool, "BOOLEAN.FROMINTEGER", integerType, boolType);
		//make_instruction((Operator)in<bool>, "BOOLEAN.IN", integerType, boolType);
		//make_instruction((Operator)inall<bool>, "BOOLEAN.INALL", nullType, boolType);
		//make_instruction((Operator)inallrev<bool>, "BOOLEAN.INALLREV", nullType, boolType);
		//make_instruction((Operator)out<bool>, "BOOLEAN.OUT", integerBool, nullType);

		make_instruction((Operator)_and, "BOOLEAN", "AND");
		make_instruction((Operator)float2bool, "BOOLEAN", "FROMFLOAT");
		make_instruction((Operator)int2bool, "BOOLEAN", "FROMINTEGER");
		make_instruction((Operator)_not, "BOOLEAN", "NOT");
		make_instruction((Operator)_or, "BOOLEAN", "OR");
	}

}