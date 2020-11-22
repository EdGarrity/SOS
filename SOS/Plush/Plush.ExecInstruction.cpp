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
		unsigned int extra_blocks = 0;

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
				int s = _env.get_stack<ExecAtom>().number_of_atoms(extra_blocks);

				if (_env.get_stack<ExecAtom>().free() > (s * 2))
				{
					_env.push<long>(i + direction);
					_env.push<long>(n);

					_env.get_stack<ExecAtom>().shove_it(0, 0, s);

					_env.push<ExecAtom>(ExecAtom("{:instruction EXEC.DO*RANGE :close 0}"));
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
				_env.get_stack<ExecAtom>().remove_item_at_position(1);
			else
				_env.get_stack<ExecAtom>().remove_item_at_position(0);
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

				_env.push<ExecAtom>(ExecAtom("{:instruction EXEC.DO*RANGE :close 0}"));
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

				_env.push<ExecAtom>(ExecAtom("{:instruction INTEGER.POP :close 0}"));
				_env.push<ExecAtom>(ExecAtom("{:instruction EXEC.DO*RANGE :close 0}"));
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
				_env.push<ExecAtom>(ExecAtom("{:instruction EXEC.WHILE :close 0}"));
				_env.get_stack<ExecAtom>().yankdup_item(1);
			}
		}

		return 1;
	}

	unsigned do_while(Environment & _env)
	{
		if (_env.has_elements<ExecAtom>(1))
		{
			_env.push<ExecAtom>(ExecAtom("{:instruction EXEC.WHILE :close 0}"));
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
				_env.get_stack<ExecAtom>().remove_item_at_position(0);
		}

		return 1;
	}

	unsigned exec_k(Environment & _env)
	{
		if (_env.has_elements<ExecAtom>(2))
			_env.get_stack<ExecAtom>().remove_item_at_position(1);

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

			// Push a list
//			_env.push<ExecAtom>(Atom("{:instruction EXEC.NOOP :close 1}"));

			// A = 0
			// B = 1
			// C = 2

			// containing B
			_env.get_stack<ExecAtom>().yankdup_item(1);

			// A = 1
			// B = 2
			// C = 3

			// and C 
			_env.get_stack<ExecAtom>().yankdup_item(3);

			// A = 3
			// B = 4
			// C = 5

			// back onto the EXEC stack
			_env.push<ExecAtom>(ExecAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"));

			// A = 1
			// B = 2
			// C = 3


			//_env.get_stack<ExecAtom>().remove_item(1);
			//_env.get_stack<ExecAtom>().remove_item(1);
			//_env.get_stack<ExecAtom>().remove_item(1);






			// Followed by another instance of C 
			_env.get_stack<ExecAtom>().yankdup_item(3);

			// A = 2
			// B = 3
			// C = 4

			// Followed by another instance of A
			_env.get_stack<ExecAtom>().yankdup_item(2);

			// Remove original A B C from stack
			_env.get_stack<ExecAtom>().remove_item_at_position(3);
			_env.get_stack<ExecAtom>().remove_item_at_position(3);
			_env.get_stack<ExecAtom>().remove_item_at_position(3);
		}

		return 1;
	}

	unsigned exec_y(Environment & _env)
	{
		if (_env.has_elements<ExecAtom>(1))
		{
			//Genome<ExecAtom> extracted_block;

			//_env.pop<ExecAtom>(extracted_block);

			//_env.push<ExecAtom>(extracted_block);
			//_env.push<ExecAtom>(Atom("{:instruction EXEC.Y :close 0}"));
			//_env.push<ExecAtom>(extracted_block);

			_env.push<ExecAtom>(ExecAtom("{:instruction EXEC.Y :close 0}"));

			_env.get_stack<ExecAtom>().yankdup_item(1);
		}

		return 1;
	}

	unsigned code_append(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(2))
		{
			Genome_section<CodeAtom> section_A = _env.get_stack<CodeAtom>()[0];
			Genome_section<CodeAtom> section_B = _env.get_stack<CodeAtom>()[1];

			if (_env.get_stack<CodeAtom>().get_atom(section_A.ending_position).close_parenthesis > 0)
				_env.get_stack<CodeAtom>().get_atom(section_A.ending_position).close_parenthesis--;

			if (_env.get_stack<CodeAtom>().get_atom(section_B.ending_position).close_parenthesis == 0)
				_env.get_stack<CodeAtom>().get_atom(section_B.ending_position).close_parenthesis = 1;
		}

		return 1;
	}

	unsigned code_atom(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(1))
		{
			Genome_section<CodeAtom> item = _env.get_stack<CodeAtom>().pop_genome();

			if (item.size == 1)
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
			Genome_section<CodeAtom> item = _env.peek_genome<CodeAtom>(0);

			if (item.size > 1)
			{
				Genome_section<CodeAtom> first_item = _env.get_stack<CodeAtom>().get_subitem(1);
				int old_top = _env.get_stack<CodeAtom>().size();

				_env.get_stack<CodeAtom>().push(first_item);
				_env.get_stack<CodeAtom>().container()[old_top].close_parenthesis++;
				_env.get_stack<CodeAtom>().remove_item_at_position(1);
			}
		}

		return 1;
	}

	unsigned code_cdr(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(1))
		{
			unsigned int extra_blocks;
			int n = _env.get_stack<CodeAtom>().number_of_atoms(extra_blocks, 0);

			if (n > 1)
			{
				int ns = _env.get_stack<CodeAtom>().number_of_atoms_in_Nth_block(extra_blocks);
				_env.get_stack<CodeAtom>().remove_items(0, ns);
			}
			else
			{
				_env.pop<CodeAtom>();
				_env.push<CodeAtom>(CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 1}"));
			}
		}

		return 1;
	}

	unsigned code_cons(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(2))
		{
			_env.get_stack<CodeAtom>().yank_item(1);

			unsigned n = _env.get_stack<CodeAtom>()[0].ending_position;
			_env.get_stack<CodeAtom>().get_atom(n).close_parenthesis--;
		}

		return 1;
	}

	unsigned code_container(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(2))
		{
			Genome_section<CodeAtom> container_block;

			// Get first block from stack
			Genome_section<CodeAtom> block_A = _env.peek_genome<CodeAtom>(0);

			// Get second block from stack
			Genome_section<CodeAtom> block_B = _env.peek_genome<CodeAtom>(1);

			container_block = _env.get_stack<CodeAtom>().container_of(block_A, block_B);

			if (container_block.size > 0)
			{
				_env.get_stack<CodeAtom>().yankdup_stack_element(container_block);

				if (container_block.starting_position == 0)
					_env.get_stack<CodeAtom>().get_atom(container_block.ending_position).close_parenthesis++;
			}

			else
				_env.push<CodeAtom>(CodeAtom("{:instruction EXEC.NOOP :close 1}"));

			_env.get_stack<CodeAtom>().remove_stack_element(1);
			_env.get_stack<CodeAtom>().remove_stack_element(1);
		}

		return 1;
	}

	unsigned code_contains(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(2))
		{
			// Get first block from stack
			Genome_section<CodeAtom> block_A = _env.peek_genome<CodeAtom>(0);

			// Get second block from stack
			Genome_section<CodeAtom> block_B = _env.peek_genome<CodeAtom>(1);

			Genome_section<CodeAtom> found = _env.get_stack<CodeAtom>().contains(block_A, block_B);
			_env.push<bool>(found.size > 0);

			_env.get_stack<CodeAtom>().remove_stack_element(0);
			_env.get_stack<CodeAtom>().remove_stack_element(0);
		}

		return 1;
	}

	unsigned code_discrepancy(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(2))
		{
			Genome<CodeAtom>& genome = _env.get_stack<CodeAtom>();
			Genome_section<CodeAtom> block_a = genome[0];
			Genome_section<CodeAtom> block_b = genome[1];

			std::set<std::string> atom_set;

			if (block_a.size > 0)
			{
				for (int i = block_a.starting_position; i <= block_a.ending_position; i++)
					atom_set.insert(genome.get_atom(i).instruction);
			}

			if (block_b.size > 0)
			{
				for (int i = block_b.starting_position; i <= block_b.ending_position; i++)
					atom_set.insert(genome.get_atom(i).instruction);
			}

			int result = 0;

			for (std::string instruction : atom_set)
			{
				int count_a = 0;
				int count_b = 0;

				if (block_a.size > 0)
				{
					for (int i = block_a.starting_position; i <= block_a.ending_position; i++)
					{
						if (instruction == genome.get_atom(i).instruction)
							count_a++;
					}
				}

				if (block_b.size > 0)
				{
					for (int i = block_b.starting_position; i <= block_b.ending_position; i++)
					{
						if (instruction == genome.get_atom(i).instruction)
							count_b++;
					}
				}

				result += std::abs(count_a - count_b);
			}

			_env.push<long>(result);
				
			_env.pop_genome<CodeAtom>();
			_env.pop_genome<CodeAtom>();
		}

		return 1;
	}

	unsigned code_do(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(1))
		{
			// Get reference to top block on code stack
			Genome_section<CodeAtom> code_block(_env.peek_genome<CodeAtom>(0));

			// Remove top code block after execution of block.
			_env.push<ExecAtom>(ExecAtom("{:instruction CODE.POP :close 1}"));

			// Push code block onto exec stack
			_env.push<ExecAtom>(code_block);

			return 1;
		}
	}

	unsigned code_do_star(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(1))
		{
			// Get reference to top block on code stack
			Genome_section<CodeAtom> code_block(_env.peek_genome<CodeAtom>(0));

			// Push code block onto exec stack
			_env.push<ExecAtom>(code_block);

			// Remove top code block before execution of block.
			_env.get_stack<CodeAtom>().pop_genome();

			return 1;
		}
	}

	unsigned code_do_range(Environment & _env)
	{
		if ((_env.has_elements<long>(2)) && (_env.has_elements<CodeAtom>(1)))
		{
			int n = _env.pop<long>();	// destination index
			int i = _env.pop<long>();	// current index

			if (n == i)
			{
				_env.push<long>(i);

				// Get reference to top block on code stack
				Genome_section<CodeAtom> code_block(_env.peek_genome<CodeAtom>(0));

				// Push code block onto exec stack
				_env.push<ExecAtom>(code_block);

				// Remove top code block before execution of block.
				_env.get_stack<CodeAtom>().pop_genome();
			}

			else
			{
				int direction = 1;

				if (i > n)
					direction = -1;

				// Get reference to top block on code stack
				Genome_section<CodeAtom> code_block = _env.get_stack<CodeAtom>()[0];

				if (_env.get_stack<ExecAtom>().free() > (code_block.size * 2))
				{
					_env.push<long>(i + direction);
					_env.push<long>(n);

					// Push code block onto exec stack
					_env.push<ExecAtom>(code_block);

					_env.push<ExecAtom>(ExecAtom("{:instruction CODE.DO*RANGE :close 1}"));
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

				_env.push<ExecAtom>(ExecAtom("{:instruction CODE.DO*RANGE :close 1}"));
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

				_env.push<ExecAtom>(ExecAtom("{:instruction INTEGER.POP :close 1}"));
				_env.push<ExecAtom>(ExecAtom("{:instruction CODE.DO*RANGE :close 1}"));
			}
		}

		return 1;
	}

	unsigned code_extract(Environment & _env)
	{
		if ((_env.has_elements<long>(1)) && (_env.has_elements<CodeAtom>(1)))
		{
			int item_number = std::abs(_env.pop<long>());	// index

			if (item_number != 0)
			{
				// Get reference to genome stack
				Genome<CodeAtom>& genome = _env.get_stack<CodeAtom>();

				// Get top block
				Genome_section<CodeAtom> first_block = genome[0];

				// Get count items in first block
				int number_of_items = genome.number_of_items(first_block);

				if (number_of_items > 0)
				{
					// Take modulo the number of blocks to ensure that it is within the meaningful range.
					item_number = std::abs(item_number - 1) % number_of_items + 1;

					Genome_section<CodeAtom> sub_block = genome.get_subitem(item_number);

					if (sub_block.size > 0)
					{
						int old_top = genome.size();

						_env.get_stack<CodeAtom>().yankdup_stack_element(sub_block);

						// Close extracted item if not last item in original top block
						if (item_number != number_of_items)
							genome.get_stack_element_ref(old_top).close_parenthesis++;

						genome.remove_item_at_position(1);
					}
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
			// Get blocks
			Genome_section<CodeAtom> code_block_a = _env.peek_genome<CodeAtom>(0);
			Genome_section<CodeAtom> code_block_b = _env.peek_genome<CodeAtom>(1);

			// Get conditional boolean
			bool s = _env.pop<bool>();

			if (s)
				_env.push<ExecAtom>(code_block_a);

			else
				_env.push<ExecAtom>(code_block_b);

			_env.get_stack<CodeAtom>().pop_genome();
			_env.get_stack<CodeAtom>().pop_genome();
		}

		return 1;
	}

	unsigned code_insert(Environment & _env)
	{
		if ((_env.has_elements<long>(1)) && (_env.has_elements<CodeAtom>(2)))
		{
			int item_number = std::abs(_env.pop<long>());	// index

			// Get reference to genome stack
			Genome<CodeAtom>& genome = _env.get_stack<CodeAtom>();

			// Get reference to second block on stack
			Genome_section<CodeAtom> block_b = genome[1];

			// Get reference to first block on stack
			Genome_section<CodeAtom> block_a = genome[0];

			// Get count items in first block
			int number_of_items = genome.number_of_items(block_a);

			// Take modulo the number of blocks to ensure that it is within the meaningful range.
			if (item_number != 0)
			{
				item_number = std::abs(item_number - 1) % number_of_items + 1;
				item_number = (item_number < 0) ? 0 : item_number;
				item_number = (item_number == number_of_items) ? 0 : item_number;
			}

			if (item_number == 0)
			{
				// Insert secoond item into first item at position 0.
				genome.yankdup_stack_element(block_b);

				// Put the second block in a list object
				genome.push(CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"));

				// Remove original second item
				genome.remove_stack_element(1);
			}
			else
			{
				// Move second block to position in first block
				genome.shove(item_number, block_b);
			}
		}

		return 1;
	}

	inline unsigned code_length(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(1))
		{
			// Get reference to genome stack
			Genome<CodeAtom>& genome = _env.get_stack<CodeAtom>();

			// Get first block from stack
			Genome_section<CodeAtom> top_block = genome[0]; 

			// Get count items in first block
			int number_of_items = genome.number_of_items(top_block);
			_env.push<long>(number_of_items);
			_env.pop_genome<CodeAtom>();
		}

		return 1;
	}

	unsigned code_list(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(2))
		{
			// Get reference to genome stack
			Genome<CodeAtom>& genome = _env.get_stack<CodeAtom>();

			// Get reference to first block on stack
			Genome_section<CodeAtom> block_a = genome[0];

			// Get reference to second block on stack
			Genome_section<CodeAtom> block_b = genome[1];

			// Close combined list
			genome.get_atom(block_b.ending_position).close_parenthesis++;

			// Balance parenthesis
			CodeAtom code("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}");
			genome.shove_it(code, block_b.starting_position);

			_env.push<CodeAtom>(CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"));
		}

		return 1;
	}

	unsigned code_member(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(2))
		{
			// Get reference to genome stack
			Genome<CodeAtom>& genome = _env.get_stack<CodeAtom>();

			// Get reference to first block on stack
			Genome_section<CodeAtom> block_a = genome[0];

			// Get reference to second block on stack
			Genome_section<CodeAtom> block_b = genome[1];

			Genome_section<CodeAtom> found = _env.get_stack<CodeAtom>().contains(block_b, block_a);
			_env.push<bool>(found.size > 0);

			_env.pop_genome<CodeAtom>();
			_env.pop_genome<CodeAtom>();
		}

		return 1;
	}

	unsigned code_nth(Environment & _env)
	{
		if ((_env.has_elements<long>(1)) && (_env.has_elements<CodeAtom>(1)))
		{
			int index = std::abs(_env.pop<long>());	// index

			// Get reference to genome stack
			Genome<CodeAtom>& genome = _env.get_stack<CodeAtom>();

			// Get reference to the top level block of the CODE stack
			Genome_section<CodeAtom> block(genome[0]);

			// Get count items in first block
			int number_of_items = genome.number_of_items(block);

			// Take modulo the number of blocks to ensure that it is within the meaningful range.
			index = std::abs(index) % number_of_items;

			// Get reference to Nth item in the top level block of the CODE stack
			Genome_section<CodeAtom> sub_block(genome.get_subitem(index + 1));

			// Blance closing parenthesis
			if (index < number_of_items - 1)
				genome.get_atom(sub_block.ending_position).close_parenthesis++;

			// Replace top genome with subsection
			genome.replace_section(sub_block.ending_position, block.ending_position, sub_block.size);
		}
		
		return 1;
	}

	unsigned code_nthcdr(Environment & _env)
	{
		if ((_env.has_elements<long>(1)) && (_env.has_elements<CodeAtom>(1)))
		{
			int item_number = std::abs(_env.pop<long>());	// index

			// Get reference to genome stack
			Genome<CodeAtom>& genome = _env.get_stack<CodeAtom>();

			// Get reference to top genome
			Genome_section<CodeAtom> first_block = genome[0];

			// Get count items in first block
			int number_of_items = genome.number_of_items(first_block);

			if (number_of_items > 0)
			{
				// Take modulo the number of blocks to ensure that it is within the meaningful range.
				item_number = std::abs(item_number) % number_of_items;

				// Remove unwanted items
				if (item_number > 0)
				{
					Genome_section<CodeAtom> keep = genome.get_subitem(item_number);
					genome.remove_items(0, keep.ending_position + 1);
				}
			}
		}

		return 1;
	}

	inline unsigned code_null(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(1))
		{
			Genome<CodeAtom>& genome = _env.get_stack<CodeAtom>();
			Genome_section<CodeAtom> top_block = genome[0];

			if (top_block.size == 0)
				_env.push<bool>(true);
			
			else if ((top_block.size == 1) 
				&& (genome.get_atom(top_block.starting_position).instruction == "CODE.NOOP")
					&& (genome.get_atom(top_block.starting_position).close_parenthesis > 0))
					_env.push<bool>(true);
			else
				_env.push<bool>(false);

			_env.pop_genome<CodeAtom>();
		}

		return 1;
	}

	unsigned code_position(Environment & _env)
	{
		int effort = 1;

		if (_env.has_elements<CodeAtom>(2))
		{
			long return_val = -1;

			// Get first block from stack
			Genome_section<CodeAtom> block_A = _env.peek_genome<CodeAtom>(0);

			// Get second block from stack
			Genome_section<CodeAtom> block_B = _env.peek_genome<CodeAtom>(1);

			if (block_A.size == 0)
			{
				if (block_B.size == 0)
					return_val = 0;
			}

			else
			{
				// Get reference to genome stack
				Genome<CodeAtom>& genome = _env.get_stack<CodeAtom>();

				// Get count items in first block
				int number_of_items = genome.number_of_items(block_A);

				for (int n = 0; n < number_of_items; n++)
				{
					Genome_section<CodeAtom> subitem = genome.get_subitem(n + 1);
					
					if (genome.comp(subitem, block_B))
					{
						return_val = n;
						break;
					}
				}
			}

			_env.pop_genome<CodeAtom>();
			_env.pop_genome<CodeAtom>();

			effort = block_A.size + block_B.size;
			_env.push(return_val);
		}

		return effort;
	}

	unsigned code_quote(Environment & _env)
	{
		if (_env.has_elements<ExecAtom>(1))
		{
			// Get reference to genome stack
			Genome<ExecAtom>& genome = _env.get_stack<ExecAtom>();

			// Get reference to top genome
			Genome_section<ExecAtom> top_block = genome[0]; 
			_env.push<CodeAtom>(top_block);
			_env.pop_genome<ExecAtom>();
		}

		return 1;
	}

	inline unsigned code_size(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(1))
		{
			// Get first block from stack
			Genome_section<CodeAtom> top_block = _env.pop_genome<CodeAtom>();

			// Get count items in first block
			int size = top_block.size;
			_env.push<long>(size);
		}

		return 1;
	}

	inline unsigned code_subst(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(3))
		{
			Genome<CodeAtom>& stack = _env.get_stack<CodeAtom>();

			// Get first block from stack
			Genome_section<CodeAtom> block_A = stack[0];

			// Get second block from stack
			Genome_section<CodeAtom> block_B = stack[1];

			// Get third block from stack
			Genome_section<CodeAtom> block_C = stack[2];

			stack.subst(block_A, block_C, block_B);

			stack.remove_item_at_position(1);
			stack.remove_item_at_position(1);
			stack.remove_item_at_position(1);
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