#pragma once
#include "..\..\SOS\Plush\Processor.h"
#include <vector>

using namespace Plush;

namespace UnitTest
{
	bool is_stack_state(Environment & env,
		std::vector<long> int_array,
		std::vector<double> double_array,
		std::vector<bool> bool_array,
		std::vector<ExecAtom> exec_array,
		std::vector<CodeAtom> code_array);
}