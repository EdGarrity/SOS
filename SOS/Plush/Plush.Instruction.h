#pragma once

#include <vector>
#include "Processor.h"
#include "Plush.StaticInit.h"

namespace Plush
{
	void initGenerics();

	/* generic functions */

	template <typename T>
	inline unsigned dup(Environment & _env)
	{
		// Check for valid parameters
		if (_env.has_elements<T>(1))
			_env.push<T>(_env.get_top<T>());

		return 1;
	}
	template <>
	inline unsigned dup<CodeAtom>(Environment & _env)
	{
		Genome<CodeAtom> stack;

		if (_env.has_elements<CodeAtom>(1))
		{
			_env.get_top<CodeAtom>(stack);
			_env.push<CodeAtom>(stack);
		}

		return 1;
	}
	template <>
	inline unsigned dup<ExecAtom>(Environment & _env)
	{
		Genome<ExecAtom> stack;

		if (_env.has_elements<ExecAtom>(1))
		{
			_env.get_top<ExecAtom>(stack);
			_env.push<ExecAtom>(stack);
		}

		return 1;
	}

	template <class T>
	inline unsigned pop_safe_index(Environment & _env)
	{
		int index = _env.pop<long>();
		int stacksize = _env.get_stack<T>().size();

		index = (index < 0) ? 0 : (index >= stacksize ? stacksize - 1 : index);

		return index;
	}

	template <class T>
	inline unsigned yank(Environment & _env)
	{
		unsigned int effort = 1;

		// Check for valid parameters
		if ((_env.has_elements<long>(1)) && (_env.has_elements<T>(1)))
		{
			int index = pop_safe_index<T>(_env);

			if (index > 0)
			{
				// Get item from deep in stack
				T v = _env.peek_index<T>(index);

				// Remove item from deep in stack
				unsigned int stack_size = _env.get_stack<T>().size();
				int delete_position = stack_size - index - 1;
				Genome<T>& stack = _env.get_stack<T>();

				if ((stack_size < domain::argmap::maximum_stack_size - 1) && (stack_size > 1))
				{
					unsigned int stack_pointer = delete_position;

					while (stack_pointer < stack_size - 1)
					{
						stack[stack_pointer] = stack[stack_pointer + 1];
						stack_pointer++;
						effort++;
					}
				}

				// Push removed item to top of stack
				stack[stack_size - 1] = v;
			}
		}

		return effort;
	}

	template<>
	inline unsigned yank<CodeAtom>(Environment & _env)
	{
		if ((_env.has_elements<long>(1)) && (_env.has_elements<CodeAtom>(1)))
		{
			int index = _env.pop<long>();	// index

			if (index > 0)
			{
				Genome<CodeAtom> extracted_block;
				Genome<CodeAtom> target_block;
				Genome<CodeAtom> modified_genome;

				// Get count of sub-blocks
				Utilities::FixedSizeStack<CodeAtom>& stack = _env.get_stack<CodeAtom>();
				Genome<CodeAtom>& genome = dynamic_cast<Genome<CodeAtom>&>(stack);

				int number_of_blocks = genome.number_of_blocks();

				// If the index is larger than the size of the specified stack, then the deepest element is `yank`ed up to the top.
				index = (index > (number_of_blocks - 1)) ? (number_of_blocks - 1) : index;
				int n = 0;

				while (_env.is_empty<CodeAtom>() == false)
				{
					_env.pop<CodeAtom>(extracted_block);

					if (n == index)
						target_block.push(extracted_block);

					else
					{
						Genome<CodeAtom> modified_genome_copy(modified_genome);
						modified_genome.clear();
						modified_genome.push(extracted_block);
						modified_genome.push(modified_genome_copy);
					}

					n++;
				}

				_env.push<CodeAtom>(modified_genome);
				_env.push<CodeAtom>(target_block);
			}											
		}

		return 1;
	}

