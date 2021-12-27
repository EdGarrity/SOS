#include <set>
#include <stack>
#include "Genome.h"
#include "Plush.ExecInstruction.h"
#include "Processor.h"
#include "Plush.StaticInit.h"
#include "..\Utilities\String.h"

namespace Plush
{
	// Add an instruction to the instruction set for this individual.
	inline size_t exec_enable_instruction(Environment& _env)
	{
		long n = _env.pop<long>();	// Instruction index

		_env.enable_function(n);

		return 1;
	}

	// Add instructions to the instruction set for this individual using the integers in the integer stack.
	inline size_t exec_enable_instructions(Environment& _env)
	{
		long number_of_instructions_to_add = _env.pop<long>();	// Number of instructions to add
		long i = _env.pop<long>();								// Instruction index

		if ((number_of_instructions_to_add > 0) && (_env.length<long>() >= (number_of_instructions_to_add - 1)))
		{
			_env.enable_function(i);
			long n = number_of_instructions_to_add - 1;

			while (n > 0)
			{
				if (_env.is_empty<long>() == false)
				{
					n--;
					i = _env.pop<long>();
					_env.enable_function(i);
				}
			}

			return number_of_instructions_to_add;
		}

		else
			return 1;
	}

	// Remove instruction from the instruction set for this individual.
	inline size_t exec_disable_instruction(Environment& _env)
	{
		long n = _env.pop<long>();	// Instruction index

		_env.disable_function(n);

		return 1;
	}

	// Expand inner block
	inline size_t noop_open_paren(Environment & _env)
	{
		//if (_env.has_elements<ExecAtom>(1))
		//{
			// Get top atom on EXEC stack
			ExecAtom atom = _env.get_top_atom<ExecAtom>();

			// Remove one closing paranthesis
			atom.close_parenthesis = (atom.close_parenthesis > 0) ? atom.close_parenthesis - 1 : atom.close_parenthesis;

			// Set top atom
			_env.set_top<ExecAtom>(atom);
		//}

		return 0;
	}

	inline size_t noop(Environment & _env)
	{
		return 0;
	}

	// An iteration instruction that executes the top item on the EXEC stack a number of times that 
	// depends on the top two integers, while also pushing the loop counter onto the INTEGER stack 
	// for possible access during the execution of the body of the loop
	inline size_t exec_do_range(Environment & _env)
	{
		long n = _env.pop<long>();	// destination index
		long i = _env.pop<long>();	// current index

		if (n == i)
			_env.push<long>(i);

		else
		{
			int direction = 1;

			if (i > n)
				direction = -1;

			// Get reference to top block on code stack
			Genome_section<ExecAtom> code_block = _env.get_stack<ExecAtom>()[0];

			if (_env.get_stack<ExecAtom>().free() > (code_block.size * 2))
			{
				_env.push<long>(i + direction);
				_env.push<long>(n);

				// Push code block onto exec stack
				_env.push<ExecAtom>(code_block);

				_env.push<ExecAtom>(ExecAtom("{:instruction EXEC.DO*RANGE :close 0}"));
			}
		}

		return 1;
	}

	inline size_t exec_if(Environment & _env)
	{
		// Get conditional boolean
		bool s = _env.pop<bool>();

		if (s)
			_env.get_stack<ExecAtom>().remove_item_at_position(1);
		else
			_env.get_stack<ExecAtom>().remove_item_at_position(0);

		return 1;
	}

	inline size_t exec_do_count(Environment & _env)
	{
		long n = _env.pop<long>();	// destination index

		if (n > 0)
		{
			_env.push<long>(0);
			_env.push<long>(n - 1);

			_env.push<ExecAtom>(ExecAtom("{:instruction EXEC.DO*RANGE :close 0}"));
		}

		return 1;
	}

	inline size_t exec_do_times(Environment & _env)
	{
		long n = _env.pop<long>();	// destination index

		if (n > 0)
		{
			_env.push<long>(0);
			_env.push<long>(n - 1);

			_env.push<ExecAtom>(ExecAtom("{:instruction INTEGER.POP :close 0}"));
			_env.push<ExecAtom>(ExecAtom("{:instruction EXEC.DO*RANGE :close 0}"));
		}

		return 1;
	}

