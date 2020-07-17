#include "Plush.ExecInstruction.h"
#include "Processor.h"
#include "Plush.StaticInit.h"
#include <set>

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

	unsigned code_car(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(1))
		{
			Utilities::FixedSizeStack<CodeAtom> block_a;

			int unmatched_a = _env.pop<CodeAtom>(block_a, 1);

			if (block_a.size() > 1)
			{
				Atom atom = block_a.bottom();
				atom.close_parentheses++;
				_env.push<CodeAtom>(atom);
			}

			else
				_env.push<CodeAtom>(block_a);
		}

		return 1;
	}

	unsigned code_cdr(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(1))
		{
			Utilities::FixedSizeStack<CodeAtom> block_a;

			int unmatched_a = _env.pop<CodeAtom>(block_a, 1);

			if (block_a.size() > 1)
			{
				while (block_a.size() > 1)
				{
					Atom atom = block_a.top();
					block_a.pop();
					_env.push<CodeAtom>(atom);
				}
			}

			else
				_env.push<CodeAtom>(CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 1}"));
		}

		return 1;
	}

	unsigned code_cons(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(2))
		{
			Utilities::FixedSizeStack<CodeAtom> block_a;
			Utilities::FixedSizeStack<CodeAtom> block_b;

			_env.pop<CodeAtom>(block_a, 1);
			_env.pop<CodeAtom>(block_b, 1);

			if ((block_a.size() > 0) && (block_b.size() > 0))
			{
				// Remove all closing paranthesis from block B before pushing the blocks back on stack
				Atom atom = block_b.top();
				block_b.pop();

				atom.close_parentheses = (atom.close_parentheses > 0) ? 0 : atom.close_parentheses;
				block_b.push(atom);

				_env.push<CodeAtom>(block_a);
				_env.push<CodeAtom>(block_b);
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

	unsigned code_container(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(2))
		{
			Utilities::FixedSizeStack<CodeAtom> block_a;
			Utilities::FixedSizeStack<CodeAtom> block_b;

			_env.pop<CodeAtom>(block_a, 1);
			_env.pop<CodeAtom>(block_b, 1);

			if ((block_a.size() > 0) && (block_b.size() > 0))
			{
				// Make sure last atom of block B is closed
				Atom atom = block_b.top();
				block_b.pop();

				atom.close_parentheses = (atom.close_parentheses == 0) ? 1 : atom.close_parentheses;
				block_b.push(atom);

				bool found = false;
				int index = 0;

				for (int i = 1; i < block_a.size(); i++)
				{
					found = true;

					for (int j = 0; j < block_b.size(); j++)
					{
						if (block_a[i].instruction != block_b[j].instruction)
						{
							found = false;
							break;
						}
					}

					if (found)
					{
						index = i - 1;
						break;
					}
				}

				if (found)
				{
					_env.push<CodeAtom>(block_b);

					atom = block_a[index];
					_env.push<CodeAtom>(atom);
				}

				else
					_env.push<CodeAtom>(CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 1}"));
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

	unsigned code_contains(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(2))
		{
			Utilities::FixedSizeStack<CodeAtom> block_a;
			Utilities::FixedSizeStack<CodeAtom> block_b;

			_env.pop<CodeAtom>(block_a, 1);
			_env.pop<CodeAtom>(block_b, 1);

			if ((block_a.size() > 0) && (block_b.size() > 0))
			{
				// Make sure last atom of block B is closed
				Atom atom = block_b.top();
				block_b.pop();

				atom.close_parentheses = (atom.close_parentheses == 0) ? 1 : atom.close_parentheses;
				block_b.push(atom);

				bool found = false;
				int index = 0;

				for (int i = 1; i < block_a.size(); i++)
				{
					found = true;

					for (int j = 0; j < block_b.size(); j++)
					{
						if (block_a[i].instruction != block_b[j].instruction)
						{
							found = false;
							break;
						}
					}

					if (found)
					{
						index = i - 1;
						break;
					}
				}

				_env.push<bool>(found);
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

	unsigned code_discrepancy(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(2))
		{
			Utilities::FixedSizeStack<CodeAtom> block_a;
			Utilities::FixedSizeStack<CodeAtom> block_b;

			_env.pop<CodeAtom>(block_a, 1);
			_env.pop<CodeAtom>(block_b, 1);

			std::set<std::string> atom_set;

			if ((block_a.size() > 0) && (block_b.size() > 0))
			{
				for (int i = 0; i < block_a.size(); i++)
					atom_set.insert(block_a[i].instruction);

				for (int i = 0; i < block_b.size(); i++)
					atom_set.insert(block_b[i].instruction);

				int result = 0;

				for (std::string instruction : atom_set)
				{
					int count_a = 0;
					int count_b = 0;

					for (int i = 0; i < block_a.size(); i++)
					{
						if (instruction == block_a[i].instruction)
							count_a++;
						if (instruction == block_b[i].instruction)
							count_b++;
					}

					result += std::abs(count_a - count_b);
				}

				_env.push<long>(result);
			}

			return 1;
		}
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
		make_instruction((Operator)code_car, "CODE", "CAR", 0);
		make_instruction((Operator)code_cdr, "CODE", "CDR", 0);
		make_instruction((Operator)code_cons, "CODE", "CONS", 0);
		make_instruction((Operator)code_container, "CODE", "CONTAINER", 0);
		make_instruction((Operator)code_contains, "CODE", "CONTAINS", 0);
		make_instruction((Operator)code_discrepancy, "CODE", "DISCREPANCY", 0);
	}

}