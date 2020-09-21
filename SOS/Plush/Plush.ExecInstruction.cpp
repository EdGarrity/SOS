#include <set>
#include <stack>
#include "Genome.h"
#include "Plush.ExecInstruction.h"
#include "Processor.h"
#include "Plush.StaticInit.h"
#include "..\Utilities\String.h"

namespace Plush
{
	// Expand inner block
	unsigned noop_open_paren(Environment & _env)
	{
		if (_env.has_elements<ExecAtom>(1))
		{
			// Get top atom on EXEC stack
			ExecAtom atom = _env.get_top<ExecAtom>();

			// Remove one closing paranthesis
			atom.close_parenthesis = (atom.close_parenthesis > 0) ? atom.close_parenthesis - 1 : atom.close_parenthesis;

			// Set top atom
			_env.set_top<ExecAtom>(atom);
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
			int n = _env.pop<long>();	// destination index
			int i = _env.pop<long>();	// current index

			if (n == i)
				_env.push<long>(i);

			else
			{
				int direction = 1;

				if (i > n)
					direction = -1;

				// Check that the EXEC stack can hold another copy of Block A
				int s = _env.get_stack<ExecAtom>().number_of_atoms();

				if (_env.get_stack<ExecAtom>().free() > (s * 2))
				{
					_env.push<long>(i + direction);
					_env.push<long>(n);

					_env.get_stack<ExecAtom>().shove(_env.get_stack<ExecAtom>(), 0, 0, s);

					_env.push<ExecAtom>(Atom("{:instruction EXEC.DO*RANGE :close 0}"));
				}
				else
					_env.push<long>(i);
			}
		}

		return 1;
	}

	unsigned exec_if(Environment & _env)
	{
		if ((_env.has_elements<bool>(1)) && (_env.has_elements<ExecAtom>(2)))
		{
			// Get conditional boolean
			bool s = _env.pop<bool>();

			if (s)
				_env.get_stack<ExecAtom>().remove_item(1);
			else
				_env.get_stack<ExecAtom>().remove_item(0);
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

				_env.push<ExecAtom>(Atom("{:instruction EXEC.DO*RANGE :close 0}"));
			}
		}

