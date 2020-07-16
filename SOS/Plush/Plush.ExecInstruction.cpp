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
			Utilities::FixedSizeStack<ExecAtom> block_a;

			int n = _env.pop<long>();	// destination index
			int i = _env.pop<long>();	// current index

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

				int unmatched_a = _env.pop<ExecAtom>(block_a, 1);

				_env.push<ExecAtom>(block_a);
				_env.push<ExecAtom>(CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"));
				_env.push<ExecAtom>(block_a);

				_env.push<ExecAtom>(CodeAtom("{:instruction EXEC.DO*RANGE :close 0}"));
			}
		}

		return 1;
	}

	unsigned exec_if(Environment & _env)
	{
		if ((_env.has_elements<bool>(1)) && (_env.has_elements<ExecAtom>(2)))
		{
			Utilities::FixedSizeStack<ExecAtom> block_a;
			Utilities::FixedSizeStack<ExecAtom> block_b;

			bool s = _env.pop<bool>();
			int unmatched_a = _env.pop<ExecAtom>(block_a, 1);
			int unmatched_b = _env.pop<ExecAtom>(block_b, 1);

			if ((block_a.size() > 0) && ((block_b.size() > 0) || (unmatched_a > 0)))
			{
				// Remove one closing paranthesis from block before pushing back on stack
				if (s)
				{
					Atom atom = block_a.top();
					block_a.pop();

					atom.close_parentheses = (atom.close_parentheses > 0) ? atom.close_parentheses - 1 : atom.close_parentheses;
					block_a.push(atom);

					_env.push<ExecAtom>(block_a);
				}

				else if (unmatched_a == 0)
				{
					Atom atom = block_b.top();
					block_b.pop();

					atom.close_parentheses = (atom.close_parentheses > 0) ? atom.close_parentheses - 1 : atom.close_parentheses;
					block_b.push(atom);

					_env.push<ExecAtom>(block_b);
				}
			}

			else
			{
				if (block_a.size() > 0)
					_env.push<ExecAtom>(block_a);

				if (block_b.size() > 0)
					_env.push<ExecAtom>(block_b);
			}
		}

		return 1;
	}

	unsigned code_append(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(2))
		{
			Utilities::FixedSizeStack<CodeAtom> block_a;
			Utilities::FixedSizeStack<CodeAtom> block_b;

			int unmatched_a = _env.pop<CodeAtom>(block_a, 1);
			int unmatched_b = _env.pop<CodeAtom>(block_b, 1);

			if ((block_a.size() > 0) && (block_b.size() > 0))
			{
				// Remove one closing paranthesis from block A before pushing the blocks back on stack
				Atom atom = block_a.top();
				block_a.pop();

				atom.close_parentheses = (atom.close_parentheses > 0) ? atom.close_parentheses - 1 : atom.close_parentheses;
				block_a.push(atom);

				atom = block_b.top();
				block_b.pop();

				atom.close_parentheses = (atom.close_parentheses == 0) ? 1 : atom.close_parentheses;
				block_b.push(atom);

				_env.push<CodeAtom>(block_b);
				_env.push<CodeAtom>(block_a);
			}

			else
			{
				if (block_a.size() > 0)
					_env.push<CodeAtom>(block_a);

				if (block_b.size() > 0)
					_env.push<CodeAtom>(block_b);
			}
		}

		return 1;
	}

	unsigned code_atom(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(1))
		{
			Utilities::FixedSizeStack<CodeAtom> block_a;

			int unmatched_a = _env.pop<CodeAtom>(block_a, 1);

			if (block_a.size() == 1)
				_env.push<bool>(true);

			else
				_env.push<bool>(false);
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
		make_instruction((Operator)noop, "EXEC", "NOOP", 0);
		make_instruction((Operator)exec_if, "EXEC", "IF", 2);

		make_instruction((Operator)code_append, "CODE", "APPEND", 0);
		make_instruction((Operator)code_atom, "CODE", "ATOM", 0);
	}

}