	template<>
	inline unsigned yank<ExecAtom>(Environment & _env)
	{
		if ((_env.has_elements<long>(1)) && (_env.has_elements<ExecAtom>(1)))
		{
			int index = _env.pop<long>();	// index

			if (index > 0)
			{
				Genome<ExecAtom> extracted_block;
				Genome<ExecAtom> target_block;
				Genome<ExecAtom> modified_genome;

				// Get count of sub-blocks
				Genome<ExecAtom>& genome = _env.get_stack<ExecAtom>();
				int number_of_blocks = genome.number_of_blocks();

				// If the index is larger than the size of the specified stack, then the deepest element is `yank`ed up to the top.
				index = (index > (number_of_blocks - 1)) ? (number_of_blocks - 1) : index;
				int n = 0;

				while (_env.is_empty<ExecAtom>() == false)
				{
					_env.pop<ExecAtom>(extracted_block);

					if (n == index)
						target_block.push(extracted_block);

					else
					{
						Genome<ExecAtom> modified_genome_copy(modified_genome);
						modified_genome.clear();
						modified_genome.push(extracted_block);
						modified_genome.push(modified_genome_copy);
					}

					n++;
				}

				_env.push<ExecAtom>(modified_genome);
				_env.push<ExecAtom>(target_block);
			}
		}

		return 1;
	}

	template <class T>
	inline unsigned yankdup(Environment & _env)
	{
		// Check for valid parameters
		if ((_env.has_elements<long>(1)) && (_env.has_elements<T>(1)))
		{
			int index = pop_safe_index<T>(_env);

			T v = _env.peek_index<T>(index);
			_env.push<T>(v);
		}

		return 1;
	}

	template<>
	inline unsigned yankdup<CodeAtom>(Environment & _env)
	{
		if ((_env.has_elements<long>(1)) && (_env.has_elements<CodeAtom>(1)))
		{
			int index = _env.pop<long>();	// index
			Genome<CodeAtom> extracted_block;
			Genome<CodeAtom> temp_genome;

			// Get number of blocks on the stack
			int number_of_blocks = _env.length<CodeAtom>();

			// If the index is larger than the size of the specified stack, then the deepest element is `yank`ed up to the top.
			index = (index > (number_of_blocks - 1)) ? (number_of_blocks - 1) : index;
			index = (index < 0) ? 0 : index;

			while ((_env.is_empty<CodeAtom>() == false) && (index >= 0))
			{
				_env.pop<CodeAtom>(extracted_block);
				temp_genome.push(extracted_block);

				if ((index) == 0)
					extracted_block.bottom().close_parenthesis = 1;

				index--;
			}

			while (temp_genome.empty() == false)
			{
				Genome<CodeAtom> temp_block;
				temp_genome.pop_genome(temp_block);
				_env.push<CodeAtom>(temp_block);
			}

			_env.push<CodeAtom>(extracted_block);
		}

		return 1;
	}

	template<>
	inline unsigned yankdup<ExecAtom>(Environment & _env)
	{
		if ((_env.has_elements<long>(1)) && (_env.has_elements<ExecAtom>(1)))
		{
			int index = _env.pop<long>();	// index
			Genome<ExecAtom> extracted_block;
			Genome<ExecAtom> temp_genome;

			// Get number of blocks on the stack
			int number_of_blocks = _env.length<ExecAtom>();

			// If the index is larger than the size of the specified stack, then the deepest element is `yank`ed up to the top.
			index = (index > (number_of_blocks - 1)) ? (number_of_blocks - 1) : index;
			index = (index < 0) ? 0 : index;

			while ((_env.is_empty<ExecAtom>() == false) && (index >= 0))
			{
				_env.pop<ExecAtom>(extracted_block);
				temp_genome.push(extracted_block);

				if ((index) == 0)
					extracted_block.bottom().close_parenthesis = 1;

				index--;
			}

			while (temp_genome.empty() == false)
			{
				Genome<ExecAtom> temp_block;
				temp_genome.pop_genome(temp_block);
				_env.push<ExecAtom>(temp_block);
			}

			_env.push<ExecAtom>(extracted_block);
		}

		return 1;
	}

	// Need spcial cases for EXEC and CODE
	template <typename T> 
	inline unsigned equals(Environment & _env)
	{
		// Check for valid parameters
		if (_env.has_elements<T>(2))
			_env.push<bool>(_env.pop<T>() == _env.pop<T>());

		return 1;
	}

	template<>
	inline unsigned equals<CodeAtom>(Environment & _env)
	{
		// Check for valid parameters
		if (_env.has_elements<CodeAtom>(2))
		{
			Genome<CodeAtom> extracted_block_A;
			Genome<CodeAtom> extracted_block_B;

			_env.pop<CodeAtom>(extracted_block_A);
			_env.pop<CodeAtom>(extracted_block_B);

			if (extracted_block_A == extracted_block_B)
				_env.push<bool>(true);
			else
				_env.push<bool>(false);
		}

		return 1;
	}
		
