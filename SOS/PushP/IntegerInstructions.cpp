#include "Instruction.h"
#include "NumOps.h"
#include "RandOps.h"

namespace push
{
	void initInt()
	{
		Type binaryInt = integerType + integerType;
		make_instruction((Operator)plus<int>, "INTEGER.+", binaryInt, integerType);
		make_instruction((Operator)minus<int>, "INTEGER.-", binaryInt, integerType);
		make_instruction((Operator)multiplies<int>, "INTEGER.*", binaryInt, integerType);
		make_instruction((Operator)divides<int>, "INTEGER./", binaryInt, integerType);
		make_instruction((Operator)bool2int, "INTEGER.FROMBOOLEAN", boolType, integerType);
		make_instruction((Operator)float2int, "INTEGER.FROMFLOAT", floatType, integerType);
		make_instruction((Operator)int_mod, "INTEGER.%", binaryInt, integerType);
		make_instruction((Operator)smaller<int>, "INTEGER.<", binaryInt, boolType);
		make_instruction((Operator)greater<int>, "INTEGER.>", binaryInt, boolType);
		make_instruction((Operator)_max<int>, "INTEGER.MAX", binaryInt, integerType);
		make_instruction((Operator)_min<int>, "INTEGER.MIN", binaryInt, integerType);
		make_instruction((Operator)rand_int, "INTEGER.RAND", nullType, integerType);
		make_instruction((Operator)rand_int, "INTEGER.ERC", nullType, integerType);
	}
} // namespace push
