#include "Plush.IntegerInstructions.h"
#include "Plush.StaticInit.h"
#include "Plush.NumOps.h"

namespace Plush
{
	void initInt()
	{
		//Type binaryInt = integerType + integerType;

		//make_instruction((Operator)plus<int>, "INTEGER.+", binaryInt, integerType);
		//make_instruction((Operator)minus<int>, "INTEGER.-", binaryInt, integerType);
		//make_instruction((Operator)multiplies<int>, "INTEGER.*", binaryInt, integerType);
		//make_instruction((Operator)divides<int>, "INTEGER./", binaryInt, integerType);
		//make_instruction((Operator)bool2int, "INTEGER.FROMBOOLEAN", boolType, integerType);
		//make_instruction((Operator)float2int, "INTEGER.FROMFLOAT", floatType, integerType);
		//make_instruction((Operator)int_mod, "INTEGER.%", binaryInt, integerType);
		//make_instruction((Operator)smaller<int>, "INTEGER.<", binaryInt, boolType);
		//make_instruction((Operator)greater<int>, "INTEGER.>", binaryInt, boolType);
		//make_instruction((Operator)_max<int>, "INTEGER.MAX", binaryInt, integerType);
		//make_instruction((Operator)_min<int>, "INTEGER.MIN", binaryInt, integerType);
		////		make_instruction((Operator)rand_int, "INTEGER.RAND", nullType, integerType);
		////		make_instruction((Operator)rand_int, "INTEGER.ERC", nullType, integerType);
		//make_instruction((Operator)in<int>, "INTEGER.IN", integerType, integerType);
		//make_instruction((Operator)inall<int>, "INTEGER.INALL", nullType, integerType);
		//make_instruction((Operator)inallrev<int>, "INTEGER.INALLREV", nullType, integerType);
		//make_instruction((Operator)out<int>, "INTEGER.OUT", binaryInt, nullType);

		make_instruction((Operator)int_mod, "INTEGER", "%", 0);
		make_instruction((Operator)multiplies<long>, "INTEGER", "*", 0);
		make_instruction((Operator)plus<long>, "INTEGER", "+", 0);
		make_instruction((Operator)minus<long>, "INTEGER", "-", 0);
		make_instruction((Operator)divides<long>, "INTEGER", "/", 0);
		make_instruction((Operator)smaller<long>, "INTEGER", "<", 0);
		make_instruction((Operator)greater<long>, "INTEGER", ">", 0);
		make_instruction((Operator)bool2int, "INTEGER", "FROMBOOLEAN", 0);
		make_instruction((Operator)float2int, "INTEGER", "FROMFLOAT", 0);
		make_instruction((Operator)_max<long>, "INTEGER", "MAX", 0);
		make_instruction((Operator)_min<long>, "INTEGER", "MIN", 0);

	}
}