#include "Instruction.h"
#include "Env.h"

#include "Literal.h"
#include "NumOps.h"
#include "RandOps.h"

namespace Push
{
#include "StaticLibMacros.h"
	DECLARE_OBJECT(FloatInstructions);

	void initFloat()
	{
		Type binaryFloat = floatType + floatType;
		make_instruction((Operator)plus<double>, "FLOAT.+", binaryFloat, floatType);
		make_instruction((Operator)minus<double>, "FLOAT.-", binaryFloat, floatType);
		make_instruction((Operator)multiplies<double>, "FLOAT.*", binaryFloat, floatType);
		make_instruction((Operator)divides<double>, "FLOAT./", binaryFloat, floatType);
		make_instruction((Operator)bool2float, "FLOAT.FROMBOOLEAN", boolType, floatType);
		make_instruction((Operator)int2float, "FLOAT.FROMINTEGER", integerType, floatType);
		make_instruction((Operator)_fmod, "FLOAT.%", binaryFloat, floatType);
		make_instruction((Operator)smaller<double>, "FLOAT.<", binaryFloat, boolType);
		make_instruction((Operator)greater<double>, "FLOAT.>", binaryFloat, boolType);
		make_instruction((Operator)_max<double>, "FLOAT.MAX", binaryFloat, floatType);
		make_instruction((Operator)_min<double>, "FLOAT.MIN", binaryFloat, floatType);
		make_instruction((Operator)_cos, "FLOAT.COS", floatType, floatType);
		make_instruction((Operator)_sin, "FLOAT.SIN", floatType, floatType);
		make_instruction((Operator)_tan, "FLOAT.TAN", floatType, floatType);
//		make_instruction((Operator)rand_float, "FLOAT.RAND", nullType, floatType);
//		make_instruction((Operator)rand_float, "FLOAT.ERC", nullType, floatType);

//		make_instruction((Operator)data2float, "FLOAT.FROMDATA", integerType, floatType);
		make_instruction((Operator)_exp, "FLOAT.EXP", floatType, floatType);
		make_instruction((Operator)_log, "FLOAT.LOG", floatType, floatType);
	}
}