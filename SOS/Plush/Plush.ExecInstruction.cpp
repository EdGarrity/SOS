#include <set>
#include <stack>
#include "Genome.h"
#include "Plush.ExecInstruction.h"
#include "Processor.h"
#include "Plush.StaticInit.h"
#include "..\Utilities\String.h"

namespace Plush
{
	//unsigned int CodeLength(Utilities::FixedSizeStack<Atom> &stack)
	//{
	//	unsigned int item_number = 0;
	//	unsigned int n = stack.size() - 1;
	//	unsigned int wanted_blocks = 0;

	//	std::stack<unsigned int> wanted_stack;

	//	do
	//	{
	//		for (; n >= 0; n--)
	//		{
	//			Plush::Atom atom = stack[n];

	//			int closing = atom.close_parentheses - Func2BlockWantsMap[atom.instruction];

	//			if (closing < 0)
	//			{
	//				wanted_stack.push(wanted_blocks);
	//				wanted_blocks = 0 - closing;
	//			}

	//			if (closing > 0)
	//			{
	//				wanted_blocks > 0 ? --wanted_blocks : 0;
	//			}

	//			if (wanted_blocks == 0)
	//			{
	//				if (wanted_stack.size() > 0)
	//				{
	//					wanted_blocks = wanted_stack.top();
	//					wanted_stack.pop();
	//				}

	//				item_number++;
	//			}
	//		};
	//	} while (n >= 0);

	//	return item_number;
	//}

	// Expand inner block
	unsigned noop_open_paren(Environment & _env)
	{
		if (_env.has_elements<ExecAtom>(1))
		{
			Utilities::FixedSizeStack<Atom> code_block;

			int unmatched_a = _env.pop<ExecAtom>(code_block, 2);

			// Remove one closing paranthesis from block before pushing back on stack
			Atom atom = code_block.top();
			code_block.pop();

			atom.close_parentheses = (atom.close_parentheses > 0) ? atom.close_parentheses - 1 : atom.close_parentheses;
			code_block.push(atom);

			_env.push<ExecAtom>(code_block);
		}

		return 0;
	}

	unsigned noop(Environment & _env)
	{
		return 0;
	}

	// An iteration instruction that executes the top item on the EXEC stack a number of times that 
	// depends on the top two integers, while also pushing the loop counter onto the INTEGER stack 
	// for possible access during the execution of the body of the loop
	unsigned exec_do_range(Environment & _env)
	{
		if ((_env.has_elements<long>(2)) && (_env.has_elements<ExecAtom>(1)))
		{
			Utilities::FixedSizeStack<Atom> code_block;

			int n = _env.pop<long>();	// destination index
			int i = _env.pop<long>();	// current index

			if (n == i)
				_env.push<long>(i);

			else
			{
				int direction = 1;

				if (i > n)
					direction = -1;

				_env.push<long>(i + direction);
				_env.push<long>(n);

				int unmatched_a = _env.pop<ExecAtom>(code_block, 1);

				_env.push<ExecAtom>(code_block);
				_env.push<ExecAtom>(code_block);

				Atom atom = _env.top<ExecAtom>();

				if (atom.instruction == "EXEC.NOOP_OPEN_PAREN")
					_env.pop<ExecAtom>();

				_env.push<ExecAtom>(CodeAtom("{:instruction EXEC.DO*RANGE :close 0}"));
			}
		}

		return 1;
	}

	unsigned exec_if(Environment & _env)
	{
		if ((_env.has_elements<bool>(1)) && (_env.has_elements<ExecAtom>(2)))
		{
			Utilities::FixedSizeStack<Atom> block_a;
			Utilities::FixedSizeStack<Atom> block_b;

			bool done = false;
			bool s = _env.pop<bool>();
			unsigned int unmatched = 0;
			unsigned int unmatched_a = _env.pop<ExecAtom>(block_a, 1);
			unsigned int unmatched_b = 0;

			if (unmatched_a > 0)
			{
				std::string noop = "{:instruction EXEC.NOOP :close " + std::to_string(unmatched_a) + "}";
				_env.push<ExecAtom>(ExecAtom(noop));
			}

//			_env.push<ExecAtom>(CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"));
			unmatched_b = _env.pop<ExecAtom>(block_b, 1);
			unmatched = unmatched_b;

			if (s)
			{
				if (block_a.size() > 0)
				{
					if (unmatched > 0)
					{
						std::string noop = "{:instruction EXEC.NOOP :close " + std::to_string(unmatched) + "}";
						_env.push<ExecAtom>(ExecAtom(noop));
					}

					Atom atom = block_a.top();
					block_a.pop();

					atom.close_parentheses = (atom.close_parentheses > 0) ? atom.close_parentheses - 1 : atom.close_parentheses;
					block_a.push(atom);

					_env.push<ExecAtom>(block_a);

					done = true;
				}
			}

			else
			{
				if (block_b.size() > 0)
				{
					if (unmatched > 0)
					{
						std::string noop = "{:instruction EXEC.NOOP :close " + std::to_string(unmatched) + "}";
						_env.push<ExecAtom>(ExecAtom(noop));
					}

					Atom atom = block_b.top();
					block_b.pop();

					atom.close_parentheses = (atom.close_parentheses > 0) ? atom.close_parentheses - 1 : atom.close_parentheses;
					block_b.push(atom);

					_env.push<ExecAtom>(block_b);

					done = true;
				}
			}

			if (!done)
			{
				if (block_a.size() > 0)
					_env.push<ExecAtom>(block_a);

				if (block_b.size() > 0)
					_env.push<ExecAtom>(block_b);
			}
		}

		return 1;
	}

