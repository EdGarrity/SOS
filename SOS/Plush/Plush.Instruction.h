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
			_env.push<T>(_env.top<T>());

		return 1;
	}
	template <>
	inline unsigned dup<CodeAtom>(Environment & _env)
	{
		//Utilities::FixedSizeStack<Atom> stack;

		//if (_env.has_elements<CodeAtom>(1))
		//{
		//	_env.top<CodeAtom>(stack, 1);
		//	_env.push<CodeAtom>(stack);
		//}

		Genome<Atom> stack;

		if (_env.has_elements<CodeAtom>(1))
		{
			_env.top<CodeAtom>(stack);
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
			int target_extra_blocks = 0;
			int index = _env.pop<long>();	// index

			if (index > 0)
			{
				Genome<Atom> extracted_block;
				Genome<Atom> target_block;
				Genome<Atom> modified_genome;

				// Get count of sub-blocks
				Utilities::FixedSizeStack<CodeAtom>& stack = _env.get_stack<CodeAtom>();
				Genome<CodeAtom>& genome = dynamic_cast<Genome<CodeAtom>&>(stack);

				int number_of_blocks = genome.number_of_blocks();

				// If the index is larger than the size of the specified stack, then the deepest element is `yank`ed up to the top.
				index = (index > (number_of_blocks - 1)) ? (number_of_blocks - 1) : index;
				int n = 0;

				while (_env.is_empty<CodeAtom>() == false)
				{
					int extra_blocks = _env.pop<CodeAtom>(extracted_block);

					if (n == index)
					{
						target_extra_blocks = extra_blocks;
						extracted_block[0].close_parentheses -= extra_blocks;
						target_block.push(extracted_block);

						if (extra_blocks > 0)
						{
							std::string noop = "{:instruction EXEC.NOOP :close " + std::to_string(extra_blocks) + "}";

							Genome<Atom> modified_genome_copy(modified_genome);
							modified_genome.clear();
							modified_genome.push(noop);
							modified_genome.push(modified_genome_copy);
						}
					}

					else
					{
						Genome<Atom> modified_genome_copy(modified_genome);
						modified_genome.clear();
						modified_genome.push(extracted_block);
						modified_genome.push(modified_genome_copy);
					}

					n++;
				}

//				modified_genome[0].close_parentheses += target_extra_blocks;

				//while (modified_genome.empty() != false)
				//{
				//	Genome<Atom> a_genome;

				//	modified_genome.pop_genome(a_genome);
				//	_env.push<CodeAtom>(a_genome);
				//}

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
					int blocks_open = 0;

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
			Genome<Atom> extracted_block;
			Genome<Atom> temp_genome;
			int extra_blocks = 0;

			// Get number of blocks on the stack
			int number_of_blocks = _env.length<CodeAtom>();

			// If the index is larger than the size of the specified stack, then the deepest element is `yank`ed up to the top.
			index = (index > (number_of_blocks - 1)) ? (number_of_blocks - 1) : index;
			index = (index < 0) ? 0 : index;

			while ((_env.is_empty<CodeAtom>() == false) && (index >= 0))
			{
				extra_blocks = _env.pop<CodeAtom>(extracted_block);
				temp_genome.push(extracted_block);

//				if ((index - extra_blocks) == 0)
				if ((index) == 0)
					extracted_block[0].close_parentheses = 1;

				else if ((index - extra_blocks) <= 0)
				{
					extracted_block.clear();
					extracted_block.push(CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 1}"));
				}

				index -= extra_blocks;
				index--;
			}

			while (temp_genome.empty() == false)
			{
				Genome<Atom> temp_block;
				temp_genome.pop_genome(temp_block);
				_env.push<CodeAtom>(temp_block);
			}

			_env.push<CodeAtom>(extracted_block);
		}

		return 1;








		//if ((_env.has_elements<long>(1)) && (_env.has_elements<CodeAtom>(1)))
		//{
		//	int index = _env.pop<long>();	// index

		//	Utilities::FixedSizeStack<CodeAtom> &stack = _env.get_stack<CodeAtom>();
		//	Utilities::FixedSizeStack<Atom> extracted_block;

		//	if (index >= 0)
		//	{
		//		// Get count of sub-blocks
		//		int number_of_blocks = 0;
		//		int n = stack.size() - 1;

		//		do
		//		{
		//			int blocks_open = 1;

		//			for (; n >= 0; n--)
		//			{
		//				Plush::Atom atom = stack[n];

		//				blocks_open += Plush::Func2BlockWantsMap[atom.instruction];
		//				blocks_open -= atom.close_parentheses;
		//				blocks_open = (blocks_open > 0) ? blocks_open : 0;

		//				if (atom.close_parentheses > 0)
		//				{
		//					if (blocks_open > 0)
		//						blocks_open++;

		//					else
		//					{
		//						number_of_blocks++;
		//						blocks_open = 1;
		//					}
		//				}
		//			};
		//		} while (n >= 0);

		//		// If the index is larger than the size of the specified stack, then the deepest element is `yank`ed up to the top.
		//		index = (index > (number_of_blocks - 1)) ? (number_of_blocks - 1) : index;

		//		// Get the target sub-block
		//		n = stack.size() - 1;
		//		int block_number = 0;

		//		do
		//		{
		//			int blocks_open = 1;

		//			for (; n >= 0; n--)
		//			{
		//				Plush::Atom atom = stack[n];

		//				if (block_number == index)
		//					extracted_block.push(atom);

		//				blocks_open += Plush::Func2BlockWantsMap[atom.instruction];
		//				blocks_open -= atom.close_parentheses;
		//				blocks_open = (blocks_open > 0) ? blocks_open : 0;

		//				if (atom.close_parentheses > 0)
		//				{
		//					if (blocks_open > 0)
		//						blocks_open++;

		//					else
		//					{
		//						block_number++;
		//						blocks_open = 1;
		//					}
		//				}
		//			};
		//		} while (n >= 0);

		//		_env.push<CodeAtom>(extracted_block);
		//	}
		//}

		//return 1;
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
			unsigned int extra_first_blocks = 0;
			unsigned int extra_second_bloxks = 0;

			Genome<class Atom> first_block;
			Genome<class Atom> second_block;

			// Get first block from stack
			extra_first_blocks = _env.pop<ExecAtom>(first_block);

			if (extra_first_blocks == 0)
			{
				// Get second block from stack
				extra_second_bloxks = _env.pop<ExecAtom>(second_block);

				// Make sure the second block contains only one block.
				if (extra_second_bloxks > 0)
					second_block[0].close_parentheses = 1;
			}
			else
			{
				// Create a NOOP second block and decrease the extra blocks in the first item by one
				second_block.push(Atom("{:instruction EXEC.NOOP_OPEN_PAREN :close 1}"));
				first_block[0].close_parentheses--;
			}

			unsigned int first_block_length = first_block.number_of_blocks();
			unsigned int second_block_length = second_block.number_of_blocks();

			if ((first_block_length > 0) && (second_block_length > 0))
			{
				if (first_block_length != second_block_length)
					result = false;
				
				else
				{
					for (int n = 0; n < first_block_length; n++)
					{
						if ((dynamic_cast<Genome<ExecAtom>&>(first_block))[n] != (dynamic_cast<Genome<ExecAtom>&>(second_block))[n])
						{
							result = false;
							break;
						}
					}
				}

				_env.push<bool>(result);
			}
			else
			{
				_env.push<ExecAtom>(second_block);
				_env.push<ExecAtom>(first_block);
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
			Genome<Atom> extracted_block_A;
			Genome<Atom> extracted_block_B;
			Genome<Atom> extracted_block_C;
			int extra_blocks = 0;
			int extra_blocks_B = 0;
			int extra_blocks_C = 0;

			// Get first block from stack
			extra_blocks = _env.pop<CodeAtom>(extracted_block_A);
			extracted_block_A[0].close_parentheses -= extra_blocks;

			// Get or create second block
			if (extra_blocks > 0)
			{
				// Create a NOOP second block and decrease the extra blocks in the first item by one
				extracted_block_B.push(Atom("{:instruction EXEC.NOOP_OPEN_PAREN :close 1}"));
				extra_blocks--;
			}

			else
			{
				// Get second block from stack
				extra_blocks_B = _env.pop<CodeAtom>(extracted_block_B);
				extracted_block_B[0].close_parentheses -= extra_blocks_B;
				extra_blocks += extra_blocks_B;
			}

			// Get or create third block
			if (extra_blocks > 0)
			{
				// Create a NOOP second block and decrease the extra blocks in the first item by one
				extracted_block_C.push(Atom("{:instruction EXEC.NOOP_OPEN_PAREN :close 1}"));
				extra_blocks--;
			}

			else
			{
				// Get second block from stack
				extra_blocks_C = _env.pop<CodeAtom>(extracted_block_C);
				extracted_block_C[0].close_parentheses -= extra_blocks_C;
				extra_blocks += extra_blocks_C;
			}

			// Balance parenthesis
			extracted_block_B[0].close_parentheses += extra_blocks;

			// Get length of blocks
			//unsigned int extracted_block_A_length = extracted_block_A.length();
			//unsigned int extracted_block_B_length = extracted_block_B.length();
			//unsigned int extracted_block_C_length = extracted_block_C.length();

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
				_env.push<ExecAtom>(block_b);
				_env.push<ExecAtom>(block_a);
				_env.push<ExecAtom>(block_c);
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
			int extra_blocks = 0;
			int simulated_closing_parenthesis = 0;
			int index = _env.pop<long>();	// index

			Genome<class Atom> first_block;
			Genome<class Atom> top_half;
			Genome<class Atom> bottom_half;
			Genome<class Atom> top_block;
			Genome<class Atom> bottom_block;
			Genome<class Atom> genome;
			Genome<class Atom> temp_block;

			if (index > 0)
			{
				// Get first block from stack
				_env.pop<CodeAtom>(first_block);

				if (first_block.size() == 0)
					_env.push<CodeAtom>(first_block);

				else
				{
					int n = 0;
					while (_env.is_empty<CodeAtom>() == false)
					{
						if (simulated_closing_parenthesis == 0)
							extra_blocks = _env.pop<CodeAtom>(genome);

						if (simulated_closing_parenthesis > 0)
						{
							genome.clear();
							Plush::Atom atom = Plush::Atom("{:instruction EXEC.NOOP :close 1}");
							genome.push(atom);

							simulated_closing_parenthesis--;
							extra_blocks = 0;
						}

						if (extra_blocks > 0)
						{
							if ((n + 1) == index)
							{
								simulated_closing_parenthesis = extra_blocks;
								genome[0].close_parentheses = 1;;
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
			int index = _env.pop<long>();	// index

			Utilities::FixedSizeStack<ExecAtom> &stack = _env.get_stack<ExecAtom>();
			Utilities::FixedSizeStack<Atom> extracted_block;
			Utilities::FixedSizeStack<Atom> block_pre_extracted;
			Utilities::FixedSizeStack<Atom> block_post_extracted;

			if (index > 0)
			{
				// Get count of sub-blocks
				int number_of_blocks = _env.NumberOfBlocks<CodeAtom>(0);

				// If the index is larger than the size of the specified stack, then the deepest element is `yank`ed up to the top.
				index = (index > (number_of_blocks - 1)) ? (number_of_blocks - 1) : index;

				// Get the top block
				_env.pop<ExecAtom>(extracted_block, 1);

				// Get the target sub-block
				int n = stack.size() - 1;
				int block_number = 0;

				do
				{
					int blocks_open = 0;

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
								block_number += atom.close_parentheses;
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
			Genome<Atom> extracted_block_A;
			Genome<Atom> extracted_block_B;
			int extra_blocks = 0;
			int extra_blocks_B = 0;

			// Get first block from stack
			extra_blocks = _env.pop<CodeAtom>(extracted_block_A);
			extracted_block_A[0].close_parentheses -= extra_blocks;

			// Get or create second block
			if (extra_blocks > 0)
			{
				// Create a NOOP second block and decrease the extra blocks in the first item by one
				extracted_block_B.push(Atom("{:instruction EXEC.NOOP_OPEN_PAREN :close 1}"));
				extra_blocks--;
			}

			else
			{
				// Get second block from stack
				extra_blocks_B = _env.pop<CodeAtom>(extracted_block_B);
				extracted_block_B[0].close_parentheses -= extra_blocks_B;
				extra_blocks += extra_blocks_B;
			}

			// Balance parenthesis
			extracted_block_A[0].close_parentheses += extra_blocks;

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
			Utilities::FixedSizeStack<Atom> block_a;
			Utilities::FixedSizeStack<Atom> block_b;

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

			if ((block_a.size() > 0) && (block_b.size() > 0))
			{
				_env.push<ExecAtom>(block_a);
				_env.push<ExecAtom>(block_b);
			}

			else
			{
				_env.push<ExecAtom>(block_b);
				_env.push<ExecAtom>(block_a);
			}
		}

		return 1;
	}
}