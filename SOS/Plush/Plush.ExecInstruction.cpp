#include "Plush.ExecInstruction.h"
#include "Processor.h"
#include "Plush.StaticInit.h"

namespace Plush
{


	unsigned noop(Environment & _env)
	{
		return 0;
	}

	// An iteration instruction that executes the top item on the EXEC stack a number of times that 
	// depends on the top two integers, while also pushing the loop counter onto the INTEGER stack 
	// for possible access during the execution of the body of the loop
	unsigned do_range(Environment & _env)
	{
		if ((_env.has_elements<long>(2)) && (_env.has_elements<ExecAtom>(1)))
		{
			int n = _env.pop<long>();	// destination index
			int i = _env.pop<long>();	// current index

			std::pair<size_t, size_t> block_range = get_block_index<ExecAtom>(_env, 1, 1);	// Code to execute

			if (n == i)
			{
				_env.push<long>(i);
				_env.push<ExecAtom>(CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"));
			}

			else
			{
				int direction = 1;

				if (i > n)
					direction = -1;

				_env.push<long>(i + direction);
				_env.push<long>(n);

				std::array<ExecAtom, domain::argmap::maximum_stack_size>& exec_stack = _env.get_stack<ExecAtom>().container();

				_env.push<ExecAtom>(CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"));

				for (int n = block_range.first; n < block_range.second + 1; n++)
				{
					ExecAtom v = exec_stack[n];    //_env.peek<ExecAtom>(n);
					_env.push<ExecAtom>(v);
				}

				_env.push<ExecAtom>(CodeAtom("{:instruction EXEC.DO*RANGE :close 0}"));
			}
		}

		return 1;
	}

	void initExec()
	{
		static bool initialized = false;

		if (initialized) return;

		initialized = true;
		//make_instruction((Operator)k, "EXEC.K", execType + execType, execType, 2);
		//make_instruction((Operator)s, "EXEC.S", execType + execType + execType, execType + execType + execType, 3);
		//make_instruction((Operator)y, "EXEC.Y", execType, execType + execType, 1);
		//make_instruction((Operator)exec_if, "EXEC.IF", execType + execType + boolType, execType, 2);
		//make_instruction((Operator)do_range, "EXEC.DO*RANGE", integerType + integerType + execType, execType, 1);
		//make_instruction((Operator)do_count, "EXEC.DO*COUNT", integerType + execType, execType, 1);
		//make_instruction((Operator)do_times, "EXEC.DO*TIMES", integerType + execType, execType, 1);
		//make_instruction((Operator)exec_while, "EXEC.WHILE", boolType + execType, execType, 1);
		//make_instruction((Operator)do_while, "EXEC.DO*WHILE", execType, execType, 1);
		//make_instruction((Operator)exec_when, "EXEC.WHEN", boolType + execType, execType, 1);

		make_instruction((Operator)do_range, "EXEC", "DO*RANGE", 1);
		make_instruction((Operator)noop, "EXEC", "NOOP_OPEN_PAREN", 1);
	}

}