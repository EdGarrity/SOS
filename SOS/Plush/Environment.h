#pragma once

//#include "Processor.h"
#include "Atom.h"
#include <vector>
#include "..\Utilities\FixedSizeStack.h"

namespace Plush
{
	typedef std::map<std::string, unsigned int> Func2BlockWantsMapType;
	extern Func2BlockWantsMapType Func2BlockWantsMap;

	class Environment
	{
	private:
		// Stacks
		Utilities::FixedSizeStack<class ExecAtom> exec_stack_;
		Utilities::FixedSizeStack<class CodeAtom> code_stack_;
		Utilities::FixedSizeStack<long> int_stack_;
		Utilities::FixedSizeStack<double> double_stack_;
		Utilities::FixedSizeStack<bool> bool_stack_;
		
	public:
		// Keep track of the number of wanted blocks
		unsigned int blocks_wanted;

		virtual void clear_stacks()
		{
			exec_stack_.clear();
			int_stack_.clear();
			code_stack_.clear();
			bool_stack_.clear();
			double_stack_.clear();

			blocks_wanted = 0;
		}

		/* Helper Functions */
		unsigned int NumberOfBlocks(Utilities::FixedSizeStack<Atom> &stack)
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

					int closing = atom.close_parentheses - Plush::Func2BlockWantsMap[atom.instruction];

					if ((closing > 0) && (blocks_open == 0))
						number_of_blocks += atom.close_parentheses;

