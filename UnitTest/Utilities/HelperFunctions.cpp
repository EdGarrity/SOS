#include "HelperFunctions.h"

namespace UnitTest
{
	bool is_stack_state(Environment & env,
		std::vector<long> int_array,
		std::vector<double> double_array,
		std::vector<bool> bool_array,
		std::vector<ExecAtom> exec_array,
		std::vector<CodeAtom> code_array)
	{
		if (env.has_elements<long>(int_array.size()) == false)
			return false;

		if (!int_array.empty())
		{
			for (size_t n = 0; n < int_array.size(); n++)
			{
				if (int_array[n] != env.peek_index<long>(n))
					return false;
			}
		}


		if (env.has_elements<double>(double_array.size()) == false)
			return false;

		if (!double_array.empty())
		{

			for (size_t n = 0; n < double_array.size(); n++)
			{
				if (double_array[n] != env.peek_index<double>(n))
					return false;
			}
		}


		if (env.has_elements<bool>(bool_array.size()) == false)
			return false;

		if (!bool_array.empty())
		{
			for (size_t n = 0; n < bool_array.size(); n++)
			{
				if (bool_array[n] != env.peek_index<bool>(n))
					return false;
			}
		}

		if (env.has_elements<ExecAtom>(exec_array.size()) == false)
			return false;

		if (!exec_array.empty())
		{
			for (size_t n = 0; n < exec_array.size(); n++)
			{
				if (exec_array[n] != env.peek_index<ExecAtom>(n))
					return false;
			}
		}

		if (env.has_elements<CodeAtom>(code_array.size()) == false)
			return false;

		if (!code_array.empty())
		{
			for (size_t n = 0; n < code_array.size(); n++)
			{
				//if (code_array[n] != processor.peek<CodeAtom>(n))
				//	return false;

				CodeAtom atom1(code_array[n]);
				CodeAtom atom2(env.peek_index<CodeAtom>(n));

				if (atom1 != atom2)
					return false;
			}
		}

		return true;

	}
}