	unsigned exec_do_count(Environment & _env)
	{
		if ((_env.has_elements<long>(1)) && (_env.has_elements<ExecAtom>(1)))
		{
			int n = _env.pop<long>();	// destination index

			if (n > 0)
			{
				_env.push<long>(0);
				_env.push<long>(n - 1);

//				_env.pop<ExecAtom>();
				_env.push<ExecAtom>(CodeAtom("{:instruction EXEC.DO*RANGE :close 0}"));
			}
		}

		return 1;
	}

	unsigned exec_do_times(Environment & _env)
	{
		if ((_env.has_elements<long>(1)) && (_env.has_elements<ExecAtom>(1)))
		{
			int n = _env.pop<long>();	// destination index
			Utilities::FixedSizeStack<Atom> code_block;

			if (n > 0)
			{
				_env.push<long>(0);
				_env.push<long>(n - 1);

				int unmatched_a = _env.pop<ExecAtom>(code_block, 1);

				_env.push<ExecAtom>(CodeAtom("{:instruction INTEGER.POP :close 1}"));
				_env.push<ExecAtom>(code_block);

				Atom atom = _env.top<ExecAtom>();

				if (atom.instruction == "EXEC.NOOP_OPEN_PAREN")
					_env.pop<ExecAtom>();

				_env.push<ExecAtom>(CodeAtom("{:instruction EXEC.DO*RANGE :close 0}"));
			}
		}

		return 1;
	}

	unsigned exec_k(Environment & _env)
	{
		if (_env.has_elements<ExecAtom>(2))
		{
			int index = 1;

			Utilities::FixedSizeStack<ExecAtom> &stack = _env.get_stack<ExecAtom>();
			Utilities::FixedSizeStack<Atom> extracted_block;
			Utilities::FixedSizeStack<Atom> block_without_extracted;

			if (index > 0)
			{
				// Get count of sub-blocks
				int number_of_blocks = 0;
				int n = stack.size() - 1;

				n = stack.size() - 1;
				int block_number = 0;

				do
				{
					int blocks_open = 0;

					for (; n >= 0; n--)
					{
						Plush::Atom atom = stack[n];

						if (block_number == index)
							extracted_block.push(atom);

						else
							block_without_extracted.push(atom);

						blocks_open += Plush::Func2BlockWantsMap[atom.instruction];
						blocks_open -= atom.close_parentheses;
						blocks_open = (blocks_open > 0) ? blocks_open : 0;

						if (atom.close_parentheses > 0)
						{
							if (blocks_open > 0)
								blocks_open++;

							else
							{
								block_number++;
								blocks_open = 1;
							}
						}
					};
				} while (n >= 0);

				_env.get_stack<ExecAtom>().clear();
				_env.push<ExecAtom>(block_without_extracted);
			}
		}

		return 1;
	}