					blocks_open -= closing;
					blocks_open = (blocks_open > 0) ? blocks_open : 0;
				};
			} while (n >= 0);

			return number_of_blocks;
		}

		template <typename T>
		unsigned int NumberOfBlocks(unsigned int block_level)
		{
			Utilities::FixedSizeStack<T> &stack = get_stack<T>();

			// Get count of sub-blocks
			int number_of_blocks = 0;
			int n = stack.size() - 1;

			do
			{
				int blocks_open = block_level;

				for (; n >= 0; n--)
				{
					Plush::Atom atom = stack[n];

					//blocks_open += Plush::Func2BlockWantsMap[atom.instruction];
					//blocks_open -= atom.close_parentheses;
					//blocks_open = (blocks_open > 0) ? blocks_open : 0;

					//if (atom.close_parentheses > 0)
					//{
					//	if (blocks_open > 0)
					//		blocks_open++;

					//	else
					//	{
					//		number_of_blocks += atom.close_parentheses;
					//		blocks_open = block_level;
					//	}
					//}


					int closing = atom.close_parentheses - Plush::Func2BlockWantsMap[atom.instruction];

					if ((closing > 0) && (blocks_open == 0))
						number_of_blocks += atom.close_parentheses;

					blocks_open -= closing;
					blocks_open = (blocks_open > 0) ? blocks_open : 0;
				};
			} while (n >= 0);

			return number_of_blocks;
		}

		/* Operations */
		template <typename T> inline Utilities::FixedSizeStack<T> &get_stack() { }
		template <> inline Utilities::FixedSizeStack<ExecAtom> &get_stack()
		{
			return exec_stack_;
		}
		template <> inline Utilities::FixedSizeStack<CodeAtom> &get_stack()
		{
			return code_stack_;
		}
		template <> inline Utilities::FixedSizeStack<long> &get_stack()
		{
			return int_stack_;
		}
		template <> inline Utilities::FixedSizeStack<double> &get_stack()
		{
			return double_stack_;
		}
		template <> inline Utilities::FixedSizeStack<bool> &get_stack()
		{
			return bool_stack_;
		}
		
		template <typename T> inline bool is_empty()
		{
			return (get_stack<T>().empty());
		}

		/* pushing and popping */

		// Need spcial cases for pushing an array of EXEC and CODE
		template <typename T>
		inline void push(T value)
		{
			get_stack<T>().push(value);
		}

		template <class T>
		inline void push(Utilities::FixedSizeStack<Atom> stack)
		{
			if (stack.size() > 0)
				for (int n = stack.size() - 1; n >= 0; n--)
					get_stack<T>().push(stack[n]);
		}

		template <typename T>
		inline T pop()
		{
			T val = get_stack<T>().top();
			get_stack<T>().pop();
			return val;
		}

		template <typename T> 
		inline int pop(Utilities::FixedSizeStack<Atom> &stack, unsigned int open_blocks)
		{
			int block_count = open_blocks;

			stack.clear();

			while (get_stack<T>().size() > 0)
			{
				Atom atom = get_stack<T>().top();
				get_stack<T>().pop();
				stack.push(atom);

				block_count += Func2BlockWantsMap[atom.instruction];
				block_count -= atom.close_parentheses;

				//if ((atom.close_parentheses > 0) && (block_count > 0))
				//	block_count++;

				if (block_count <= 0)
					break;
			};
			
			block_count *= -1;

			return block_count; // Return number of unmatched close parenthesis.
		}

		// Need spcial cases for EXEC and CODE
		template <typename T>
		inline T top()
		{
			T val = get_stack<T>().top();
			return val;
		}

		template <typename T>
		inline int top(Utilities::FixedSizeStack<Atom> &stack, unsigned int open_blocks)
		{
			int blocks_open = open_blocks;

			stack.clear();

			for (int n = get_stack<T>().size() - 1; n >= 0; n--)
			{
				Atom atom = get_stack<T>()[n];
				stack.push(atom);

				blocks_open += Func2BlockWantsMap[atom.instruction];
				blocks_open -= atom.close_parentheses;

				if (blocks_open <= 0)
					break;
			};

			blocks_open *= -1;

			return blocks_open; // Return number of unmatched close parenthesis.
		}

		template <typename T>
		inline T top(T val)
		{
			get_stack<T>().top() = val;
			return val;
		}

		// Need spcial cases for EXEC and CODE
		template <typename T>
		inline bool has_elements(unsigned sz)
		{
			if (get_stack<T>().size() < sz)
				return false;
			else
				return true;
		}

		template <>
		inline bool has_elements<ExecAtom>(unsigned sz)
		{
			if (get_stack<ExecAtom>().size() < sz)
				return false;

			else
			{
				//Utilities::FixedSizeStack<ExecAtom> &stack = get_stack<ExecAtom>();

				//// Get count of sub-blocks
				//int number_of_blocks = 0;
				//int n = stack.size() - 1;

				//do
				//{
				//	int blocks_open = 0;

				//	for (; n >= 0; n--)
				//	{
				//		Plush::Atom atom = stack[n];

				//		blocks_open += Plush::Func2BlockWantsMap[atom.instruction];
				//		blocks_open -= atom.close_parentheses;
				//		blocks_open = (blocks_open > 0) ? blocks_open : 0;

				//		if (atom.close_parentheses > 0)
				//		{
				//			if (blocks_open > 0)
				//				blocks_open++;

				//			else
				//			{
				//				number_of_blocks += atom.close_parentheses;
				//				blocks_open = 1;
				//			}
				//		}
				//	};
				//} while (n >= 0);

				//if (number_of_blocks < sz)
				//	return false;

				//else
				//	return true;

				if (NumberOfBlocks<ExecAtom>(0) < sz)
					return false;

				else
					return true;
			}
		}

		template <typename T>
		inline T peek_index(unsigned index)
		{
			if (has_elements<T>(index + 1))
				return get_stack<T>().container()[get_stack<T>().size() - index - 1];

			else
				throw;
		}

		template <typename T> inline int &peek_index(unsigned index, unsigned open_blocks, Utilities::FixedSizeStack<Atom> &stack)
		{
			int i = 0;
			int blocks_open = open_blocks;

			stack.clear();

			for (int n = 0; n < index + 1; n++)
			{
				if (blocks_open == 0)
				{
					Atom atom = peek_index<T>(n);

					if (n == index)
						stack.push(atom);

					blocks_open += Func2BlockWantsMap[atom.instruction];
					blocks_open -= atom.close_parentheses;

					i++;
				}

				else
				{
					while ((blocks_open > 0) && (i < get_stack<T>().size()))
					{
						Atom atom = peek_index<T>(n);

						if (n == index)
							stack.push(atom);

						blocks_open += Func2BlockWantsMap[atom.instruction];
						blocks_open -= atom.close_parentheses;

						i++;
					};
				}
			}

			blocks_open *= -1;

			return blocks_open; // Return number of unmatched close parenthesis.
		}
	};
}
