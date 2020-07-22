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
	template <>
	inline unsigned dup<CodeAtom>(Environment & _env)
	{
		Utilities::FixedSizeStack<Atom> stack;

		if (_env.has_elements<CodeAtom>(1))
		{
			_env.top<CodeAtom>(stack, 1);
			_env.push<CodeAtom>(stack);
		}

		return 1;
	}
	template <>
	inline unsigned dup<ExecAtom>(Environment & _env)
	{
		Utilities::FixedSizeStack<Atom> stack;

		if (_env.has_elements<ExecAtom>(1))
		{
			_env.top<ExecAtom>(stack, 1);
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

				std::array<T, domain::argmap::maximum_stack_size>& stack = _env.get_stack<T>().container();

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

			Utilities::FixedSizeStack<CodeAtom> &stack = _env.get_stack<CodeAtom>();
			Utilities::FixedSizeStack<Atom> extracted_block;
			Utilities::FixedSizeStack<Atom> block_without_extracted;

			if (index > 0)
			{
				// Get count of sub-blocks
				int number_of_blocks = 0;
				int n = stack.size() - 1;

				do
				{
					int blocks_open = 1;

					for (; n >= 0; n--)
					{
						Plush::Atom atom = stack[n];

						blocks_open += Plush::Func2BlockWantsMap[atom.instruction];
						blocks_open -= atom.close_parentheses;
						blocks_open = (blocks_open > 0) ? blocks_open : 0;

						if (atom.close_parentheses > 0)
						{
							if (blocks_open > 0)
								blocks_open++;

							else
							{
								number_of_blocks++;
								blocks_open = 1;
							}
						}
					};
				} while (n >= 0);

				// If the index is larger than the size of the specified stack, then the deepest element is `yank`ed up to the top.
				index = (index > (number_of_blocks - 1)) ? (number_of_blocks - 1) : index;

				// Get the target sub-block
				n = stack.size() - 1;
				int block_number = 0;

				do
				{
					int blocks_open = 1;

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

				_env.get_stack<CodeAtom>().clear();
				_env.push<CodeAtom>(block_without_extracted);
				_env.push<CodeAtom>(extracted_block);
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

			Utilities::FixedSizeStack<ExecAtom> &stack = _env.get_stack<ExecAtom>();
			Utilities::FixedSizeStack<Atom> extracted_block;
			Utilities::FixedSizeStack<Atom> block_without_extracted;

			if (index > 0)
			{
				// Get count of sub-blocks
				int number_of_blocks = 0;
				int n = stack.size() - 1;

				do
				{
					int blocks_open = 1;

					for (; n >= 0; n--)
					{
						Plush::Atom atom = stack[n];

						blocks_open += Plush::Func2BlockWantsMap[atom.instruction];
						blocks_open -= atom.close_parentheses;
						blocks_open = (blocks_open > 0) ? blocks_open : 0;

						if (atom.close_parentheses > 0)
						{
							if (blocks_open > 0)
								blocks_open++;

							else
							{
								number_of_blocks++;
								blocks_open = 1;
							}
						}
					};
				} while (n >= 0);

				// If the index is larger than the size of the specified stack, then the deepest element is `yank`ed up to the top.
				index = (index > (number_of_blocks - 1)) ? (number_of_blocks - 1) : index;

				// Get the target sub-block
				n = stack.size() - 1;
				int block_number = 0;

				do
				{
					int blocks_open = 1;

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
				_env.push<ExecAtom>(extracted_block);
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

			Utilities::FixedSizeStack<CodeAtom> &stack = _env.get_stack<CodeAtom>();
			Utilities::FixedSizeStack<Atom> extracted_block;

			if (index >= 0)
			{
				// Get count of sub-blocks
				int number_of_blocks = 0;
				int n = stack.size() - 1;

				do
				{
					int blocks_open = 1;

					for (; n >= 0; n--)
					{
						Plush::Atom atom = stack[n];

						blocks_open += Plush::Func2BlockWantsMap[atom.instruction];
						blocks_open -= atom.close_parentheses;
						blocks_open = (blocks_open > 0) ? blocks_open : 0;

						if (atom.close_parentheses > 0)
						{
							if (blocks_open > 0)
								blocks_open++;

							else
							{
								number_of_blocks++;
								blocks_open = 1;
							}
						}
					};
				} while (n >= 0);

				// If the index is larger than the size of the specified stack, then the deepest element is `yank`ed up to the top.
				index = (index > (number_of_blocks - 1)) ? (number_of_blocks - 1) : index;

				// Get the target sub-block
				n = stack.size() - 1;
				int block_number = 0;

				do
				{
					int blocks_open = 1;

					for (; n >= 0; n--)
					{
						Plush::Atom atom = stack[n];

						if (block_number == index)
							extracted_block.push(atom);

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

				_env.push<CodeAtom>(extracted_block);
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
			int n = _env.pop<long>();

			Utilities::FixedSizeStack<Atom> code_block;

			_env.peek_index<ExecAtom>(n, 0, code_block);
			_env.push<ExecAtom>(code_block);
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
			bool result = true;

			Utilities::FixedSizeStack<Atom> block_a;
			Utilities::FixedSizeStack<Atom> block_b;

			int unmatched_a = _env.pop<CodeAtom>(block_a, 1);
			int unmatched_b = _env.pop<CodeAtom>(block_b, 1);

			if (block_a.size() != block_b.size())
				result = false;

			else
			{
				while (block_a.size() > 0)
				{
					CodeAtom atom_a = block_a.top();
					CodeAtom atom_b = block_b.top();

					block_a.pop();
					block_b.pop();

					if (atom_a != atom_b)
						result = false;
				}
			}

			_env.push<bool>(result);
		}

		return 1;
	}
		
	template<>
	inline unsigned equals<ExecAtom>(Environment & _env)
	{
		// Check for valid parameters
		if (_env.has_elements<ExecAtom>(2))
		{
			bool result = true;

			Utilities::FixedSizeStack<Atom> block_a;
			Utilities::FixedSizeStack<Atom> block_b;

			int unmatched_a = _env.pop<ExecAtom>(block_a, 1);
			int unmatched_b = _env.pop<ExecAtom>(block_b, 1);

			if ((block_a.size() == 0) || (block_b.size() == 0))
			{
				_env.push<ExecAtom>(block_b);
				_env.push<ExecAtom>(block_a);
			}

			else if (block_a.size() != block_b.size())
			{
				_env.push<bool>(false);
			}

			else
			{
				while (block_a.size() > 0)
				{
					ExecAtom atom_a = block_a.top();
					ExecAtom atom_b = block_b.top();

					block_a.pop();
					block_b.pop();

					if (atom_a != atom_b)
					{
						result = false;
						break;
					}
				}

				_env.push<bool>(result);
			}
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
		Utilities::FixedSizeStack<Atom> stack;

		if (_env.has_elements<CodeAtom>(1))
			_env.pop<CodeAtom>(stack, 1);

		return 1;
	}
	template <>
	inline unsigned protected_pop<ExecAtom>(Environment & _env)
	{
		Utilities::FixedSizeStack<Atom> stack;

		if (_env.has_elements<ExecAtom>(1))
			_env.pop<ExecAtom>(stack, 1);

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
			Utilities::FixedSizeStack<CodeAtom> &stack = _env.get_stack<CodeAtom>();
			Utilities::FixedSizeStack<Atom> extracted_block_A;
			Utilities::FixedSizeStack<Atom> extracted_block_B;
			Utilities::FixedSizeStack<Atom> extracted_block_C;
			Utilities::FixedSizeStack<Atom> block_without_extracted;

			// Get count of sub-blocks
			int number_of_blocks = 0;
			int n = stack.size() - 1;

			n = stack.size() - 1;
			int block_number = 0;

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

			//int B_paranethesis = extracted_block_B[extracted_block_B.size() - 1].close_parentheses;
			//extracted_block_B[extracted_block_B.size() - 1].close_parentheses = extracted_block_C[extracted_block_C.size() - 1].close_parentheses;
			//extracted_block_C[extracted_block_C.size() - 1].close_parentheses = B_paranethesis;

			//extracted_block_C[extracted_block_C.size() - 1].close_parentheses =
			//	extracted_block_C[extracted_block_C.size() - 1].close_parentheses == 0
			//	? 1 
			//	: extracted_block_C[extracted_block_C.size() - 1].close_parentheses;

			_env.get_stack<CodeAtom>().clear();
			_env.push<CodeAtom>(block_without_extracted);
			_env.push<CodeAtom>(extracted_block_B);
			_env.push<CodeAtom>(extracted_block_A);
			_env.push<CodeAtom>(extracted_block_C);
		}

		return 1;
	}

	template<>
	inline unsigned rot<ExecAtom>(Environment & _env)
	{
		if (_env.has_elements<ExecAtom>(3))
		{
			_env.push<long>(2);
			_env.push<ExecAtom>(CodeAtom("{:instruction EXEC.YANK :close 0}"));
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

			std::array<T, domain::argmap::maximum_stack_size>& stack = _env.get_stack<T>().container();

			if ((stack_size < domain::argmap::maximum_stack_size - 1) && (stack_size > 1))
			{
				T v = _env.top<T>();

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

			Utilities::FixedSizeStack<CodeAtom> &stack = _env.get_stack<CodeAtom>();
			Utilities::FixedSizeStack<Atom> extracted_block;
			Utilities::FixedSizeStack<Atom> block_pre_extracted;
			Utilities::FixedSizeStack<Atom> block_post_extracted;

			if (index > 0)
			{
				// Get count of sub-blocks
				int number_of_blocks = 0;
				int n = stack.size() - 1;

				do
				{
					int blocks_open = 1;

					for (; n >= 0; n--)
					{
						Plush::Atom atom = stack[n];

						blocks_open += Plush::Func2BlockWantsMap[atom.instruction];
						blocks_open -= atom.close_parentheses;
						blocks_open = (blocks_open > 0) ? blocks_open : 0;

						if (atom.close_parentheses > 0)
						{
							if (blocks_open > 0)
								blocks_open++;

							else
							{
								number_of_blocks++;
								blocks_open = 1;
							}
						}
					};
				} while (n >= 0);

				// If the index is larger than the size of the specified stack, then the deepest element is `yank`ed up to the top.
				index = (index > (number_of_blocks - 1)) ? (number_of_blocks - 1) : index;

				// Get the top block
				_env.pop<CodeAtom>(extracted_block, 1);

				// Get the target sub-block
				n = stack.size() - 1;
				int block_number = 0;

				do
				{
					int blocks_open = 1;

					for (; n >= 0; n--)
					{
						Atom atom = stack[n];

						if (block_number < index)
							block_pre_extracted.push(atom);

						else
							block_post_extracted.push(atom);

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

				_env.get_stack<CodeAtom>().clear();
				_env.push<CodeAtom>(block_post_extracted);
				_env.push<CodeAtom>(extracted_block);
				_env.push<CodeAtom>(block_pre_extracted);
			}
		}

		return 1;
	}

	template<>
	inline unsigned shove<ExecAtom>(Environment & _env)
	{
		if ((_env.has_elements<long>(1)) && (_env.has_elements<ExecAtom>(1)))
		{
			int index = _env.pop<long>();	// index

			Utilities::FixedSizeStack<ExecAtom> &stack = _env.get_stack<ExecAtom>();
			Utilities::FixedSizeStack<Atom> extracted_block;
			Utilities::FixedSizeStack<Atom> block_pre_extracted;
			Utilities::FixedSizeStack<Atom> block_post_extracted;

			if (index > 0)
			{
				// Get count of sub-blocks
				int number_of_blocks = 0;
				int n = stack.size() - 1;

				do
				{
					int blocks_open = 1;

					for (; n >= 0; n--)
					{
						Plush::Atom atom = stack[n];

						blocks_open += Plush::Func2BlockWantsMap[atom.instruction];
						blocks_open -= atom.close_parentheses;
						blocks_open = (blocks_open > 0) ? blocks_open : 0;

						if (atom.close_parentheses > 0)
						{
							if (blocks_open > 0)
								blocks_open++;

							else
							{
								number_of_blocks++;
								blocks_open = 1;
							}
						}
					};
				} while (n >= 0);

				// If the index is larger than the size of the specified stack, then the deepest element is `yank`ed up to the top.
				index = (index > (number_of_blocks - 1)) ? (number_of_blocks - 1) : index;

				// Get the top block
				_env.pop<ExecAtom>(extracted_block, 1);

				// Get the target sub-block
				n = stack.size() - 1;
				int block_number = 0;

				do
				{
					int blocks_open = 1;

					for (; n >= 0; n--)
					{
						Atom atom = stack[n];

						if (block_number < index)
							block_pre_extracted.push(atom);

						else
							block_post_extracted.push(atom);

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
				_env.push<ExecAtom>(block_post_extracted);
				_env.push<ExecAtom>(extracted_block);
				_env.push<ExecAtom>(block_pre_extracted);
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
			std::array<T, domain::argmap::maximum_stack_size>& stack = _env.get_stack<T>().container();
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
			Utilities::FixedSizeStack<CodeAtom> &stack = _env.get_stack<CodeAtom>();
			Utilities::FixedSizeStack<Atom> extracted_block_A;
			Utilities::FixedSizeStack<Atom> extracted_block_B;
			Utilities::FixedSizeStack<Atom> block_without_extracted;

			// Get count of sub-blocks
			int number_of_blocks = 0;
			int n = stack.size() - 1;

			n = stack.size() - 1;
			int block_number = 0;

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

			if ((extracted_block_A.size() > 0) && (extracted_block_B.size() > 0))
			{
				_env.get_stack<CodeAtom>().clear();
				_env.push<CodeAtom>(block_without_extracted);
				_env.push<CodeAtom>(extracted_block_A);
				_env.push<CodeAtom>(extracted_block_B);
			}
		}

		return 1;
	}

	template<>
	inline unsigned swap<ExecAtom>(Environment & _env)
	{
		if (_env.has_elements<ExecAtom>(2))
		{
			Utilities::FixedSizeStack<ExecAtom> &stack = _env.get_stack<ExecAtom>();
			Utilities::FixedSizeStack<Atom> extracted_block_A;
			Utilities::FixedSizeStack<Atom> extracted_block_B;
			Utilities::FixedSizeStack<Atom> block_without_extracted;

			// Get count of sub-blocks
			int number_of_blocks = 0;
			int n = stack.size() - 1;

			n = stack.size() - 1;
			int block_number = 0;

			do
			{
				int blocks_open = 1;

				for (; n >= 0; n--)
				{
					Plush::Atom atom = _env.pop<ExecAtom>(); //stack[n];

					if (block_number == 0)
						extracted_block_A.push(atom);

					else if (block_number == 1)
						extracted_block_B.push(atom);

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

			if ((extracted_block_A.size() > 0) && (extracted_block_B.size() > 0))
			{
				_env.get_stack<ExecAtom>().clear();
				_env.push<ExecAtom>(block_without_extracted);
				_env.push<ExecAtom>(extracted_block_A);
				_env.push<ExecAtom>(extracted_block_B);
			}
		}

		return 1;
	}
}