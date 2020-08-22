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
		Genome<class ExecAtom> exec_stack_;
		Genome<class CodeAtom> code_stack_;
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

		template<typename T>
		unsigned int length()
		{
			Utilities::FixedSizeStack<T>& stack = get_stack<T>();
			return stack.size();
		}

		template<>
		unsigned int length<CodeAtom>()
		{
			Utilities::FixedSizeStack<CodeAtom>& stack = get_stack<CodeAtom>();
			Genome<CodeAtom>& genome = dynamic_cast<Genome<CodeAtom>&>(stack);

			return genome.number_of_blocks();
		}

		template<>
		unsigned int length<ExecAtom>()
		{
			Utilities::FixedSizeStack<ExecAtom>& stack = get_stack<ExecAtom>();
			Genome<ExecAtom>& genome = dynamic_cast<Genome<ExecAtom>&>(stack);

			return genome.number_of_blocks();
		}

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

		template <typename T>
		inline void push(T value)
		{
			get_stack<T>().push(value);
		}

		inline void push(Genome<class CodeAtom>& genome)
		{
			//if (stack.size() > 0)
			//	for (int n = 0; n < stack.size(); n++)
			//		get_stack<CodeAtom>().push(stack[n]);

			if (genome.size() > 0)
			{
				for (int n = 0; n < genome.size(); n++)
				{
					Atom atom = genome[n];

					if (get_stack<CodeAtom>().size() > 0)
					{
						Atom top_atom = top<CodeAtom>();

						if ((top_atom.instruction == "EXEC.NOOP")
							&& (top_atom.type == Atom::AtomType::ins))
						{
							atom.close_parentheses += top_atom.close_parentheses;
							get_stack<CodeAtom>().pop();
						}
					}

					get_stack<CodeAtom>().push(atom);
				}
			}
		}

		inline void push(Genome<class ExecAtom>& genome)
		{
			//if (stack.size() > 0)
			//	for (int n = 0; n < stack.size(); n++)
			//		get_stack<ExecAtom>().push(stack[n]);
			if (genome.size() > 0)
			{
				for (int n = 0; n < genome.size(); n++)
				{
					Atom atom = genome[n];

					if (get_stack<ExecAtom>().size() > 0)
					{
						Atom top_atom = top<ExecAtom>();

						if ((top_atom.instruction == "EXEC.NOOP")
							&& (top_atom.type == Atom::AtomType::ins))
						{
							atom.close_parentheses += top_atom.close_parentheses;
							get_stack<ExecAtom>().pop();
						}
					}

					get_stack<ExecAtom>().push(atom);
				}
			}
		}

		template <class T>
		inline void push(Utilities::FixedSizeStack<Atom> &stack)
		{
			if (stack.size() > 0)
				for (int n = 0; n < stack.size(); n++)
					get_stack<T>().push(stack[n]);
		}

		template <>
		inline void push<CodeAtom>(Utilities::FixedSizeStack<Atom> &stack)
		{
			Genome<CodeAtom>& genome = dynamic_cast<Genome<CodeAtom>&>(stack);
			push(genome);

			//if (genome.size() > 0)
			//{
			//	for (int n = 0; n < genome.size(); n++)
			//	{
			//		Atom atom = genome[n];

			//		if (get_stack<CodeAtom>().size() > 0)
			//		{
			//			Atom top_atom = top<CodeAtom>();

			//			if ((top_atom.instruction == "EXEC.NOOP")
			//				&& (top_atom.type == Atom::AtomType::ins))
			//			{
			//				atom.close_parentheses += top_atom.close_parentheses;
			//				get_stack<CodeAtom>().pop();
			//			}
			//		}

			//		get_stack<CodeAtom>().push(atom);
			//	}
			//}
		}

		template <>
		inline void push<ExecAtom>(Utilities::FixedSizeStack<Atom> &stack)
		{
			Genome<CodeAtom>& genome = dynamic_cast<Genome<CodeAtom>&>(stack);
			push(genome);

			//if (genome.size() > 0)
			//{
			//	for (int n = 0; n < genome.size(); n++)
			//	{
			//		Atom atom = genome[n];

			//		if (get_stack<ExecAtom>().size() > 0)
			//		{
			//			Atom top_atom = top<ExecAtom>();

			//			if ((top_atom.instruction == "EXEC.NOOP")
			//				&& (top_atom.type == Atom::AtomType::ins))
			//			{
			//				atom.close_parentheses += top_atom.close_parentheses;
			//				get_stack<ExecAtom>().pop();
			//			}
			//		}

			//		get_stack<ExecAtom>().push(atom);
			//	}
			//}
		}

		template <typename T>
		inline T pop()
		{
			T val = get_stack<T>().top();
			get_stack<T>().pop();
			return val;
		}

		template <class T>
		inline unsigned int pop(Genome<Atom> &other_stack)
		{
			Utilities::FixedSizeStack<T>& stack = get_stack<T>();
			Genome<T>& genome = dynamic_cast<Genome<T>&>(stack);

			return genome.pop_genome(other_stack);
		}

		template <class T>
		inline unsigned int pop(Genome<T> &other_stack)
		{
			Utilities::FixedSizeStack<T>& stack = get_stack<T>();
			Genome<T>& genome = dynamic_cast<Genome<T>&>(stack);
//
////			Utilities::FixedSizeStack<T>& stack2 = other_stack.get_stack();
//			Genome<Atom>& other_genome = dynamic_cast<Genome<Atom>&>(other_stack);

//			Genome<Atom>& other_genome = other_stack;

//			return genome.pop_genome(other_stack);
//			return genome.pop_genome(genome);

			return genome.pop_genome(genome.get_base(other_stack));
		}

		template <typename T>
		inline T top()
		{
			T val = get_stack<T>().top();
			return val;
		}

		template <class T>
		inline unsigned int top(Genome<class Atom> &other_stack)
		{
			Utilities::FixedSizeStack<T>& stack = get_stack<T>();
			Genome<T>& genome = dynamic_cast<Genome<T>&>(stack);

			return genome.top(other_stack);
		}

		template <typename T>
		inline T top(T val)
		{
			get_stack<T>().top() = val;
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
			Utilities::FixedSizeStack<CodeAtom>& stack = get_stack<CodeAtom>();
			Genome<CodeAtom>& genome = dynamic_cast<Genome<CodeAtom>&>(stack);

			if (genome.number_of_blocks() < sz)
				return false;

			else
				return true;
		}

		template <>
		inline bool has_elements<ExecAtom>(unsigned sz)
		{
			Utilities::FixedSizeStack<ExecAtom>& stack = get_stack<ExecAtom>();
			Genome<ExecAtom>& genome = dynamic_cast<Genome<ExecAtom>&>(stack);

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
