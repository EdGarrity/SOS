#include "PushFunctions.h"

#include "../../PushP/Instruction.h"

namespace domain
{
	namespace stock_forecaster
	{
		void init_push_application_specific_functions()
		{
			Push::make_instruction((Push::Operator)data2float, "FLOAT.FROMDATA", Push::integerType, Push::floatType);

		}


	}
}