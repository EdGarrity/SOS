#pragma once

#include <vector>
#include "Processor.h"
#include "Plush.StaticInit.h"

namespace Plush
{
	void initGenerics();

	/* generic functions */

	// Need spcial cases for EXEC and CODE
	template <typename T>
	inline unsigned dup(Environment & _env)
	{
		// Check for valid parameters
		if (_env.has_elements<T>(1))
			_env.push<T>(_env.top<T>());

		return 1;
	}

	template <class T>
	inline unsigned safe_index(Environment & _env)
	{
		int index = _env.pop<long>();
		int stacksize = _env.get_stack<T>().size();
		index = (index < 0) ? 0 : (index >= stacksize ? stacksize - 1 : index);
		return stacksize - 1 - index;
	}

	// Need spcial cases for EXEC and CODE
	template <class T>
	inline unsigned yankdup(Environment & _env)
	{
		// Check for valid parameters
		if ((_env.has_elements<long>(1)) && (_env.has_elements<T>(1)))
		{
			int index = safe_index<T>(_env);

			T v = _env.peek_index<T>(index);
			_env.push<T>(v);
		}

		return 1;
	}
	template<>
	inline unsigned yankdup<CodeAtom>(Environment & _env)
	{
		// Check for valid parameters
		if ((_env.has_elements<long>(1)) && (_env.has_elements<CodeAtom>(1)))
		{
			int n = _env.pop<long>();

			std::pair<size_t, size_t> block_range = get_block_index<CodeAtom>(_env, n, 0);

			if (block_range.first == block_range.second)
			{
				CodeAtom v = _env.peek_index<CodeAtom>(block_range.first);
				_env.push<CodeAtom>(v);
			}

			else
			{
				_env.push<CodeAtom>(CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"));

				for (int n = block_range.first; n < block_range.second + 1; n++)
				{
					CodeAtom v = _env.peek_index<CodeAtom>(n);
					_env.push<CodeAtom>(v);
				}
			}
		}

		return 1;
	}

	template<>
	inline unsigned yankdup<ExecAtom>(Environment & _env)
	{
		// Check for valid parameters
		if ((_env.has_elements<long>(1)) && (_env.has_elements<ExecAtom>(1)))
		{
			enum block_states
			{
				close = 0,
				close_and_open
			};

			Utilities::FixedSizeStack<block_states> block_stack;

			int blocks_needed = 0;
			int added_opens = 0;
			int prev_added_opens = 0;
			int hidden_blocks = 0;
			int block_level = 0;
			int prev_block_level = 0;
			bool top_level_block_start = false;
			int blocks_closed = 0;
			bool previously_poped_close_open = false;
			bool top_level_block_end = false;
			int start_index = 0;
			int end_index = 0;
			bool found = false;

			// Zero based index of stack block to yank
			unsigned int index_of_block_we_are_looking_for = _env.pop<long>();


			std::array<ExecAtom, domain::argmap::maximum_stack_size>& exec_stack = _env.get_stack<ExecAtom>().container();

			unsigned int exec_stack_index = _env.get_stack<ExecAtom>().size();

			while (exec_stack_index > 0)
			{
				bool poped_close_open = false;

				prev_block_level = block_level;

				exec_stack_index--;

				ExecAtom atom = exec_stack[exec_stack_index];

				blocks_needed = Func2BlockWantsMap[atom.instruction];
				added_opens = blocks_needed + atom.close_parentheses;

				for (int n = 0; n < added_opens; n++)
				{
					if (n == 0)
					{
						block_stack.push(block_states::close);
						block_level++;
					}

					else
						block_stack.push(block_states::close_and_open);
				}

				bool popped = false;

				for (int n = 0; n < atom.close_parentheses; n++)
				{
					if (block_stack.empty())
						break;

					if (block_stack.top() == block_states::close)
						block_level--;

					if (block_stack.top() == block_states::close_and_open)
						poped_close_open = true;

					block_stack.pop();
					popped = true;
				}

				hidden_blocks = ((blocks_needed > 0) && (blocks_needed > added_opens)) ? (blocks_needed - added_opens) : 0;


				if ((prev_block_level == 0) && (block_level == 1))
					top_level_block_start = true;

				else if ((block_level == 1) && (previously_poped_close_open))
					top_level_block_start = true;

				else if ((prev_added_opens > 0) && (block_level == 1))
					top_level_block_start = true;

				else
					top_level_block_start = false;


				if ((popped) && (block_level == 0))
					top_level_block_end = true;

				else
					top_level_block_end = false;


				// Check if this is the block we are looking for
				if (index_of_block_we_are_looking_for == 0)
				{
					// found start of block
					start_index = exec_stack_index;

					if ((block_level == 0) || (top_level_block_end))
					{
						// found end of block
						end_index = exec_stack_index;
						found = true;
						break;
					}
				}

				if ((hidden_blocks > 0) || (top_level_block_start))
					index_of_block_we_are_looking_for--;

				previously_poped_close_open = poped_close_open;
				prev_added_opens = added_opens;
			}

			if (found)
			{
				for (int n = start_index; n < end_index; n++)
				{
					ExecAtom v = _env.peek_index<ExecAtom>(n);
					_env.push<ExecAtom>(v);
				}
			}
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

	template <class T>
	inline unsigned flush(Environment & _env)
	{
		_env.get_stack<T>().clear();
		return 1;
	}

	template <class T>
	inline unsigned protected_pop(Environment & _env)
	{
		if (_env.has_elements<T>(1))
			_env.pop<T>();

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

	template <class T>
	inline unsigned shove(Environment & _env)
	{
		unsigned int effort = 1;

		if ((_env.has_elements<long>(1)) && (_env.has_elements<T>(1)))
		{
			int insert_position = safe_index<T>(_env);
			unsigned int stack_size = _env.get_stack<T>().size();

			std::array<T, domain::argmap::maximum_stack_size>& stack = _env.get_stack<T>().container();

			if ((stack_size < domain::argmap::maximum_stack_size - 1) && (stack_size > 1))
			{
				T v = _env.top<T>();

				unsigned int stack_pointer = stack_size - 1;

				while (stack_pointer > insert_position)
				{
					stack[stack_pointer + 1] = stack[stack_pointer];
					stack_pointer--;
				}

				stack[stack_pointer + 1] = v;

//				_env.pop<T>();
			}

			effort = stack.size() - insert_position + 1;
		}

		return effort;
	}

}