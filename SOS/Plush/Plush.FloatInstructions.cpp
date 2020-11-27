#include "Plush.FloatInstructions.h"
#include "Plush.StaticInit.h"
#include "Plush.NumOps.h"

namespace Plush
{
	void initFloat()
	{

		//make_instruction((Operator)plus<double>, "FLOAT.+", binaryFloat, floatType);
		//make_instruction((Operator)minus<double>, "FLOAT.-", binaryFloat, floatType);
		//make_instruction((Operator)multiplies<double>, "FLOAT.*", binaryFloat, floatType);
		//make_instruction((Operator)divides<double>, "FLOAT./", binaryFloat, floatType);
		//make_instruction((Operator)bool2float, "FLOAT.FROMBOOLEAN", boolType, floatType);
		//make_instruction((Operator)int2float, "FLOAT.FROMINTEGER", integerType, floatType);
		//make_instruction((Operator)_fmod, "FLOAT.%", binaryFloat, floatType);
		//make_instruction((Operator)smaller<double>, "FLOAT.<", binaryFloat, boolType);
		//make_instruction((Operator)greater<double>, "FLOAT.>", binaryFloat, boolType);
		//make_instruction((Operator)_max<double>, "FLOAT.MAX", binaryFloat, floatType);
		//make_instruction((Operator)_min<double>, "FLOAT.MIN", binaryFloat, floatType);
		//make_instruction((Operator)_cos, "FLOAT.COS", floatType, floatType);
		//make_instruction((Operator)_sin, "FLOAT.SIN", floatType, floatType);
		//make_instruction((Operator)_tan, "FLOAT.TAN", floatType, floatType);
		//make_instruction((Operator)_log, "FLOAT.LOG", floatType, floatType);
		//make_instruction((Operator)in<double>, "FLOAT.IN", integerType, floatType);
		//make_instruction((Operator)inall<double>, "FLOAT.INALL", nullType, floatType);
		//make_instruction((Operator)inallrev<double>, "FLOAT.INALLREV", nullType, floatType);
		//make_instruction((Operator)out<double>, "FLOAT.OUT", floatType + integerType, nullType);

		
		//make_instruction((Operator)plus<double>, "FLOAT", "+");
		//make_instruction((Operator)minus<double>, "FLOAT", "-");
		//make_instruction((Operator)multiplies<double>, "FLOAT", "*");
		//make_instruction((Operator)divides<double>, "FLOAT", "/");
		//make_instruction((Operator)bool2float, "FLOAT", "FROMBOOLEAN");
		//make_instruction((Operator)int2float, "FLOAT", "FROMINTEGER");
		//make_instruction((Operator)_fmod, "FLOAT", "%");
		//make_instruction((Operator)smaller<double>, "FLOAT", "<");
		//make_instruction((Operator)greater<double>, "FLOAT", ">");
		//make_instruction((Operator)_max<double>, "FLOAT", "MAX");
		//make_instruction((Operator)_min<double>, "FLOAT", "MIN");
		//make_instruction((Operator)_cos, "FLOAT", "COS");
		//make_instruction((Operator)_sin, "FLOAT", "SIN");
		//make_instruction((Operator)_tan, "FLOAT", "TAN");
		//make_instruction((Operator)in<double>, "FLOAT", "IN");
		//make_instruction((Operator)inall<double>, "FLOAT", "INALL");
		//make_instruction((Operator)inallrev<double>, "FLOAT", "INALLREV");
		//make_instruction((Operator)out<double>, "FLOAT", "OUT");


		push_make_instruction((Operator)plus<double>, "FLOAT", "+", floatType + floatType);
		push_make_instruction((Operator)minus<double>, "FLOAT", "-", floatType + floatType);
		push_make_instruction((Operator)multiplies<double>, "FLOAT", "*", floatType + floatType);
		push_make_instruction((Operator)divides<double>, "FLOAT", "/", floatType + floatType);
		push_make_instruction((Operator)bool2float, "FLOAT", "FROMBOOLEAN", boolType);
		push_make_instruction((Operator)int2float, "FLOAT", "FROMINTEGER", integerType);
		push_make_instruction((Operator)_fmod, "FLOAT", "%", floatType + floatType);
		push_make_instruction((Operator)smaller<double>, "FLOAT", "<", floatType + floatType);
		push_make_instruction((Operator)greater<double>, "FLOAT", ">", floatType + floatType);
		push_make_instruction((Operator)_max<double>, "FLOAT", "MAX", floatType + floatType);
		push_make_instruction((Operator)_min<double>, "FLOAT", "MIN", floatType + floatType);
		push_make_instruction((Operator)_cos, "FLOAT", "COS", floatType);
		push_make_instruction((Operator)_sin, "FLOAT", "SIN", floatType);
		push_make_instruction((Operator)_tan, "FLOAT", "TAN", floatType);
		push_make_instruction((Operator)in<double>, "FLOAT", "IN", integerType);
		push_make_instruction((Operator)inall<double>, "FLOAT", "INALL", nullType);
		push_make_instruction((Operator)inallrev<double>, "FLOAT", "INALLREV", nullType);
		push_make_instruction((Operator)out<double>, "FLOAT", "OUT", out_in<double>());
	}
}