	inline size_t exec_while(Environment & _env)
	{
		bool flag = _env.pop<bool>();
		size_t effort = 1;

		if (flag)
		{
			_env.push<ExecAtom>(ExecAtom("{:instruction EXEC.WHILE :close 0}"));
			effort = _env.get_stack<ExecAtom>().yankdup_item(1);
		}

		return effort;
	}

	inline size_t do_while(Environment & _env)
	{
		_env.push<ExecAtom>(ExecAtom("{:instruction EXEC.WHILE :close 0}"));
		return _env.get_stack<ExecAtom>().yankdup_item(1);
	}

	inline size_t exec_when(Environment & _env)
	{
		size_t effort = 1;
		bool flag = _env.pop<bool>();

		if (!flag)
			effort = _env.get_stack<ExecAtom>().remove_item_at_position(0);

		return effort;
	}

	inline size_t exec_k(Environment & _env)
	{
		return _env.get_stack<ExecAtom>().remove_item_at_position(1);
	}

	inline size_t exec_s(Environment & _env)
	{
		size_t effort = 0;

		// containing B
		effort += _env.get_stack<ExecAtom>().yankdup_item(1);

		// and C 
		effort += _env.get_stack<ExecAtom>().yankdup_item(3);

		// back onto the EXEC stack
		effort += _env.push<ExecAtom>(ExecAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"));

		// Followed by another instance of C 
		effort += _env.get_stack<ExecAtom>().yankdup_item(3);

		// Followed by another instance of A
		effort += _env.get_stack<ExecAtom>().yankdup_item(2);

		// Remove original A B C from stack
		effort += _env.get_stack<ExecAtom>().remove_item_at_position(3);
		effort += _env.get_stack<ExecAtom>().remove_item_at_position(3);
		effort += _env.get_stack<ExecAtom>().remove_item_at_position(3);

		return effort;
	}

	inline size_t exec_y(Environment & _env)
	{
		_env.push<ExecAtom>(ExecAtom("{:instruction EXEC.Y :close 0}"));
		return _env.get_stack<ExecAtom>().yankdup_item(1);
	}

	inline size_t code_append(Environment & _env)
	{
		Genome_section<CodeAtom> section_A = _env.get_stack<CodeAtom>()[0];
		Genome_section<CodeAtom> section_B = _env.get_stack<CodeAtom>()[1];

		if (_env.get_stack<CodeAtom>().get_atom_at_position(section_A.ending_position).close_parenthesis > 0)
			_env.get_stack<CodeAtom>().get_atom_at_position(section_A.ending_position).close_parenthesis--;

		if (_env.get_stack<CodeAtom>().get_atom_at_position(section_B.ending_position).close_parenthesis == 0)
			_env.get_stack<CodeAtom>().get_atom_at_position(section_B.ending_position).close_parenthesis = 1;

		return 1;
	}

	inline size_t code_atom(Environment & _env)
	{
		Genome_section<CodeAtom> item = _env.get_stack<CodeAtom>().pop_genome();

		if (item.size == 1)
			_env.push<bool>(true);

		else
			_env.push<bool>(false);

		return 1;
	}

	inline size_t code_car(Environment & _env)
	{
		Genome_section<CodeAtom> item = _env.peek_genome<CodeAtom>(0);

		if (item.size > 1)
		{
			Genome_section<CodeAtom> first_item = _env.get_stack<CodeAtom>().get_subitem(1);
			size_t old_top = _env.get_stack<CodeAtom>().size();

			_env.get_stack<CodeAtom>().push(first_item);
			_env.get_stack<CodeAtom>().get_atom_at_index(old_top).close_parenthesis++;
			_env.get_stack<CodeAtom>().remove_item_at_position(1);
		}

		return 1;
	}

	inline size_t code_cdr(Environment & _env)
	{
		Genome<CodeAtom>& stack = _env.get_stack<CodeAtom>();
		Genome_section<CodeAtom> block_a = stack[0];

		if (block_a.size > 1)
		{
			Genome_section<CodeAtom> first_item = stack.get_item_that_starts_at_position(0);
			stack.remove_items(0, first_item.size);
		}
		else
		{
			_env.pop<CodeAtom>();
			_env.push<CodeAtom>(CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 1}"));
		}

		return 1;
	}

	inline size_t code_cons(Environment & _env)
	{
		_env.get_stack<CodeAtom>().yank_item(1);

		unsigned n = _env.get_stack<CodeAtom>()[0].ending_position;
		_env.get_stack<CodeAtom>().get_atom_at_position(n).close_parenthesis--;

		return 1;
	}

	inline size_t code_container(Environment & _env)
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
				_env.get_stack<CodeAtom>().get_atom_at_position(container_block.ending_position).close_parenthesis++;
		}

