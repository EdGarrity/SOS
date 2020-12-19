#include "Plush.CodeInstruction.h"
#include "Processor.h"
#include "Plush.StaticInit.h"
#include <set>

namespace Plush
{
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
					}

					for (int i = 0; i < block_b.size(); i++)
					{
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

	void initCode()
	{
		static bool initialized = false;

		if (initialized) return;

		initialized = true;

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