	unsigned exec_s(Environment & _env)
	{
		if (_env.has_elements<ExecAtom>(3))
		{
			Utilities::FixedSizeStack<Atom> block_a;
			Utilities::FixedSizeStack<Atom> block_b;
			Utilities::FixedSizeStack<Atom> block_c;

			unsigned int unmatched = 0;

			unsigned int unmatched_a = _env.pop<ExecAtom>(block_a, 1);

			if (unmatched_a > 0)
			{
				std::string noop = "{:instruction EXEC.NOOP :close " + std::to_string(unmatched_a) + "}";
				_env.push<ExecAtom>(ExecAtom(noop));

				Atom atom = block_a.top();
				block_a.pop();

				atom.close_parentheses = (atom.close_parentheses > unmatched_a)
					? atom.close_parentheses - unmatched_a
					: atom.close_parentheses;
				block_a.push(atom);
			}

//			_env.push<ExecAtom>(CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"));

			unsigned int unmatched_b = _env.pop<ExecAtom>(block_b, 1);

			if (unmatched_b > 0)
			{
				std::string noop = "{:instruction EXEC.NOOP :close " + std::to_string(unmatched_b) + "}";
				_env.push<ExecAtom>(ExecAtom(noop));

				Atom atom = block_b.top();
				block_b.pop();

				atom.close_parentheses = (atom.close_parentheses > unmatched_b)
					? atom.close_parentheses - unmatched_b
					: atom.close_parentheses;
				block_b.push(atom);
			}

//			_env.push<ExecAtom>(CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"));

			unsigned int unmatched_c = _env.pop<ExecAtom>(block_c, 1);

			if (unmatched_c > 0)
			{
				std::string noop = "{:instruction EXEC.NOOP :close " + std::to_string(unmatched_c) + "}";
				_env.push<ExecAtom>(ExecAtom(noop));

				Atom atom = block_c.top();
				block_c.pop();

				atom.close_parentheses = (atom.close_parentheses > unmatched_c)
					? atom.close_parentheses - unmatched_c
					: atom.close_parentheses;
				block_c.push(atom);
			}

			unmatched = unmatched_c;

			if ((block_a.size() > 0) && (block_b.size() > 0) && (block_c.size() > 0))
			{
				_env.push<ExecAtom>(ExecAtom("{:instruction EXEC.NOOP :close 1}"));
				_env.push<ExecAtom>(block_b);
				_env.push<ExecAtom>(block_c);
				_env.push<ExecAtom>(ExecAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"));
				_env.push<ExecAtom>(block_c);
				_env.push<ExecAtom>(block_a);
			}

			else
			{
				_env.push<ExecAtom>(block_c);
				_env.push<ExecAtom>(block_b);
				_env.push<ExecAtom>(block_a);
			}
		}
		return 1;
	}

	unsigned exec_y(Environment & _env)
	{
		if (_env.has_elements<ExecAtom>(1))
		{
			Utilities::FixedSizeStack<Atom> extracted_block;

			_env.pop<ExecAtom>(extracted_block, 1);

			_env.push<ExecAtom>(extracted_block);
			_env.push<ExecAtom>(ExecAtom("{:instruction EXEC.Y :close 0}"));
			_env.push<ExecAtom>(ExecAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"));
			_env.push<ExecAtom>(extracted_block);
		}

		return 1;
	}

	unsigned code_append(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(2))
		{
			Utilities::FixedSizeStack<Atom> block_a;
			Utilities::FixedSizeStack<Atom> block_b;

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
			Utilities::FixedSizeStack<Atom> block_a;

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
			Utilities::FixedSizeStack<Atom> block_a;

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
			Utilities::FixedSizeStack<Atom> block_a;

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
			Utilities::FixedSizeStack<Atom> block_a;
			Utilities::FixedSizeStack<Atom> block_b;

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
			Utilities::FixedSizeStack<Atom> block_a;
			Utilities::FixedSizeStack<Atom> block_b;

			_env.pop<CodeAtom>(block_a, 1);
			_env.pop<CodeAtom>(block_b, 1);

			if ((block_a.size() > 0) && (block_b.size() > 0))
			{
				// Make sure last atom of block B is closed
				Atom atom = block_b.top();
				block_b.pop();

				atom.close_parentheses = (atom.close_parentheses < 2) ? 2 : atom.close_parentheses;
				block_b.push(atom);

				bool found = false;
				int index = 0;

				for (int i = 1; i < (block_a.size() - block_b.size() + 1); i++)
				{
					found = true;

					for (int j = 0; j < block_b.size(); j++)
					{
						if (block_a[i + j].instruction != block_b[j].instruction)
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
			Utilities::FixedSizeStack<Atom> block_a;
			Utilities::FixedSizeStack<Atom> block_b;

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

				for (int i = 1; i < (block_a.size() - block_b.size() + 1); i++)
				{
					found = true;

					for (int j = 0; j < block_b.size(); j++)
					{
						if (block_a[i + j].instruction != block_b[j].instruction)
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
			Utilities::FixedSizeStack<Atom> block_a;
			Utilities::FixedSizeStack<Atom> block_b;

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

	unsigned code_do(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(1))
		{
			Utilities::FixedSizeStack<Atom> block_a;

			_env.pop<CodeAtom>(block_a, 1);
			_env.push<CodeAtom>(block_a);
			_env.push<ExecAtom>(CodeAtom("{:instruction CODE.POP :close 1}"));
			_env.push<ExecAtom>(block_a);

			return 1;
		}
	}

	unsigned code_do_star(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(1))
		{
			Utilities::FixedSizeStack<Atom> block_a;

			_env.pop<CodeAtom>(block_a, 1);
			_env.push<ExecAtom>(block_a);

			return 1;
		}
	}

	unsigned code_do_range(Environment & _env)
	{
		if ((_env.has_elements<long>(2)) && (_env.has_elements<CodeAtom>(1)))
		{
			Utilities::FixedSizeStack<Atom> block_a;

			int n = _env.pop<long>();	// destination index
			int i = _env.pop<long>();	// current index

			if (n == i)
			{
				_env.push<long>(i);
				_env.pop<CodeAtom>(block_a, 1);
				_env.push<CodeAtom>(block_a);
				_env.push<ExecAtom>(CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"));
				_env.push<ExecAtom>(block_a);
			}

			else
			{
				int direction = 1;

				if (i > n)
					direction = -1;

				_env.push<long>(i + direction);
				_env.push<long>(n);

				_env.pop<CodeAtom>(block_a, 1);
				_env.push<CodeAtom>(block_a);

				_env.push<ExecAtom>(CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"));
				_env.push<ExecAtom>(block_a);

				_env.push<ExecAtom>(CodeAtom("{:instruction CODE.DO*RANGE :close 1}"));
			}
		}

		return 1;
	}

	unsigned code_do_count(Environment & _env)
	{
		if ((_env.has_elements<long>(1)) && (_env.has_elements<CodeAtom>(1)))
		{
			int n = _env.pop<long>();	// destination index

			if (n > 0)
			{
				_env.push<long>(0);
				_env.push<long>(n - 1);

				_env.push<ExecAtom>(CodeAtom("{:instruction CODE.DO*RANGE :close 1}"));
			}
		}

		return 1;
	}

	unsigned code_do_times(Environment & _env)
	{
		if ((_env.has_elements<long>(1)) && (_env.has_elements<CodeAtom>(1)))
		{
			int n = _env.pop<long>();	// destination index

			if (n > 0)
			{
				_env.push<long>(0);
				_env.push<long>(n - 1);

				_env.push<ExecAtom>(CodeAtom("{:instruction INTEGER.POP :close 1}"));
				_env.push<ExecAtom>(CodeAtom("{:instruction CODE.DO*RANGE :close 1}"));
			}
		}

		return 1;
	}

	unsigned code_extract(Environment & _env)
	{
		if ((_env.has_elements<long>(1)) && (_env.has_elements<CodeAtom>(1)))
		{
			int index = std::abs(_env.pop<long>());	// index

			Utilities::FixedSizeStack<Atom> top_block;
			Utilities::FixedSizeStack<Atom> extracted_block;
			Utilities::FixedSizeStack<Atom> block_without_extracted;
			Utilities::FixedSizeStack<Atom> block_copy;

			if (index != 0)
			{
				// Get first block from stack
				_env.pop<CodeAtom>(top_block, 1);

				// Save copy of the top block.
				block_copy = top_block;

				// Get count of sub-blocks
				int number_of_blocks = 0;
				int n = 0;

				do
				{
					int blocks_open = 1;

					for (; n < top_block.size(); n++)
					{
						Plush::Atom atom = top_block[n];

						blocks_open += Plush::Func2BlockWantsMap[atom.instruction];
						blocks_open -= atom.close_parentheses;

						if (atom.close_parentheses > 0)
						{
							if (blocks_open > 0)
								blocks_open++;

							number_of_blocks++;
						}

						if (blocks_open <= 0)
							break;
					};

					if (blocks_open <= 0)
						break;

				} while (n < top_block.size());

				// Take modulo the number of blocks to ensure that it is within the meaningful range.
				index = (std::abs(index) - 1) % number_of_blocks;

				// Restore sub-block from copy
				top_block = block_copy;

				// Get the target sub-block
				n = 0;
				int block_number = 0;

				do
				{
					int blocks_open = 1;
					bool first_element_of_block = false;

					for (; n < top_block.size(); n++)
					{
						Plush::Atom atom = top_block[n];

						if (Plush::Func2BlockWantsMap[atom.instruction] > 0)
							first_element_of_block = true;

						if ((block_number == index) && (!first_element_of_block))
							extracted_block.push(atom);

						else
							block_without_extracted.push(atom);

						first_element_of_block = false;

						blocks_open += Plush::Func2BlockWantsMap[atom.instruction];
						blocks_open -= atom.close_parentheses;

						if (atom.close_parentheses > 0)
						{
							if (blocks_open > 0)
								blocks_open++;

							block_number++;
						}

						if (blocks_open <= 0)
							break;
					};

					if (blocks_open <= 0)
						break;

				} while ((block_number < number_of_blocks) && (n < top_block.size()));

				_env.push<CodeAtom>(block_without_extracted);
				_env.push<CodeAtom>(extracted_block);
			}
		}

		return 1;
	}

	inline unsigned bool2code(Environment & _env)
	{
		if (_env.has_elements<bool>(1))
		{
			bool val = _env.pop<bool>();

			if (val)
				_env.push<CodeAtom>(CodeAtom("{:instruction TRUE :close 1}"));

			else
				_env.push<CodeAtom>(CodeAtom("{:instruction FALSE :close 1}"));
		}

		return 1;
	}

	inline unsigned float2code(Environment & _env)
	{
		if (_env.has_elements<double>(1))
		{
			double val = _env.pop<double>();
			std::string instruction = Utilities::string_format("{:instruction %f :close 1}", val);
			_env.push<CodeAtom>(CodeAtom(instruction));
		}

		return 1;
	}

	inline unsigned int2code(Environment & _env)
	{
		if (_env.has_elements<long>(1))
		{
			long val = _env.pop<long>();
			std::string instruction = Utilities::string_format("{:instruction %d :close 1}", val);
			_env.push<CodeAtom>(CodeAtom(instruction));
		}

		return 1;
	}

	unsigned code_if(Environment & _env)
	{
		if ((_env.has_elements<bool>(1)) && (_env.has_elements<CodeAtom>(2)))
		{
			Utilities::FixedSizeStack<Atom> block_a;
			Utilities::FixedSizeStack<Atom> block_b;

			bool s = _env.pop<bool>();
			int unmatched_a = _env.pop<CodeAtom>(block_a, 1);
			int unmatched_b = _env.pop<CodeAtom>(block_b, 1);

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
					_env.push<CodeAtom>(block_a);

				if (block_b.size() > 0)
					_env.push<CodeAtom>(block_b);
			}
		}

		return 1;
	}

	unsigned code_insert(Environment & _env)
	{
		//if ((_env.has_elements<long>(1)) && (_env.has_elements<CodeAtom>(2)))
		//{
		//	int index = std::abs(_env.pop<long>());	// index

		//	Utilities::FixedSizeStack<Atom> top_block_1;
		//	Utilities::FixedSizeStack<Atom> top_block_2;
		//	Utilities::FixedSizeStack<Atom> sub_block;
		//	Utilities::FixedSizeStack<Atom> block_copy;

		//	// Get first block from stack
		//	_env.pop<CodeAtom>(top_block_1, 1);

		//	// Get second block from stack
		//	_env.pop<CodeAtom>(top_block_2, 1);

		//	int number_of_blocks = _env.CodeLength(top_block_1);

		//	// Take modulo the number of blocks to ensure that it is within the meaningful range.
		//	index = std::abs(index % number_of_blocks);

		//	if (index == 0)
		//	{
		//		_env.push<CodeAtom>(top_block_1);
		//		_env.push<CodeAtom>(top_block_2);
		//		_env.push<CodeAtom>(CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"));
		//	}

		//	else
		//	{
		//		// Save copy of the top block.
		//		block_copy = top_block_1;

		//		// Get count of sub-blocks
		//		//int number_of_blocks = 0;
		//		//int n = 0;

		//		//do
		//		//{
		//		//	int blocks_open = 2;

		//		//	for (; n < top_block_1.size(); n++)
		//		//	{
		//		//		Plush::Atom atom = top_block_1[n];

		//		//		blocks_open += Plush::Func2BlockWantsMap[atom.instruction];
		//		//		blocks_open -= atom.close_parentheses;

		//		//		if ((atom.close_parentheses > 0) && (blocks_open > 0))
		//		//		{
		//		//			blocks_open++;
		//		//			number_of_blocks++;
		//		//		}

		//		//		if (blocks_open <= 0)
		//		//			break;
		//		//	};

		//		//	if (blocks_open <= 0)
		//		//		break;

		//		//} while (n < top_block_1.size());

		//		// Restore sub-block from copy
		//		top_block_1 = block_copy;

		//		// Get the target sub-block
		//		int n = 0;
		//		int block_number = 0;

		//		do
		//		{
		//			int blocks_open = 2;
		//			sub_block.clear();

		//			for (; n < top_block_1.size(); n++)
		//			{
		//				Plush::Atom atom = top_block_1[n];
		//				sub_block.push(atom);

		//				blocks_open += Plush::Func2BlockWantsMap[atom.instruction];
		//				blocks_open -= atom.close_parentheses;

		//				if ((atom.close_parentheses > 0) && (blocks_open > 0))
		//				{
		//					blocks_open++;
		//					block_number++;

		//					if (block_number == index)
		//						for (int j = 0; j < top_block_2.size(); j++)
		//							sub_block.push(top_block_2[j]);
		//				}

		//				if (blocks_open <= 0)
		//					break;
		//			};

		//			if (blocks_open <= 0)
		//				break;

		//		} while ((block_number < number_of_blocks) && (n < top_block_1.size()));

		//		if (index == 0)
		//			for (int j = 0; j < top_block_2.size(); j++)
		//				sub_block.push(top_block_2[j]);

		//		_env.push<CodeAtom>(sub_block);
		//	}
		//}

		return 1;
	}

	inline unsigned code_length(Environment & _env)
	{
		//if (_env.has_elements<CodeAtom>(1))
		//{
		//	Utilities::FixedSizeStack<Atom> top_block;

		//	// Get top block from stack
		//	_env.pop<CodeAtom>(top_block, 1);

		//	// Get count of sub-blocks
		//	//int number_of_blocks = 0;
		//	//int n = 0;

		//	//do
		//	//{
		//	//	int blocks_open = 1;

		//	//	for (; n < top_block.size(); n++)
		//	//	{
		//	//		Plush::Atom atom = top_block[n];

		//	//		blocks_open += Plush::Func2BlockWantsMap[atom.instruction];
		//	//		blocks_open -= atom.close_parentheses;

		//	//		if (atom.close_parentheses > 0)
		//	//		{
		//	//			if (blocks_open <= 1)
		//	//				number_of_blocks++;

		//	//			if (blocks_open > 0)
		//	//				blocks_open++;
		//	//		}

		//	//		else if (blocks_open <= 1)
		//	//			number_of_blocks++;

		//	//		if (blocks_open <= 0)
		//	//			break;
		//	//	};

		//	//	if (blocks_open <= 0)
		//	//		break;

		//	//} while (n < top_block.size());

		//	int number_of_blocks = Genome::CodeLength(top_block);

		//	_env.push<long>(number_of_blocks);
		//}

		return 1;
	}

	unsigned code_list(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(2))
		{
			Utilities::FixedSizeStack<Atom> block_a;
			Utilities::FixedSizeStack<Atom> block_b;

			int unmatched_a = _env.pop<CodeAtom>(block_a, 1);
			int unmatched_b = _env.pop<CodeAtom>(block_b, 1);

			if ((block_a.size() > 0) && (block_b.size() > 0))
			{
				// Add one closing paranthesis to block B before pushing the blocks back on stack
				Atom atom = block_b.top();
				block_b.pop();

				atom.close_parentheses++;
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

	unsigned code_member(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(2))
		{
			Utilities::FixedSizeStack<Atom> block_a;
			Utilities::FixedSizeStack<Atom> block_b;

			_env.pop<CodeAtom>(block_a, 1);
			_env.pop<CodeAtom>(block_b, 1);

			if ((block_b.size() > 0) && (block_a.size() > 0))
			{
				// Make sure last atom of block B is closed
				Atom atom = block_a.top();
				block_a.pop();

				atom.close_parentheses = (atom.close_parentheses == 0) ? 1 : atom.close_parentheses;
				block_a.push(atom);

				bool found = false;
				int index = 0;

				for (int i = 1; i < (block_b.size() - block_a.size() + 1); i++)
				{
					found = true;

					for (int j = 0; j < block_a.size(); j++)
					{
						if (block_b[i + j].instruction != block_a[j].instruction)
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

	unsigned code_nth(Environment & _env)
	{
		if ((_env.has_elements<long>(1)) && (_env.has_elements<CodeAtom>(1)))
		{
			int index = std::abs(_env.pop<long>());	// index

			Utilities::FixedSizeStack<Atom> top_block;
			Utilities::FixedSizeStack<Atom> extracted_block;
			Utilities::FixedSizeStack<Atom> block_without_extracted;
			Utilities::FixedSizeStack<Atom> block_copy;

			if (index != 0)
			{
				// Get first block from stack
				_env.pop<CodeAtom>(top_block, 1);

				// Get length of top block


















				// Save copy of the top block.
				block_copy = top_block;

				// Get count of sub-blocks
				int number_of_blocks = 0;
				int n = 0;

				do
				{
					int blocks_open = 1;

					for (; n < top_block.size(); n++)
					{
						Plush::Atom atom = top_block[n];

						blocks_open += Plush::Func2BlockWantsMap[atom.instruction];
						blocks_open -= atom.close_parentheses;

						if (atom.close_parentheses > 0)
						{
							if (blocks_open > 0)
								blocks_open++;

							number_of_blocks++;
						}

						if (blocks_open <= 0)
							break;
					};

					if (blocks_open <= 0)
						break;

				} while (n < top_block.size());

				// Take modulo the number of blocks to ensure that it is within the meaningful range.
				index = (std::abs(index) - 1) % number_of_blocks;

				// Restore sub-block from copy
				top_block = block_copy;

				// Get the target sub-block
				n = 0;
				int block_number = 0;

				do
				{
					int blocks_open = 1;
					bool first_element_of_block = false;

					for (; n < top_block.size(); n++)
					{
						Plush::Atom atom = top_block[n];

						if (Plush::Func2BlockWantsMap[atom.instruction] > 0)
							first_element_of_block = true;

						if ((block_number == index) && (!first_element_of_block))
							extracted_block.push(atom);

						else
							block_without_extracted.push(atom);

						first_element_of_block = false;

						blocks_open += Plush::Func2BlockWantsMap[atom.instruction];
						blocks_open -= atom.close_parentheses;

						if (atom.close_parentheses > 0)
						{
							if (blocks_open > 0)
								blocks_open++;

							block_number++;
						}

						if (blocks_open <= 0)
							break;
					};

					if (blocks_open <= 0)
						break;

				} while ((block_number < number_of_blocks) && (n < top_block.size()));

				_env.push<CodeAtom>(extracted_block);
			}
		}

		return 1;
	}

	unsigned code_nthcdr(Environment & _env)
	{
		if ((_env.has_elements<long>(1)) && (_env.has_elements<CodeAtom>(1)))
		{
			int index = std::abs(_env.pop<long>());	// index

			Genome<class CodeAtom> top_block;
			Genome<class CodeAtom> left_half;
			Genome<class CodeAtom> right_half;
			Utilities::FixedSizeStack<long> wanted_stack;

			if (index != 0)
			{
				// Get first block from stack
				_env.pop(top_block);

				// Get count items in first block
				int number_of_items = top_block.length();

				// Take modulo the number of blocks to ensure that it is within the meaningful range.
				index = std::abs(index) % number_of_items;

				// Remove unwanted items
				top_block.split(left_half, right_half, index);

				// Push remaining items back on the code stack
				_env.push(right_half);
				
				//		// Get the target sub-block
		//		int n = 0;
		//		int block_number = 0;
		//		int blocks_wanted = 0;

		//		do
		//		{
		//			for (; n < top_block.size(); n++)
		//			{
		//				Plush::Atom atom = top_block[n];

		//				if (block_number <= index)
		//					excluded_code.push(atom);

		//				else
		//					included_code.push(atom);

		//				if ((atom.close_parentheses > 0) && (wanted_stack.size() == 0))
		//					block_number += atom.close_parentheses;

		//				int new_blocks_wanted = Plush::Func2BlockWantsMap[atom.instruction] - atom.close_parentheses;

		//				if (new_blocks_wanted > 0)
		//				{
		//					wanted_stack.push(blocks_wanted);
		//					blocks_wanted = new_blocks_wanted;
		//				}

		//				while (new_blocks_wanted < 0)
		//				{
		//					new_blocks_wanted++;
		//					blocks_wanted--;

		//					if (blocks_wanted == 0)
		//					{
		//						blocks_wanted = wanted_stack.top();
		//						wanted_stack.pop();
		//					}
		//				}
		//			};

		//		} while ((block_number < number_of_blocks) && (n < top_block.size()));

		//		if (included_code.size() > 1)
		//		{
		//			for (int n = 1; n < excluded_code.size(); n++)
		//				extracted_block_cdr.push(included_code[n]);

		//			_env.push<CodeAtom>(extracted_block_cdr);
		//		}
			}
		}

		return 1;
	}

	inline unsigned code_null(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(1))
		{
			Utilities::FixedSizeStack<Atom> top_block;

			// Get first block from stack
			_env.pop<CodeAtom>(top_block, 1);

			if (top_block.size() == 0)
				_env.push<bool>(true);
			
			else if ((top_block.size() == 1) 
				&& (top_block.top().instruction == "CODE.NOOP")
					&& (top_block.top().close_parentheses > 0)) 
					_env.push<bool>(true);

			else
				_env.push<bool>(false);
		}

		return 1;
	}

	unsigned code_position(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(2))
		{
			Utilities::FixedSizeStack<Atom> block_a;
			Utilities::FixedSizeStack<Atom> block_b;

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

				for (int i = 1; i < (block_a.size() - block_b.size() + 1); i++)
				{
					found = true;

					for (int j = 0; j < block_b.size(); j++)
					{
						if (block_a[i + j].instruction != block_b[j].instruction)
						{
							found = false;
							break;
						}
					}

					if (found)
					{
						index = i;
						break;
					}
				}

				if (found)
					_env.push<long>(index);

				else
					_env.push<long>(-1);
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

	unsigned code_quote(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(1))
		{
			Utilities::FixedSizeStack<Atom> block_a;

			_env.pop<ExecAtom>(block_a, 1);
			_env.push<CodeAtom>(block_a);
		}

		return 1;
	}

	inline unsigned code_size(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(1))
		{
			Utilities::FixedSizeStack<Atom> top_block;

			// Get top block from stack
			_env.pop<CodeAtom>(top_block, 1);

			// Get count of atoms
			int size = top_block.size();

			_env.push<long>(size);
		}

		return 1;
	}

	inline unsigned code_subst(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(3))
		{
			Utilities::FixedSizeStack<CodeAtom> &stack = _env.get_stack<CodeAtom>();
			Utilities::FixedSizeStack<Atom> extracted_block_A;
			Utilities::FixedSizeStack<Atom> extracted_block_B;
			Utilities::FixedSizeStack<Atom> extracted_block_C;
			Utilities::FixedSizeStack<Atom> block_without_extracted;
			Utilities::FixedSizeStack<Atom> modified_block;

			// Get count of sub-blocks
			int number_of_blocks = 0;
			int n = stack.size() - 1;

			n = stack.size() - 1;
			int block_number = 0;

			// Pop first three blocks
			do
			{
				int blocks_open = 1;

				for (; n >= 0; n--)
				{
					Plush::Atom atom = _env.pop<CodeAtom>(); //stack[n];

					if (block_number == 0)
						extracted_block_A.push(atom);

					else if (block_number == 1)
						extracted_block_B.push(atom);

					else if (block_number == 2)
						extracted_block_C.push(atom);

					else
						block_without_extracted.push(atom);

					blocks_open += Plush::Func2BlockWantsMap[atom.instruction];
					blocks_open -= atom.close_parentheses;
					blocks_open = (blocks_open > 0) ? blocks_open : 0;

					if (atom.close_parentheses > 0)
					{
						if (blocks_open > 0)
							blocks_open++;

						else
						{
							block_number++;
							blocks_open = 1;
						}
					}
				};
			} while (n >= 0);

			//  To name the first, second and third items from :code 1, 2 and 3. Replace all 3 in 1 with 2.
			if ((extracted_block_A.size() > 0) && (extracted_block_B.size() > 0) && (extracted_block_C.size() > 0))
			{
				// Make sure last atom of block B and C are closed
				Atom atom = extracted_block_B.top();
				extracted_block_B.pop();
				atom.close_parentheses = (atom.close_parentheses == 0) ? 1 : atom.close_parentheses;
				extracted_block_B.push(atom);

				atom = extracted_block_C.top();
				extracted_block_C.pop();
				atom.close_parentheses = (atom.close_parentheses == 0) ? 1 : atom.close_parentheses;
				extracted_block_C.push(atom);

				// Find block 3 in block 1
				bool found = false;
				int index = 0;

				for (int i = 0; i < (extracted_block_A.size() - extracted_block_C.size() + 1); i++)
				{
					found = true;

					for (int j = 0; j < extracted_block_C.size(); j++)
					{
						if (extracted_block_A[i + j].instruction != extracted_block_C[j].instruction)
						{
							found = false;
							break;
						}
					}

					if (found)
					{
						// Replace block 3 with block 2 in block 1.
						for (int j = 0; j < extracted_block_B.size(); j++)
							modified_block.push(extracted_block_B[j]);

						i += extracted_block_C.size() - 1;
					}
					else
						modified_block.push(extracted_block_A[i]);
				}

				_env.get_stack<CodeAtom>().clear();
				_env.push<CodeAtom>(block_without_extracted);
				_env.push<CodeAtom>(modified_block);
			}

			else
			{
				_env.get_stack<CodeAtom>().clear();
				_env.push<CodeAtom>(block_without_extracted);
				_env.push<CodeAtom>(extracted_block_C);
				_env.push<CodeAtom>(extracted_block_B);
				_env.push<CodeAtom>(extracted_block_A);
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

		make_instruction((Operator)exec_do_range, "EXEC", "DO*RANGE");
		make_instruction((Operator)noop_open_paren, "EXEC", "NOOP_OPEN_PAREN");
		make_instruction((Operator)noop, "EXEC", "NOOP");
		make_instruction((Operator)exec_if, "EXEC", "IF");
		make_instruction((Operator)exec_do_count, "EXEC", "DO*COUNT");
		make_instruction((Operator)exec_do_times, "EXEC", "DO*TIMES");
		make_instruction((Operator)exec_k, "EXEC", "K");
		make_instruction((Operator)exec_s, "EXEC", "S");
		make_instruction((Operator)exec_y, "EXEC", "Y");

		set_parentheses("EXEC", "DO*COUNT", 1);
		set_parentheses("EXEC", "DO*RANGE", 1);
		set_parentheses("EXEC", "DO*TIMES", 1);
		set_parentheses("EXEC", "DUP", 1);
		set_parentheses("EXEC", "=", 2);
		set_parentheses("EXEC", "IF", 2);
		set_parentheses("EXEC", "K", 2);
		set_parentheses("EXEC", "NOOP_OPEN_PAREN", 1);
		set_parentheses("EXEC", "POP", 1);
		set_parentheses("EXEC", "ROT", 3);
		set_parentheses("EXEC", "S", 3);
		set_parentheses("EXEC", "SHOVE", 1);
		set_parentheses("EXEC", "SWAP", 2);
		set_parentheses("EXEC", "Y", 1);

		make_instruction((Operator)code_append, "CODE", "APPEND");
		make_instruction((Operator)code_atom, "CODE", "ATOM");
		make_instruction((Operator)code_car, "CODE", "CAR");
		make_instruction((Operator)code_cdr, "CODE", "CDR");
		make_instruction((Operator)code_cons, "CODE", "CONS");
		make_instruction((Operator)code_container, "CODE", "CONTAINER");
		make_instruction((Operator)code_contains, "CODE", "CONTAINS");
		make_instruction((Operator)code_discrepancy, "CODE", "DISCREPANCY");
		make_instruction((Operator)code_do, "CODE", "DO");
		make_instruction((Operator)code_do_star, "CODE", "DO*");
		make_instruction((Operator)code_do_range, "CODE", "DO*RANGE");
		make_instruction((Operator)code_do_count, "CODE", "DO*COUNT");
		make_instruction((Operator)code_do_times, "CODE", "DO*TIMES");
		make_instruction((Operator)code_extract, "CODE", "EXTRACT");
		make_instruction((Operator)bool2code, "CODE", "FROMBOOLEAN");
		make_instruction((Operator)float2code, "CODE", "FROMFLOAT");
		make_instruction((Operator)int2code, "CODE", "FROMINTEGER");
		make_instruction((Operator)code_if, "CODE", "IF");
		make_instruction((Operator)code_insert, "CODE", "INSERT");
		make_instruction((Operator)code_length, "CODE", "LENGTH");
		make_instruction((Operator)code_list, "CODE", "LIST");
		make_instruction((Operator)code_member, "CODE", "MEMBER");
		make_instruction((Operator)code_nth, "CODE", "NTH");
		make_instruction((Operator)code_nthcdr, "CODE", "NTHCDR");
		make_instruction((Operator)code_null, "CODE", "NULL");
		make_instruction((Operator)code_position, "CODE", "POSITION");
		make_instruction((Operator)code_quote, "CODE", "QUOTE");
		make_instruction((Operator)code_size, "CODE", "SIZE");
		make_instruction((Operator)code_subst, "CODE", "SUBST");

//		set_parentheses("CODE", "NOOP_OPEN_PAREN", 1);
		set_parentheses("CODE", "QUOTE", 1);
	}
}