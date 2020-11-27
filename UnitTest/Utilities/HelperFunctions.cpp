#include "HelperFunctions.h"

namespace UnitTest
{
	template <typename S1>
	bool match(Environment & env, std::vector<S1> s2)
	{
		if (env.get_stack<S1>().size() != s2.size())
			return false;

		if (!s2.empty())
		{
			for (size_t n = 0; n < s2.size(); n++)
			{
				Genome<S1> S1_Stack = env.get_stack<S1>();

				if (s2[n] != S1_Stack.get_stack_element(n))
					return false;
			}
		}

		return true;
	}

	// long, double, bool, ExecAtom, CodeAtom
	bool is_stack_state(Environment & env,
		std::vector<long> int_array,
		std::vector<double> double_array,
		std::vector<bool> bool_array,
		std::vector<ExecAtom> exec_array,
		std::vector<CodeAtom> code_array)
	{
		if (match<long>(env, int_array)
			&& match<double>(env, double_array)
			&& match<bool>(env, bool_array)
			&& match<ExecAtom>(env, exec_array)
			&& match<CodeAtom>(env, code_array)
			)
			return true;

		else
			return false;
	}
}