	template<>
	inline unsigned equals<ExecAtom>(Environment & _env)
	{
		// Check for valid parameters
		if (_env.has_elements<ExecAtom>(2))
		{
			Genome<ExecAtom> block_A;
			Genome<ExecAtom> block_B;

			_env.pop<ExecAtom>(block_A);
			_env.pop<ExecAtom>(block_B);

			if (block_A == block_B)
				_env.push<bool>(true);
			else
				_env.push<bool>(false);
		}

		return 1;
	}


	template <class T>
	inline unsigned flush(Environment & _env)
	{
		_env.get_stack<T>().clear();
		return 1;
	}

	template <typename  T>
	inline unsigned protected_pop(Environment & _env)
	{
		if (_env.has_elements<T>(1))
			_env.pop<T>();

		return 1;
	}
	template <>
	inline unsigned protected_pop<CodeAtom>(Environment & _env)
	{
		Genome<CodeAtom> first_block;

		if (_env.has_elements<CodeAtom>(1))
			_env.pop<CodeAtom>(first_block);

		return 1;
	}
	template <>
	inline unsigned protected_pop<ExecAtom>(Environment & _env)
	{
		Genome<ExecAtom> first_block;

		if (_env.has_elements<ExecAtom>(1))
			_env.pop<ExecAtom>(first_block);

		return 1;
	}

	template <class T>
	unsigned rot(Environment & _env)
	{
		if (_env.has_elements<T>(3))
		{
			T x = _env.pop<T>();
			T y = _env.pop<T>();
			T z = _env.pop<T>();
			_env.push<T>(y);
			_env.push<T>(x);
			_env.push<T>(z);
		}

		return 1;
	}

	template<>
	inline unsigned rot<CodeAtom>(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(3))
		{
			Genome<CodeAtom> block_A;
			Genome<CodeAtom> block_B;
			Genome<CodeAtom> block_C;

			_env.pop<CodeAtom>(block_A);
			_env.pop<CodeAtom>(block_B);
			_env.pop<CodeAtom>(block_C);

			_env.push<CodeAtom>(block_B);
			_env.push<CodeAtom>(block_A);
			_env.push<CodeAtom>(block_C);
		}