		else
			_env.push<CodeAtom>(CodeAtom("{:instruction EXEC.NOOP :close 1}"));

		_env.get_stack<CodeAtom>().remove_stack_element(1);
		_env.get_stack<CodeAtom>().remove_stack_element(1);

		return 1;
	}

	inline size_t code_contains(Environment & _env)
	{
		// Get first block from stack
		Genome_section<CodeAtom> block_A = _env.peek_genome<CodeAtom>(0);

		// Get second block from stack
		Genome_section<CodeAtom> block_B = _env.peek_genome<CodeAtom>(1);

		Genome_section<CodeAtom> found = _env.get_stack<CodeAtom>().contains(block_A, block_B);
		_env.push<bool>(found.size > 0);

		_env.get_stack<CodeAtom>().remove_stack_element(0);
		_env.get_stack<CodeAtom>().remove_stack_element(0);

		return 1;
	}

	inline size_t code_discrepancy(Environment & _env)
	{
		Genome<CodeAtom>& genome = _env.get_stack<CodeAtom>();
		Genome_section<CodeAtom> block_a = genome[0];
		Genome_section<CodeAtom> block_b = genome[1];

		std::set<std::string> atom_set;

		if (block_a.size > 0)
		{
			for (size_t i = block_a.starting_position; i <= block_a.ending_position; i++)
				atom_set.insert(genome.get_atom_at_position(i).instruction_name);
		}

		if (block_b.size > 0)
		{
			for (size_t i = block_b.starting_position; i <= block_b.ending_position; i++)
				atom_set.insert(genome.get_atom_at_position(i).instruction_name);
		}

		int result = 0;

		for (std::string instruction : atom_set)
		{
			int count_a = 0;
			int count_b = 0;

			if (block_a.size > 0)
			{
				for (size_t i = block_a.starting_position; i <= block_a.ending_position; i++)
				{
					if (instruction == genome.get_atom_at_position(i).instruction_name)
						count_a++;
				}
			}

			if (block_b.size > 0)
			{
				for (size_t i = block_b.starting_position; i <= block_b.ending_position; i++)
				{
					if (instruction == genome.get_atom_at_position(i).instruction_name)
						count_b++;
				}
			}

			result += std::abs(count_a - count_b);
		}

		_env.push<long>(result);
				
		_env.pop_genome<CodeAtom>();
		_env.pop_genome<CodeAtom>();

		return 1;
	}

	inline size_t code_do(Environment & _env)
	{
		// Get reference to top block on code stack
		Genome_section<CodeAtom> code_block(_env.peek_genome<CodeAtom>(0));

		// Remove top code block after execution of block.
		_env.push<ExecAtom>(ExecAtom("{:instruction CODE.POP :close 1}"));

		// Push code block onto exec stack
		_env.push<ExecAtom>(code_block);

		return 1;
	}

	inline size_t code_do_star(Environment & _env)
	{
		// Get reference to top block on code stack
		Genome_section<CodeAtom> code_block(_env.peek_genome<CodeAtom>(0));

		// Push code block onto exec stack
		_env.push<ExecAtom>(code_block);

		// Remove top code block before execution of block.
		_env.get_stack<CodeAtom>().pop_genome();

		return 1;
	}

	inline size_t code_do_range(Environment & _env)
	{
		long n = _env.pop<long>();	// destination index
		long i = _env.pop<long>();	// current index

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
			Genome_section<CodeAtom> code_block = _env.peek_genome<CodeAtom>(0); //_env.get_stack<CodeAtom>()[0];

			if (_env.get_stack<ExecAtom>().free() > (code_block.size * 2))
			{
				_env.push<long>(i + direction);
				_env.push<long>(n);

				// Push code block onto exec stack
				_env.push<ExecAtom>(code_block);

				_env.push<ExecAtom>(ExecAtom("{:instruction CODE.DO*RANGE :close 1}"));
			}
		}

		return 1;
	}

	inline size_t code_do_count(Environment & _env)
	{
		long n = _env.pop<long>();	// destination index

		if (n > 0)
		{
			_env.push<long>(0);
			_env.push<long>(n - 1);

			_env.push<ExecAtom>(ExecAtom("{:instruction CODE.DO*RANGE :close 1}"));
		}

		return 1;
	}

	inline size_t code_do_times(Environment & _env)
	{
		long n = _env.pop<long>();	// destination index

		if (n > 0)
		{
			_env.push<long>(0);
			_env.push<long>(n - 1);

			_env.push<ExecAtom>(ExecAtom("{:instruction INTEGER.POP :close 1}"));
			_env.push<ExecAtom>(ExecAtom("{:instruction CODE.DO*RANGE :close 1}"));
		}

		return 1;
	}

	inline size_t code_extract(Environment & _env)
	{
		long item_number = std::abs(_env.pop<long>());	// index

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
				long n = item_number - 1;
				item_number = std::abs(n) % number_of_items + 1;

				Genome_section<CodeAtom> sub_block = genome.get_subitem(item_number);

				if (sub_block.size > 0)
				{
					unsigned int old_top = genome.size();

					_env.get_stack<CodeAtom>().yankdup_stack_element(sub_block);

					// Close extracted item if not last item in original top block
					if (item_number != number_of_items)
						genome.get_atom_at_index(old_top).close_parenthesis++;

					genome.remove_item_at_position(1);
				}
			}
		}

		return 1;
	}

	inline size_t bool2code(Environment & _env)
	{
		bool val = _env.pop<bool>();

		if (val)
			_env.push<CodeAtom>(CodeAtom("{:instruction TRUE :close 1}"));

		else
			_env.push<CodeAtom>(CodeAtom("{:instruction FALSE :close 1}"));

		return 1;
	}

	inline size_t float2code(Environment & _env)
	{
		double val = _env.pop<double>();
		std::string instruction = Utilities::string_format("{:instruction %f :close 1}", val);
		_env.push<CodeAtom>(CodeAtom(instruction));

		return 1;
	}

	inline size_t int2code(Environment & _env)
	{
		long val = _env.pop<long>();
		std::string instruction = Utilities::string_format("{:instruction %d :close 1}", val);
		_env.push<CodeAtom>(CodeAtom(instruction));

		return 1;
	}

	inline size_t code_if(Environment & _env)
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

		return 1;
	}

	inline size_t code_insert(Environment & _env)
	{
		long item_number = std::abs(_env.pop<long>());	// index

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
			long n = item_number - 1;
			item_number = std::abs(n) % number_of_items + 1;
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

		return 1;
	}

	inline size_t code_length(Environment & _env)
	{
		// Get reference to genome stack
		Genome<CodeAtom>& genome = _env.get_stack<CodeAtom>();

		// Get first block from stack
		Genome_section<CodeAtom> top_block = genome[0]; 

		// Get count items in first block
		unsigned int number_of_items = genome.number_of_items(top_block);
		_env.push<long>(number_of_items);
		_env.pop_genome<CodeAtom>();

		return 1;
	}

	inline size_t code_list(Environment & _env)
	{
		// Get reference to genome stack
		Genome<CodeAtom>& genome = _env.get_stack<CodeAtom>();

		// Get reference to first block on stack
		Genome_section<CodeAtom> block_a = genome[0];

		// Get reference to second block on stack
		Genome_section<CodeAtom> block_b = genome[1];

		// Close combined list
		genome.get_atom_at_position(block_b.ending_position).close_parenthesis++;

		// Balance parenthesis
		CodeAtom code("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}");
		genome.shove_it(code, block_b.starting_position);

		_env.push<CodeAtom>(CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"));

		return 1;
	}

	inline size_t code_member(Environment & _env)
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

		return 1;
	}

	inline size_t code_nth(Environment & _env)
	{
		long index = std::abs(_env.pop<long>());	// index

		// Get reference to genome stack
		Genome<CodeAtom>& genome = _env.get_stack<CodeAtom>();

		// Get reference to the top level block of the CODE stack
		Genome_section<CodeAtom> block(genome[0]);

		// Get count items in first block
		unsigned int number_of_items = genome.number_of_items(block);

		// Take modulo the number of blocks to ensure that it is within the meaningful range.
		index = index % number_of_items;

		// Get reference to Nth item in the top level block of the CODE stack
		Genome_section<CodeAtom> sub_block(genome.get_subitem(index + 1));

		// Blance closing parenthesis
		if ((number_of_items > 0) && (index < number_of_items - 1))
			genome.get_atom_at_position(sub_block.ending_position).close_parenthesis++;

		// Replace top genome with subsection
		genome.replace_section(sub_block.ending_position, block.ending_position, sub_block.size);
		
		return 1;
	}

	inline size_t code_nthcdr(Environment & _env)
	{
		long item_number = std::abs(_env.pop<long>());	// index

		// Get reference to genome stack
		Genome<CodeAtom>& genome = _env.get_stack<CodeAtom>();

		// Get reference to top genome
		Genome_section<CodeAtom> first_block = genome[0];

		// Get count items in first block
		unsigned int number_of_items = genome.number_of_items(first_block);

		if (number_of_items > 0)
		{
			// Take modulo the number of blocks to ensure that it is within the meaningful range.
			item_number = item_number % number_of_items;

			// Remove unwanted items
			if (item_number > 0)
			{
				Genome_section<CodeAtom> keep = genome.get_subitem(item_number);
				genome.remove_items(0, keep.ending_position + 1);
			}
		}

		return 1;
	}

	inline size_t code_null(Environment & _env)
	{
		Genome<CodeAtom>& genome = _env.get_stack<CodeAtom>();
		Genome_section<CodeAtom> top_block = genome[0];

		if (top_block.size == 0)
			_env.push<bool>(true);
			
		else if ((top_block.size == 1) 
			&& (genome.get_atom_at_position(top_block.starting_position).instruction_name == "CODE.NOOP")
				&& (genome.get_atom_at_position(top_block.starting_position).close_parenthesis > 0))
				_env.push<bool>(true);
		else
			_env.push<bool>(false);

		_env.pop_genome<CodeAtom>();

		return 1;
	}

	inline size_t code_position(Environment & _env)
	{
		size_t effort = 1;

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
			unsigned int number_of_items = genome.number_of_items(block_A);

			for (unsigned int n = 0; n < number_of_items; n++)
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

		return effort;
	}

	inline size_t code_quote(Environment & _env)
	{
		// Get reference to genome stack
		Genome<ExecAtom>& genome = _env.get_stack<ExecAtom>();

		// Get reference to top genome
		Genome_section<ExecAtom> top_block = genome[0]; 
		_env.push<CodeAtom>(top_block);
		_env.pop_genome<ExecAtom>();

		return 1;
	}

	inline size_t code_size(Environment & _env)
	{
		// Get first block from stack
		Genome_section<CodeAtom> top_block = _env.pop_genome<CodeAtom>();

		// Get count items in first block
		unsigned int size = top_block.size;
		_env.push<long>(size);

		return 1;
	}

	inline size_t code_subst(Environment & _env)
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
	inline size_t in2code(Environment & _env)
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
	inline size_t inall2code(Environment & _env)
	{
		if (_env.input.size() > 0)
		{
			for (unsigned int index = 0; index < _env.input.size(); index++)
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
	inline size_t inallrev2code(Environment & _env)
	{
		if (_env.input.size() > 0)
		{
			for (size_t index = _env.input.size() - 1; index >= 0; index--)
			{
				double value = _env.input[index];
				_env.push<CodeAtom>(CodeAtom(value));
			}
		}

		return 1;
	}

	void initExec()
	{
		//static bool initialized = false;

		//if (initialized) return;

		//initialized = true;

		push_make_instruction((Operator)exec_enable_instruction, "EXEC", "ENABLE*INSTRUCTION", integerType, nullType);
		push_make_instruction((Operator)exec_enable_instructions, "EXEC", "ENABLE*INSTRUCTIONS", integerType+ integerType, nullType);
		push_make_instruction((Operator)exec_disable_instruction, "EXEC", "DISABLE*INSTRUCTION", integerType, nullType);

		push_make_instruction((Operator)exec_k, "EXEC", "K", execType + execType, nullType);
		push_make_instruction((Operator)exec_s, "EXEC", "S", execType + execType + execType, execType);
		push_make_instruction((Operator)exec_y, "EXEC", "Y", execType, execType);
		push_make_instruction((Operator)exec_if, "EXEC", "IF", execType + execType + boolType, nullType);
		push_make_instruction((Operator)exec_do_range, "EXEC", "DO*RANGE", integerType + integerType + execType, nullType);
		push_make_instruction((Operator)exec_do_count, "EXEC", "DO*COUNT", integerType + execType, integerType + execType);
		push_make_instruction((Operator)exec_do_times, "EXEC", "DO*TIMES", integerType + execType, integerType + execType + execType);
		push_make_instruction((Operator)exec_while, "EXEC", "WHILE", integerType + execType, execType + execType);
		push_make_instruction((Operator)do_while, "EXEC", "DO*WHILE", execType, execType + execType);
		push_make_instruction((Operator)exec_when, "EXEC", "DO*WHEN", boolType + execType, nullType);

		push_make_instruction((Operator)noop_open_paren, "EXEC", "NOOP_OPEN_PAREN", execType, nullType);
		push_make_instruction((Operator)noop, "EXEC", "NOOP", nullType, nullType);

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

		push_make_instruction((Operator)code_append, "CODE", "APPEND", codeType + codeType, nullType);
		push_make_instruction((Operator)code_atom, "CODE", "ATOM", codeType, boolType);
		push_make_instruction((Operator)code_car, "CODE", "CAR", codeType, codeType);
		push_make_instruction((Operator)code_cdr, "CODE", "CDR", codeType, nullType);
		push_make_instruction((Operator)code_cons, "CODE", "CONS", codeType + codeType, nullType);
		push_make_instruction((Operator)code_container, "CODE", "CONTAINER", codeType + codeType, codeType);
		push_make_instruction((Operator)code_contains, "CODE", "CONTAINS", codeType + codeType, boolType);
		push_make_instruction((Operator)code_discrepancy, "CODE", "DISCREPANCY", codeType + codeType, integerType);
		push_make_instruction((Operator)code_do, "CODE", "DO", codeType, execType + execType);
		push_make_instruction((Operator)code_do_star, "CODE", "DO*", codeType, execType);
		push_make_instruction((Operator)code_do_range, "CODE", "DO*RANGE", codeType + integerType + integerType, codeType);
		push_make_instruction((Operator)code_do_count, "CODE", "DO*COUNT", codeType + integerType, codeType + integerType);
		push_make_instruction((Operator)code_do_times, "CODE", "DO*TIMES", codeType + integerType, execType + execType);
		push_make_instruction((Operator)code_extract, "CODE", "EXTRACT", codeType + integerType, nullType);
		push_make_instruction((Operator)bool2code, "CODE", "FROMBOOLEAN", boolType, codeType);
		push_make_instruction((Operator)float2code, "CODE", "FROMFLOAT", floatType, codeType);
		push_make_instruction((Operator)int2code, "CODE", "FROMINTEGER", integerType, codeType);
		push_make_instruction((Operator)code_if, "CODE", "IF", codeType + codeType + boolType, execType);
		push_make_instruction((Operator)code_insert, "CODE", "INSERT", codeType + codeType + integerType, codeType);
		push_make_instruction((Operator)code_length, "CODE", "LENGTH", codeType, integerType);
		push_make_instruction((Operator)code_list, "CODE", "LIST", codeType + codeType, codeType);
		push_make_instruction((Operator)code_member, "CODE", "MEMBER", codeType + codeType, boolType);
		push_make_instruction((Operator)code_nth, "CODE", "NTH", codeType + integerType, nullType);
		push_make_instruction((Operator)code_nthcdr, "CODE", "NTHCDR", codeType + integerType, nullType);
		push_make_instruction((Operator)code_null, "CODE", "NULL", codeType, boolType);
		push_make_instruction((Operator)code_position, "CODE", "POSITION", codeType + codeType, integerType);
		push_make_instruction((Operator)code_quote, "CODE", "QUOTE", execType, codeType);
		push_make_instruction((Operator)code_size, "CODE", "SIZE", codeType, integerType);
		push_make_instruction((Operator)code_subst, "CODE", "SUBST", codeType + codeType + codeType, nullType);

		set_parentheses("CODE", "QUOTE", 1);
	}
}