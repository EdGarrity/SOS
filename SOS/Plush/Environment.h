#pragma once

#include <vector>
#include <map>
#include <stdexcept>
#include "Genome.h"

namespace Plush
{
	typedef std::map<std::string, unsigned int> Func2BlockWantsMapType;
	extern Func2BlockWantsMapType Func2BlockWantsMap;

	class Environment
	{
	private:
		// Stacks
		Genome<ExecAtom> exec_stack_;
		Genome<CodeAtom> code_stack_;
		//Utilities::FixedSizeStack<long> int_stack_;
		//Utilities::FixedSizeStack<double> double_stack_;
		//Utilities::FixedSizeStack<bool> bool_stack_;
		Genome<long> int_stack_;
		Genome<double> double_stack_;
		Genome<bool> bool_stack_;

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

		template<typename T>
		unsigned int length()
		{
			Utilities::FixedSizeStack<T>& stack = get_stack<T>();
			return stack.size();
		}

		template<>
		unsigned int length<CodeAtom>()
		{
			//Utilities::FixedSizeStack<CodeAtom>& stack = get_stack<CodeAtom>();
			//Genome<CodeAtom>& genome = dynamic_cast<Genome<CodeAtom>&>(stack);

			Genome<CodeAtom>& genome = get_stack<CodeAtom>();
			return genome.number_of_blocks();
		}

		template<>
		unsigned int length<ExecAtom>()
		{
			//Utilities::FixedSizeStack<ExecAtom>& stack = get_stack<ExecAtom>();
			//Genome<ExecAtom>& genome = dynamic_cast<Genome<ExecAtom>&>(stack);

			Genome<ExecAtom>& genome = get_stack<ExecAtom>();
			return genome.number_of_blocks();
		}

		template <typename T> inline Genome<T> &get_stack(){}
		template <> inline Genome<ExecAtom> &get_stack()
		{
			return exec_stack_;
		}
		template <> inline Genome<CodeAtom> &get_stack()
		{
			return code_stack_;
		}
		template <> inline Genome<long> &get_stack()
		{
			return int_stack_;
		}
		template <> inline Genome<double> &get_stack()
		{
			return double_stack_;
		}
		template <> inline Genome<bool> &get_stack()
		{
			return bool_stack_;
		}
		
		template <typename T> inline bool is_empty()
		{
			return (get_stack<T>().empty());
		}

		/* pushing and popping */

		//template <typename T>
		//inline void push(T& value)
		//{
		//	get_stack<T>().push(value);
		//}

		// Check if all calls to this function should use push(Genome<T>) instead
		//template <class T>
		//inline void push(Genome<Atom> &stack)
		//{
		//	if (stack.size() > 0)
		//		for (int n = 0; n < stack.size(); n++)
		//			get_stack<T>().push(stack[n]);
		//}

		// Check if all calls to this function should use push(Genome<T>) instead
		//template <>
		//inline void push<CodeAtom>(Utilities::FixedSizeStack<Atom> &stack)
		//{
		//	Genome<CodeAtom>& genome = dynamic_cast<Genome<CodeAtom>&>(stack);
		//	push(genome);
		//}

		// Check if all calls to this function should use push(Genome<T>) instead
		//template <>
		//inline void push<ExecAtom>(Utilities::FixedSizeStack<Atom> &stack)
		//{
		//	Genome<CodeAtom>& genome = dynamic_cast<Genome<CodeAtom>&>(stack);
		//	push(genome);
		//}

		//template <class T>
		//inline void push(Genome<class CodeAtom>& genome)
		//{
		//	if (genome.size() > 0)
		//	{
		//		for (int n = 0; n < genome.size(); n++)
		//		{
		//			T atom = genome[n];

		//			if (get_stack<T>().size() > 0)
		//			{
		//				T top_atom = top<T>();

		//				if ((top_atom.instruction == "EXEC.NOOP")
		//					&& (top_atom.type == Atom::AtomType::ins))
		//				{
		//					atom.close_parentheses += top_atom.close_parentheses;
		//					get_stack<T>().pop();
		//				}
		//			}

		//			get_stack<T>().push(atom);
		//		}
		//	}
		//}





		//template <class T> inline void push<T>(Genome<Atom>& genome) {}
		//template <>	inline void push<CodeAtom>(Genome<Atom>& genome)
		//{
		//	get_stack<CodeAtom>().push(genome);
		//}
		//template <> inline void push<ExecAtom>(Genome<Atom>& genome)
		//{
		//	get_stack<ExecAtom>().push(genome);
		//}

		template <typename T>
		inline void push(T value)
		{
			get_stack<T>().push(value);
		}

		template <class T>
		inline void push_genome(Genome<CodeAtom>& genome)
		{
			get_stack<T>().push_genome(genome);
		}
		
		template <class T>
		inline void push_genome(Genome<ExecAtom>& genome)
		{
			get_stack<T>().push_genome(genome);
		}

		inline void push(CodeAtom genome)
		{
			get_stack<CodeAtom>().push(genome);
		}
		inline void push(ExecAtom genome)
		{
			get_stack<ExecAtom>().push(genome);
		}

		template <typename T>
		inline T pop()
		{
			T val = get_stack<T>().get_top();
			get_stack<T>().pop();
			return val;
		}

		template <class T>
		inline unsigned int pop(Genome<T> &other_stack)
		{
			//Utilities::FixedSizeStack<T>& stack = get_stack<T>();
			//Genome<T>& genome = dynamic_cast<Genome<T>&>(stack);

			//return genome.pop_genome(other_stack);

			return get_stack<T>().pop_genome(other_stack);
		}

		template <typename T>
		inline T get_top()
		{
			T val = get_stack<T>().get_top();
			return val;
		}

		template <class T>
		inline unsigned int get_top(Genome<T> &other_stack)
		{
			//Utilities::FixedSizeStack<T>& stack = get_stack<T>();
			//Genome<T>& genome = dynamic_cast<Genome<T>&>(stack);

			//return genome.top(other_stack);

			return get_stack<T>().get_top(other_stack);
		}

		template <typename T>
		inline T set_top(T val)
		{
			get_stack<T>().get_top_ref() = val;
			return val;
		}

		template <typename T>
		inline bool has_elements(unsigned sz)
		{
			if (get_stack<T>().size() < sz)
				return false;
			else
				return true;
		}

		template <>
		inline bool has_elements<CodeAtom>(unsigned sz)
		{
//			Utilities::FixedSizeStack<CodeAtom>& stack = get_stack<CodeAtom>();
//			Genome<CodeAtom>& genome = dynamic_cast<Genome<CodeAtom>&>(stack);
			Genome<CodeAtom>& genome = get_stack<CodeAtom>();

			if (genome.number_of_blocks() < sz)
				return false;

			else
				return true;
		}

		template <>
		inline bool has_elements<ExecAtom>(unsigned sz)
		{
			//Utilities::FixedSizeStack<ExecAtom>& stack = get_stack<ExecAtom>();
			//Genome<ExecAtom>& genome = dynamic_cast<Genome<ExecAtom>&>(stack);
			Genome<ExecAtom>& genome = get_stack<ExecAtom>();

			if (genome.number_of_blocks() < sz)
				return false;

			else
				return true;
		}

		template <typename T>
		inline T peek_index(unsigned index)
		{
			if (has_elements<T>(index + 1))
				return get_stack<T>().container()[get_stack<T>().size() - index - 1];

			else
				throw;
		}
	};
}