		return 1;
	}

	template<>
	inline unsigned rot<ExecAtom>(Environment & _env)
	{
		if (_env.has_elements<ExecAtom>(3))
		{
			Genome<ExecAtom> block_A;
			Genome<ExecAtom> block_B;
			Genome<ExecAtom> block_C;

			_env.pop<ExecAtom>(block_A);
			_env.pop<ExecAtom>(block_B);
			_env.pop<ExecAtom>(block_C);

			_env.push<ExecAtom>(block_B);
			_env.push<ExecAtom>(block_A);
			_env.push<ExecAtom>(block_C);
		}

		return 1;
	}

	template <class T>
	inline unsigned shove(Environment & _env)
	{
		unsigned int effort = 1;

		if ((_env.has_elements<long>(1)) && (_env.has_elements<T>(1)))
		{
			int index = pop_safe_index<T>(_env);
			int stack_size = _env.get_stack<T>().size();

			int insert_position = stack_size - index - 1;

			Genome<T>& stack = _env.get_stack<T>();

			if ((stack_size < domain::argmap::maximum_stack_size - 1) && (stack_size > 1))
			{
				T v = _env.get_top<T>();

				int stack_pointer = stack_size - 1;

				while (stack_pointer >= insert_position)
				{
					stack[stack_pointer + 1] = stack[stack_pointer];
					stack_pointer--;
				}

				stack[stack_pointer + 1] = v;
				_env.push(stack[stack_size]);
			}

			effort = stack.size() - insert_position + 1;
		}

		return effort;
	}

	template<>
	inline unsigned shove<CodeAtom>(Environment & _env)
	{
		if ((_env.has_elements<long>(1)) && (_env.has_elements<CodeAtom>(1)))
		{
			int index = _env.pop<long>();	// index

			Genome<CodeAtom> first_block;
			Genome<CodeAtom> top_block;
			Genome<CodeAtom> bottom_block;

			if (index > 0)
			{
				// Get first block from stack
				_env.pop<CodeAtom>(first_block);

				if (first_block.size() == 0)
					_env.push<CodeAtom>(first_block);

				else
				{
					Genome<CodeAtom>& genome = _env.get_stack<CodeAtom>();

					genome.split(top_block, bottom_block, index, Genome<CodeAtom>::SPLIT_MODE::block);

					_env.clear<CodeAtom>();
					_env.push<CodeAtom>(bottom_block);
					_env.push<CodeAtom>(first_block);
					_env.push<CodeAtom>(top_block);
				}
			}
		}

		return 1;
	}

	template<>
	inline unsigned shove<ExecAtom>(Environment & _env)
	{
		if ((_env.has_elements<long>(1)) && (_env.has_elements<ExecAtom>(1)))
		{
			int extra_blocks = 0;
			int simulated_closing_parenthesis = 0;
			int index = _env.pop<long>();	// index

			Genome<ExecAtom> first_block;
			Genome<ExecAtom> top_half;
			Genome<ExecAtom> bottom_half;
			Genome<ExecAtom> top_block;
			Genome<ExecAtom> bottom_block;
			Genome<ExecAtom> genome;
			Genome<ExecAtom> temp_block;

			if (index > 0)
			{
				// Get first block from stack
				_env.pop<ExecAtom>(first_block);

				if (first_block.size() == 0)
					_env.push<ExecAtom>(first_block);

				else
				{
					int n = 0;
					while (_env.is_empty<ExecAtom>() == false)
					{
						if (simulated_closing_parenthesis == 0)
							extra_blocks = _env.pop<ExecAtom>(genome);

						if (simulated_closing_parenthesis > 0)
						{
							genome.clear();
							ExecAtom atom = Plush::ExecAtom("{:instruction EXEC.NOOP :close 1}");
							genome.push(atom);

							simulated_closing_parenthesis--;
							extra_blocks = 0;
						}

						if (extra_blocks > 0)
						{
							if ((n + 1) == index)
							{
								simulated_closing_parenthesis = extra_blocks;
								genome.bottom().close_parenthesis = 1;;
							}

							else
								n += extra_blocks;
						}

						if (n < index)
							top_half.push(genome);
						else
							bottom_half.push(genome);

						n++;
					}

					while (top_half.empty() == false)
					{
						top_half.pop_genome(temp_block);
						top_block.push(temp_block);
					}

					while (bottom_half.empty() == false)
					{
						bottom_half.pop_genome(temp_block);
						bottom_block.push(temp_block);
					}

					_env.push<ExecAtom>(bottom_block);
					_env.push<ExecAtom>(first_block);
					_env.push<ExecAtom>(top_block);
				}
			}
		}

		return 1;
	}

	template <class T>
	inline unsigned stackdepth(Environment & _env)
	{
		_env.push<long>(_env.get_stack<T>().size());
		return 1;
	}

	template <typename T>
	inline unsigned swap(Environment & _env)
	{
		if (_env.has_elements<T>(2))
		{
			Genome<T>& stack = _env.get_stack<T>();

			unsigned int stack_size = _env.get_stack<T>().size();

			T tmp = stack[stack_size - 1];
			stack[stack_size - 1] = stack[stack_size - 2];
			stack[stack_size - 2] = tmp;
		}

		return 1;
	}

	template<>
	inline unsigned swap<CodeAtom>(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(2))
		{
			Genome<CodeAtom> extracted_block_A;
			Genome<CodeAtom> extracted_block_B;

			// Get first block from stack
			_env.pop<CodeAtom>(extracted_block_A);

			// Get second block from stack
			_env.pop<CodeAtom>(extracted_block_B);

			_env.push<CodeAtom>(extracted_block_A);
			_env.push<CodeAtom>(extracted_block_B);
		}

		return 1;
	}

	template<>
	inline unsigned swap<ExecAtom>(Environment & _env)
	{
		if (_env.has_elements<ExecAtom>(2))
		{
			Genome<ExecAtom> extracted_block_A;
			Genome<ExecAtom> extracted_block_B;

			// Get first block from stack
			_env.pop<ExecAtom>(extracted_block_A);

			//Get second block from stack
			_env.pop<ExecAtom>(extracted_block_B);

			_env.push<ExecAtom>(extracted_block_A);
			_env.push<ExecAtom>(extracted_block_B);
		}

		return 1;
	}
}