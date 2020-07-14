#pragma once

#include "Atom.h"
#include <vector>
#include "..\Utilities\FixedSizeStack.h"

namespace Plush
{
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

		template <class T, size_t N = domain::argmap::maximum_stack_size>
		inline void push(Utilities::FixedSizeStack<T, N> stack)
		{
			for (T element : stack)
				get_stack<T>().push(element);
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

		template <class T, size_t N = domain::argmap::maximum_stack_size>
		inline std::array<T, N> pop(Utilities::FixedSizeStack<T, N> &stack)
		{
			T val = get_stack<T>().top();
			get_stack<T>().pop();
			return stack.push(val);
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
				return get_stack<T>().container()[index];

			else
				throw;
		}
	};
}
