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

		//make_instruction((Operator)plus<long>, "INTEGER", "+");
		//make_instruction((Operator)minus<long>, "INTEGER", "-");
		//make_instruction((Operator)multiplies<long>, "INTEGER", "*");
		//make_instruction((Operator)divides<long>, "INTEGER", "/");
		//make_instruction((Operator)bool2int, "INTEGER", "FROMBOOLEAN");
		//make_instruction((Operator)float2int, "INTEGER", "FROMFLOAT");
		//make_instruction((Operator)int_mod, "INTEGER", "%");
		//make_instruction((Operator)smaller<long>, "INTEGER", "<");
		//make_instruction((Operator)greater<long>, "INTEGER", ">");
		//make_instruction((Operator)_max<long>, "INTEGER", "MAX");
		//make_instruction((Operator)_min<long>, "INTEGER", "MIN");
		//make_instruction((Operator)in<long>, "INTEGER", "IN");
		//make_instruction((Operator)inall<long>, "INTEGER", "INALL");
		//make_instruction((Operator)inallrev<long>, "INTEGER", "INALLREV");
		//make_instruction((Operator)out<long>, "INTEGER", "OUT");


		push_make_instruction((Operator)plus<long>, "INTEGER", "+", integerType + integerType, nullType);
		push_make_instruction((Operator)minus<long>, "INTEGER", "-", integerType + integerType, nullType);
		push_make_instruction((Operator)multiplies<long>, "INTEGER", "*", integerType + integerType, nullType);
		push_make_instruction((Operator)divides<long>, "INTEGER", "/", integerType + integerType, nullType);
		push_make_instruction((Operator)bool2int, "INTEGER", "FROMBOOLEAN", boolType, integerType);
		push_make_instruction((Operator)float2int, "INTEGER", "FROMFLOAT", floatType, integerType);
		push_make_instruction((Operator)int_mod, "INTEGER", "%", integerType + integerType, nullType);
		push_make_instruction((Operator)smaller<long>, "INTEGER", "<", integerType + integerType, boolType);
		push_make_instruction((Operator)greater<long>, "INTEGER", ">", integerType + integerType, boolType);
		push_make_instruction((Operator)_max<long>, "INTEGER", "MAX", integerType + integerType, nullType);
		push_make_instruction((Operator)_min<long>, "INTEGER", "MIN", integerType + integerType, nullType);
		//push_make_instruction((Operator)in<long>, "INTEGER", "IN", integerType, in_out<long>());
		//push_make_instruction((Operator)inall<long>, "INTEGER", "INALL", nullType, nullType);
		//push_make_instruction((Operator)inallrev<long>, "INTEGER", "INALLREV", nullType, nullType);
		push_make_instruction((Operator)out<long>, "INTEGER", "OUT", out_in<long>(), nullType);
	}
}