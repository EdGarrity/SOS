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
		Utilities::FixedSizeStack<class ExecAtom> exec_stack_;
		Utilities::FixedSizeStack<class CodeAtom> code_stack_;
		Utilities::FixedSizeStack<long> int_stack_;
		Utilities::FixedSizeStack<double> double_stack_;
		Utilities::FixedSizeStack<bool> bool_stack_;
		
	public:
		virtual void clear_stacks()
		{
			exec_stack_.clear();
			int_stack_.clear();
			code_stack_.clear();
			bool_stack_.clear();
			double_stack_.clear();
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
		inline void push(Utilities::FixedSizeStack<T> stack)
		{
			//for (int n = 0; n < stack.size(); n++)
			//	get_stack<T>().push(stack[n]);
			if (stack.size() > 0)
				for (int n = stack.size() - 1; n >= 0; n--)
					get_stack<T>().push(stack[n]);
		}

		// Need spcial cases for EXEC and CODE
		// Need to support receiving and returning stack of instructions
		// If number of parenthesis balance, then pop will retuirn first instruction
		// for impabance, then return balanced stack (from first instruction to instuction with matching close parenthesis inclusive)
		template <typename T>
		inline T pop()
		{
			T val = get_stack<T>().top();
			get_stack<T>().pop();
			return val;
		}

		template <typename T> inline int &pop(Utilities::FixedSizeStack<T> &stack, unsigned int open_blocks) { }
		template <> inline int &pop(Utilities::FixedSizeStack<CodeAtom> &stack, unsigned int open_blocks)
		{
			int blocks_open = open_blocks;

			while (get_stack<CodeAtom>().size() > 0)
			{
				Atom atom = get_stack<CodeAtom>().top();
				get_stack<CodeAtom>().pop();
				stack.push(atom);

				blocks_open += Func2BlockWantsMap[atom.instruction];
				blocks_open -= atom.close_parentheses;

				if (blocks_open <= 0)
					break;
			};
			
			blocks_open *= -1;

			return blocks_open; // Return number of unmatched close parenthesis.
		}
		template <> inline int &pop(Utilities::FixedSizeStack<ExecAtom> &stack, unsigned int open_blocks)
		{
			int blocks_open = open_blocks;

			while ((blocks_open > 0) && (get_stack<ExecAtom>().size() > 0))
			{
				Atom atom = get_stack<ExecAtom>().top();
				get_stack<ExecAtom>().pop();
				stack.push(atom);

				blocks_open += Func2BlockWantsMap[atom.instruction];
				blocks_open -= atom.close_parentheses;
			};

			blocks_open *= -1;

			return blocks_open; // Return number of unmatched close parenthesis.
		}

		// Need spcial cases for EXEC and CODE
		template <typename T>
		inline T top()
		{
			T val = get_stack<T>().top();
			return val;
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

		// Need spcial cases for EXEC and CODE
		template <typename T>
		inline T peek_index(unsigned index)
		{
			if (has_elements<T>(index + 1))
				return get_stack<T>().container()[get_stack<T>().size() - index - 1];

			else
				throw;
		}

		template <typename T> inline int &peek_index(unsigned index, unsigned open_blocks, Utilities::FixedSizeStack<T> &stack) { }
		template <> inline int &peek_index(unsigned index, unsigned open_blocks, Utilities::FixedSizeStack<CodeAtom> &stack)
		{
			int i = 0;
			int blocks_open = open_blocks;

			for (int n = 0; n < index + 1; n++)
			{
//				blocks_open = 1;

				if (blocks_open == 0)
				{
					CodeAtom atom = peek_index<CodeAtom>(n);

					if (n == index)
						stack.push(atom);

					blocks_open += Func2BlockWantsMap[atom.instruction];
					blocks_open -= atom.close_parentheses;

					i++;
				}

				else
				{
					while ((blocks_open > 0) && (i < get_stack<CodeAtom>().size()))
					{
						CodeAtom atom = peek_index<CodeAtom>(n);

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
		template <> inline int &peek_index(unsigned index, unsigned open_blocks, Utilities::FixedSizeStack<ExecAtom> &stack)
		{
			int i = 0;
			int blocks_open = open_blocks;

			for (int n = 0; n < index + 1; n++)
			{
				//				blocks_open = 1;

				if (blocks_open == 0)
				{
					ExecAtom atom = peek_index<ExecAtom>(n);

					if (n == index)
						stack.push(atom);

					blocks_open += Func2BlockWantsMap[atom.instruction];
					blocks_open -= atom.close_parentheses;

					i++;
				}

				else
				{
					while ((blocks_open > 0) && (i < get_stack<ExecAtom>().size()))
					{
						ExecAtom atom = peek_index<ExecAtom>(n);

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