		return 1;
	}

	unsigned exec_do_times(Environment & _env)
	{
		if ((_env.has_elements<long>(1)) && (_env.has_elements<ExecAtom>(1)))
		{
			int n = _env.pop<long>();	// destination index

			if (n > 0)
			{
				_env.push<long>(0);
				_env.push<long>(n - 1);

				_env.push<ExecAtom>(Atom("{:instruction INTEGER.POP :close 0}"));
				_env.push<ExecAtom>(Atom("{:instruction EXEC.DO*RANGE :close 0}"));
			}
		}

		return 1;
	}

	unsigned exec_while(Environment & _env)
	{
		if ((_env.has_elements<bool>(1)) && (_env.has_elements<ExecAtom>(1)))
		{
			bool flag = _env.pop<bool>();

			if (flag)
			{
				_env.push<ExecAtom>(Atom("{:instruction EXEC.WHILE :close 0}"));
				_env.get_stack<ExecAtom>().yankdup_item(1);
			}
		}

		return 1;
	}

	unsigned do_while(Environment & _env)
	{
		if (_env.has_elements<ExecAtom>(1))
		{
			_env.push<ExecAtom>(Atom("{:instruction EXEC.WHILE :close 0}"));
			_env.get_stack<ExecAtom>().yankdup_item(1);
		}

		return 1;
	}

	unsigned exec_when(Environment & _env)
	{
		if ((_env.has_elements<bool>(1)) && (_env.has_elements<ExecAtom>(1)))
		{
			bool flag = _env.pop<bool>();

			if (!flag)
				_env.get_stack<ExecAtom>().remove_item(0);
		}

		return 1;
	}

	unsigned exec_k(Environment & _env)
	{
		if (_env.has_elements<ExecAtom>(2))
			_env.get_stack<ExecAtom>().remove_item(1);

		return 1;
	}

	unsigned exec_s(Environment & _env)
	{
		if (_env.has_elements<ExecAtom>(3))
		{
			//Genome<ExecAtom> block_a;
			//Genome<ExecAtom> block_b;
			//Genome<ExecAtom> block_c;

			// Pop three blocks from stack
			//_env.pop<ExecAtom>(block_a);
			//_env.pop<ExecAtom>(block_b);
			//_env.pop<ExecAtom>(block_c);

			//// Push a list containing B and C back onto the EXEC stack
			//_env.push<ExecAtom>(Atom("{:instruction EXEC.NOOP :close 1}"));
			//_env.push<ExecAtom>(block_b);
			//_env.push<ExecAtom>(block_c);
			//_env.push<ExecAtom>(Atom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"));

			//// Followed by another instance of C 
			//_env.push<ExecAtom>(block_c);

			//// Followed by another instance of A
			//_env.push<ExecAtom>(block_a);

			// A = 0
			// B = 1
			// C = 2

			// Push a list containing B and C back onto the EXEC stack
			_env.push<ExecAtom>(Atom("{:instruction EXEC.NOOP :close 1}"));

			// A = 1
			// B = 2
			// C = 3

			//_env.push<ExecAtom>(block_b);
			_env.push<ExecAtom>(Atom("{:instruction EXEC.NOOP :close 1}"));

			// A = 2
			// B = 3
			// C = 4

			_env.get_stack<ExecAtom>().yankdup_item(3);

			// A = 3
			// B = 4
			// C = 5

			//_env.push<ExecAtom>(block_c);
			_env.push<ExecAtom>(Atom("{:instruction EXEC.NOOP :close 1}"));

			// A = 4
			// B = 5
			// C = 6

			_env.get_stack<ExecAtom>().yankdup_item(6);

			// A = 5
			// B = 6
			// C = 7

			_env.push<ExecAtom>(Atom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"));

			// A = 6
			// B = 7
			// C = 8

			// Followed by another instance of C 
			//_env.push<ExecAtom>(block_c);
			_env.get_stack<ExecAtom>().yankdup_item(8);

			// A = 7
			// B = 8
			// C = 9

			// Followed by another instance of A
			//_env.push<ExecAtom>(block_a);
			_env.get_stack<ExecAtom>().yankdup_item(7);
		}

		return 1;
	}

	unsigned exec_y(Environment & _env)
	{
		if (_env.has_elements<ExecAtom>(1))
		{
			Genome<ExecAtom> extracted_block;

			_env.pop<ExecAtom>(extracted_block);

			_env.push<ExecAtom>(extracted_block);
			_env.push<ExecAtom>(Atom("{:instruction EXEC.Y :close 0}"));
			_env.push<ExecAtom>(extracted_block);
		}

		return 1;
	}

	unsigned code_append(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(2))
		{
			Genome<CodeAtom> block_a;
			Genome<CodeAtom> block_b;

			_env.pop(block_a);
			_env.pop(block_b);

			block_a.bottom().close_parenthesis
				= (block_a.bottom().close_parenthesis > 0)
				? block_a.bottom().close_parenthesis - 1
				: block_a.bottom().close_parenthesis;

			block_b.bottom().close_parenthesis
				= (block_b.bottom().close_parenthesis == 0)
				? 1 
				: block_b.bottom().close_parenthesis;

			_env.push<CodeAtom>(block_b);
			_env.push<CodeAtom>(block_a);
		}

		return 1;
	}

	unsigned code_atom(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(1))
		{
			Genome<CodeAtom> block;

			_env.pop<CodeAtom>(block);

			if (block.size() == 1)
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
			Genome<CodeAtom> top_block;
			_env.pop<CodeAtom>(top_block);

			if (top_block.size() > 1)
			{
				Genome<CodeAtom> first_item;
				top_block.pop_item(first_item);
				first_item.bottom().close_parenthesis++;
				_env.push<CodeAtom>(first_item);
			}

			else
				_env.push<CodeAtom>(top_block);
		}

		return 1;
	}

	unsigned code_cdr(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(1))
		{
			Genome<CodeAtom> top_block;
			_env.pop<CodeAtom>(top_block);

			if (top_block.size() > 1)
			{
				Genome<CodeAtom> first_item;
				top_block.pop_item(first_item);
				_env.push<CodeAtom>(top_block);
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
			Genome<CodeAtom> block_a;
			Genome<CodeAtom> block_b;

			_env.pop<CodeAtom>(block_a);
			_env.pop<CodeAtom>(block_b);

			block_b.bottom().close_parenthesis--;
			
			_env.push<CodeAtom>(block_a);
			_env.push<CodeAtom>(block_b);
		}

		return 1;
	}

	unsigned code_container(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(2))
		{
			Genome<CodeAtom> extracted_block_A;
			Genome<CodeAtom> extracted_block_B;
			Genome<CodeAtom> modified_block_A;
			Genome<CodeAtom> container_block;

			// Get first block from stack
			_env.pop<CodeAtom>(extracted_block_A);

			// Get second block from stack
			_env.pop<CodeAtom>(extracted_block_B);

			if (extracted_block_A.container_of(extracted_block_B, container_block))
				_env.push<CodeAtom>(container_block);

			else
				_env.push<CodeAtom>(CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 1}"));
		}

		return 1;
	}

	unsigned code_contains(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(2))
		{
			Genome<CodeAtom> extracted_block_A;
			Genome<CodeAtom> extracted_block_B;
			Genome<CodeAtom> modified_block_A;

			// Get first block from stack
			_env.pop<CodeAtom>(extracted_block_A);

			// Get second block from stack
			_env.pop<CodeAtom>(extracted_block_B);

			bool found = (extracted_block_B.contains(extracted_block_A) >= 0);
			_env.push<bool>(found);
		}

		return 1;
	}

	unsigned code_discrepancy(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(2))
		{
			Genome<CodeAtom> block_a;
			Genome<CodeAtom> block_b;

			_env.pop<CodeAtom>(block_a);
			_env.pop<CodeAtom>(block_b);

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
		}

		return 1;
	}

	unsigned code_do(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(1))
		{
			Genome<CodeAtom> block_a;

			_env.pop<CodeAtom>(block_a);
			_env.push<CodeAtom>(block_a);
			_env.push<ExecAtom>(Atom("{:instruction CODE.POP :close 1}"));
			_env.push<ExecAtom>(block_a);

			return 1;
		}
	}

	unsigned code_do_star(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(1))
		{
			Genome<CodeAtom> block_a;

			_env.pop(block_a);
			_env.push<ExecAtom>(block_a);

			return 1;
		}
	}

	unsigned code_do_range(Environment & _env)
	{
		if ((_env.has_elements<long>(2)) && (_env.has_elements<CodeAtom>(1)))
		{
			Genome<CodeAtom> block_a;

			int n = _env.pop<long>();	// destination index
			int i = _env.pop<long>();	// current index

			if (n == i)
			{
				_env.push<long>(i);
				_env.pop(block_a);
				_env.push<ExecAtom>(block_a);
			}

			else
			{
				int direction = 1;

				if (i > n)
					direction = -1;

				_env.pop<CodeAtom>(block_a);

				if (_env.get_stack<ExecAtom>().free() > (block_a.size() * 2))
				{
					_env.push<long>(i + direction);
					_env.push<long>(n);

					_env.push<CodeAtom>(block_a);
					_env.push<ExecAtom>(block_a);

					_env.push<ExecAtom>(Atom("{:instruction CODE.DO*RANGE :close 1}"));
				}
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

				_env.push<ExecAtom>(Atom("{:instruction CODE.DO*RANGE :close 1}"));
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

				_env.push<ExecAtom>(Atom("{:instruction INTEGER.POP :close 1}"));
				_env.push<ExecAtom>(Atom("{:instruction CODE.DO*RANGE :close 1}"));
			}
		}

		return 1;
	}

	unsigned code_extract(Environment & _env)
	{
		if ((_env.has_elements<long>(1)) && (_env.has_elements<CodeAtom>(1)))
		{
			int index = std::abs(_env.pop<long>());	// index

			//unsigned int extra_first_blocks = 0;
			//unsigned int extra_second_bloxks = 0;

			Genome<CodeAtom> first_block;
			//Genome<CodeAtom> second_block;
			Genome<CodeAtom> left_half;
			Genome<CodeAtom> right_half;
			Genome<CodeAtom> indexed_block;
			Genome<CodeAtom> rest_block;

			//Genome<CodeAtom> top_block;
			//Genome<CodeAtom> extracted_block;
			//Genome<CodeAtom> block_without_extracted;
			//Genome<CodeAtom> block_copy;

			if (index != 0)
			{
				// Get top block
				_env.pop(first_block);

				// Get count items in first block
				int number_of_items = first_block.number_of_items();

				if (number_of_items > 0)
				{
					// Take modulo the number of blocks to ensure that it is within the meaningful range.
					index = std::abs(index - 1) % number_of_items;

					// Split block at insertion point
					first_block.split(left_half, right_half, index, Genome<CodeAtom>::SPLIT_MODE::item);

					// Compensate for extra blocks in the second item.
					//if (extra_second_bloxks > 0)
					//	right_half.bottom().close_parenthesis += extra_second_bloxks;

					// Get indexed block
					right_half.split(indexed_block, rest_block, 1, Genome<CodeAtom>::SPLIT_MODE::item);

					// Close extracted item
					if ((indexed_block.get_top_ref().instruction == "EXEC.NOOP_OPEN_PAREN") 
						&& (indexed_block.get_top_ref().close_parenthesis == 0)
						)
						indexed_block.bottom().close_parenthesis = 2;
					else
						indexed_block.bottom().close_parenthesis = 1;

					// Replace top of stack with extracted item
					_env.push<CodeAtom>(indexed_block);
				}
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
			Genome<CodeAtom> extracted_block_A;
			Genome<CodeAtom> extracted_block_B;

			// Get conditional boolean
			bool s = _env.pop<bool>();

			// Get first block from stack
			_env.pop<CodeAtom>(extracted_block_A);

			// Get second block from stack
			_env.pop<CodeAtom>(extracted_block_B);

			if (s)
				_env.push<ExecAtom>(extracted_block_A);

			else
				_env.push<ExecAtom>(extracted_block_B);
		}

		return 1;
	}

	unsigned code_insert(Environment & _env)
	{
		if ((_env.has_elements<long>(1)) && (_env.has_elements<CodeAtom>(2)))
		{
			int index = std::abs(_env.pop<long>());	// index

			Genome<CodeAtom> first_block;
			Genome<CodeAtom> second_block;
			Genome<CodeAtom> left_half;
			Genome<CodeAtom> right_half;
			
			// Get first block from stack
			_env.pop<CodeAtom>(first_block);

			// Get second block from stack
			_env.pop<CodeAtom>(second_block);

			// Put the second block in a list object
			second_block.push(CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"));

			if (index == 0)
			{
				// Insert secoond item into first item at position 0.
				_env.push<CodeAtom>(first_block);
				_env.push<CodeAtom>(second_block);
			}
			else
			{
				// Get count items in first block
				int number_of_items = first_block.number_of_items();

				// Take modulo the number of blocks to ensure that it is within the meaningful range.
				index = std::abs(index) % number_of_items;

				// Split block at insertion point
				first_block.split(left_half, right_half, index, Genome<CodeAtom>::SPLIT_MODE::item);

				// Insert second block into first block at insertion point
				_env.push<CodeAtom>(right_half);
				_env.push<CodeAtom>(second_block);
				_env.push<CodeAtom>(left_half);
			}
		}

		return 1;
	}

	inline unsigned code_length(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(1))
		{
			Genome<CodeAtom> top_block;

			// Get first block from stack
			_env.pop<CodeAtom>(top_block);

			// Get count items in first block
			int number_of_items = top_block.number_of_items();
			_env.push<long>(number_of_items);
		}

		return 1;
	}

	unsigned code_list(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(2))
		{
			Genome<CodeAtom> extracted_block_A;
			Genome<CodeAtom> extracted_block_B;

			_env.pop<CodeAtom>(extracted_block_A);

			// Get second block from stack
			_env.pop<CodeAtom>(extracted_block_B);

			// Close combined list
			extracted_block_B.bottom().close_parenthesis++;

			_env.push<CodeAtom>(extracted_block_B);
			_env.push<CodeAtom>(CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"));
			_env.push<CodeAtom>(extracted_block_A);
			_env.push<CodeAtom>(CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"));
		}

		return 1;
	}

	unsigned code_member(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(2))
		{
			Genome<CodeAtom> extracted_block_A;
			Genome<CodeAtom> extracted_block_B;

			// Get first block from stack
			_env.pop<CodeAtom>(extracted_block_A);

			// Get second block from stack
			_env.pop<CodeAtom>(extracted_block_B);

			bool found = (extracted_block_A.contains(extracted_block_B) >= 0);
			_env.push<bool>(found);
		}

		return 1;
	}

	unsigned code_nth(Environment & _env)
	{
		if ((_env.has_elements<long>(1)) && (_env.has_elements<CodeAtom>(1)))
		{
			int index = std::abs(_env.pop<long>());	// index

			Genome<CodeAtom> top_block;
			Genome<CodeAtom> left_half;
			Genome<CodeAtom> right_half;

			// Get first block from stack
			_env.pop(top_block);

			if (top_block.size() == 0)
				_env.push<CodeAtom>(top_block);

			else
			{
				// Get count items in first block
				int number_of_items = top_block.number_of_items();

				// Take modulo the number of blocks to ensure that it is within the meaningful range.
				index = std::abs(index) % number_of_items;

				if (index == 0)
				{
					// Extract first item
					top_block.split(left_half, right_half, 1, Genome<CodeAtom>::SPLIT_MODE::item);

					// Push extracted item back on the code stack
					if ((index + 1) != number_of_items)
						left_half.bottom().close_parenthesis++;

					_env.push<CodeAtom>(left_half);
				}

				else
				{
					// Extract item N
					top_block.split(left_half, right_half, index, Genome<CodeAtom>::SPLIT_MODE::item);
					left_half.clear();
					right_half.split(left_half, right_half, 1, Genome<CodeAtom>::SPLIT_MODE::item);

					// Push extracted item back on the code stack
					if ((index + 1) != number_of_items)
						left_half.bottom().close_parenthesis++;
	
					_env.push<CodeAtom>(left_half);
				}
			}
		}
		
		return 1;
	}

	unsigned code_nthcdr(Environment & _env)
	{
		if ((_env.has_elements<long>(1)) && (_env.has_elements<CodeAtom>(1)))
		{
			int index = std::abs(_env.pop<long>());	// index

			Genome<CodeAtom> extracted_block_A;
			Genome<CodeAtom> left_half;
			Genome<CodeAtom> right_half;

			// Get first block from stack
			_env.pop<CodeAtom>(extracted_block_A);

			// Get count items in first block
			unsigned int extracted_block_A_size = extracted_block_A.number_of_items();

			// Take modulo the number of blocks to ensure that it is within the meaningful range.
			index = std::abs(index) % extracted_block_A_size;

			// Remove unwanted items
			extracted_block_A.split(left_half, right_half, index, Genome<CodeAtom>::SPLIT_MODE::item);

			// Push remaining items back on the code stack
			_env.push<CodeAtom>(right_half);
		}

		return 1;
	}

	inline unsigned code_null(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(1))
		{
			Genome<CodeAtom> top_block;

			// Get first block from stack
			_env.pop<CodeAtom>(top_block);

			if (top_block.size() == 0)
				_env.push<bool>(true);
			
			else if ((top_block.size() == 1) 
				&& (top_block.get_top().instruction == "CODE.NOOP")
					&& (top_block.get_top().close_parenthesis > 0))
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
			Genome<CodeAtom> extracted_block_A;
			Genome<CodeAtom> extracted_block_B;

			// Get first block from stack
			_env.pop<CodeAtom>(extracted_block_A);

			// Get second block from stack
			_env.pop<CodeAtom>(extracted_block_B);

			int position = extracted_block_A.contains(extracted_block_B);
			_env.push<long>(position);
		}

		return 1;
	}

	unsigned code_quote(Environment & _env)
	{
		if (_env.has_elements<ExecAtom>(1))
		{
			Genome<ExecAtom> block_a;

			_env.pop<ExecAtom>(block_a);
			_env.push<CodeAtom>(block_a);
		}

		return 1;
	}

	inline unsigned code_size(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(1))
		{
			Genome<CodeAtom> top_block;

			// Get top block from stack
			_env.pop<CodeAtom>(top_block);

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
			Genome<CodeAtom> extracted_block_A;
			Genome<CodeAtom> extracted_block_B;
			Genome<CodeAtom> extracted_block_C;

			// Get first block from stack
			_env.pop<CodeAtom>(extracted_block_A);

			// Get second block from stack
			_env.pop<CodeAtom>(extracted_block_B);

			// Get second block from stack
			_env.pop<CodeAtom>(extracted_block_C);

			extracted_block_A.subst(extracted_block_B, extracted_block_C);

			_env.push<CodeAtom>(extracted_block_A);
		}

		return 1;
	}

	// Purpose: 
	//   Push the Nth element from the input array onto the Code stack.
	//
	// Parameters:
	//   None
	//
	// Return value:
	//   1
	//
	// Side Effects:
	//   If successful, a number is pushed to the Code stack
	//
	// Thread Safe:
	//   Yes
	//
	// Remarks:
	//   N is popped from the Integer stack first. If N < 0, or if N >= size of input array, or the Integer 
	//	 stack is empty then a NO-OP is executed instead.
	//
	unsigned in2code(Environment & _env)
	{
		long index = _env.pop<long>();

		index = std::abs((long)(index % _env.input.size()));
		double value = _env.input[index];
		_env.push<CodeAtom>(CodeAtom(value));

		return 1;
	}

	// Purpose: 
	//   Push all elements from the input array onto a stack.
	//
	// Parameters:
	//   None
	//
	// Return value:
	//   1
	//
	// Side Effects:
	//   If successful, all numbers are pushed to the a stack
	//
	// Thread Safe:
	//   Yes
	//
	// Remarks:
	//   if input array is empty, NO-OP is executed.
	//
	unsigned inall2code(Environment & _env)
	{
		if (_env.input.size() > 0)
		{
			for (int index = 0; index < _env.input.size(); index++)
			{
				double value = _env.input[index];
				_env.push<CodeAtom>(CodeAtom(value));
			}
		}

		return 1;
	}

	// Purpose: 
	//   Push all elements from the input array onto a stack in reverse order.
	//
	// Parameters:
	//   None
	//
	// Return value:
	//   1
	//
	// Side Effects:
	//   If successful, all numbers are pushed to a stack
	//
	// Thread Safe:
	//   Yes
	//
	// Remarks:
	//   if input array is empty, NO-OP is executed.
	//
	unsigned inallrev2code(Environment & _env)
	{
		if (_env.input.size() > 0)
		{
			for (int index = _env.input.size() - 1; index >= 0; index--)
			{
				double value = _env.input[index];
				_env.push<CodeAtom>(CodeAtom(value));
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

		make_instruction((Operator)exec_k, "EXEC", "K");
		make_instruction((Operator)exec_s, "EXEC", "S");
		make_instruction((Operator)exec_y, "EXEC", "Y");
		make_instruction((Operator)exec_if, "EXEC", "IF");
		make_instruction((Operator)exec_do_range, "EXEC", "DO*RANGE");
		make_instruction((Operator)exec_do_count, "EXEC", "DO*COUNT");
		make_instruction((Operator)exec_do_times, "EXEC", "DO*TIMES");
		make_instruction((Operator)exec_while, "EXEC", "WHILE");
		make_instruction((Operator)do_while, "EXEC", "DO*WHILE");
		make_instruction((Operator)exec_when, "EXEC", "DO*WHEN");

		make_instruction((Operator)noop_open_paren, "EXEC", "NOOP_OPEN_PAREN");
		make_instruction((Operator)noop, "EXEC", "NOOP");

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

		set_parentheses("CODE", "QUOTE", 1);
	}
}