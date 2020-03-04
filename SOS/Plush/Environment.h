#pragma once

#include "Atom.h"
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
		
//		// Returns first atom in genome instruction
//		std::string first_atom(std::string instruction);
//
//		// Returns rest of genome instruction after first instruction
//		std::string rest_atom(std::string instruction);

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

		// Need spcial cases for EXEC and CODE
		template <typename T>
		inline T pop()
		{
			T val = get_stack<T>().top();
			get_stack<T>().pop();
			return val;
		}

		// Need spcial cases for EXEC and CODE
		template <typename T>
		inline T top()
		{
			T val = get_stack<T>().top();
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
		inline T peek(unsigned index)
		{
			if (has_elements<T>(index + 1))
				return get_stack<T>().container()[index];

			else
				throw;
		}
	};
}
