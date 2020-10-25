#pragma once

#include <vector>
#include <map>
#include <stdexcept>
#include "Genome.h"

namespace Plush
{
	typedef std::map<std::string, unsigned int> Func2BlockWantsMapType;
	extern Func2BlockWantsMapType Func2BlockWantsMap;

	//typedef combinable<Push::Env_detail> Env;

	class Environment
	{
	private:
		// Stacks
		Genome<ExecAtom> exec_stack_;
		Genome<CodeAtom> code_stack_;
		Genome<long> int_stack_;
		Genome<double> double_stack_;
		Genome<bool> bool_stack_;

		std::vector<double> null_input;

	public:
		Environment()
		{
			clear_stacks();
			null_input.clear();
			input = null_input;
			output.clear();
		}

		// Pointer to input & output data
		std::vector<double> input = null_input;
		std::vector<double> output;

		virtual void clear_stacks()
		{
			exec_stack_.clear();
			int_stack_.clear();
			code_stack_.clear();
			bool_stack_.clear();
			double_stack_.clear();
		}

		virtual void initialize(std::vector<double> & _input, unsigned _reserve = 1000)
		{
			clear_stacks();
			input = _input;
			output.clear();
		}

		/* Helper Functions */

		template<typename T>
		unsigned int length()
		{
			Genome<T>& stack = get_stack<T>();
			return stack.size();
		}

		template<>
		unsigned int length<CodeAtom>()
		{
			Genome<CodeAtom>& genome = get_stack<CodeAtom>();
			return genome.number_of_blocks();
		}

		template<>
		unsigned int length<ExecAtom>()
		{
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

		template <typename T> inline void clear()
		{
			get_stack<T>().clear();
		}

		/* pushing and popping */

		template <typename T>
		inline void push(T value)
		{
			get_stack<T>().push(value);
		}

		//template <class T>
		//inline void push(Genome<CodeAtom>& genome)
		//{
		//	get_stack<T>().push_genome(genome);
		//}
		
		//template <class T>
		//inline void push(Genome<ExecAtom>& genome)
		//{
		//	get_stack<T>().push_genome(genome);
		//}

		//inline void push(CodeAtom genome)
		//{
		//	get_stack<CodeAtom>().push(genome);
		//}
		//inline void push(ExecAtom genome)
		//{
		//	get_stack<ExecAtom>().push(genome);
		//}

		//template <class T>
		//inline void push(Genome_section<T> genome_section)
		//{
		//	get_stack<T>().push_genome(genome_section);
		//}

		template <class T>
		inline void push(Genome_section<CodeAtom> genome_section)
		{
			Genome<CodeAtom> code_genome = get_stack<CodeAtom>();

			if (genome_section.size == 0)
			{
				push(T("{:instruction EXEC.NOOP_OPEN_PAREN :close 1}"));
			}
			else
			{
				for (int n = genome_section.ending_position; n >= (int)genome_section.starting_position; n--)
				{
					CodeAtom code_atom = code_genome.get_atom(n);
					T t_atom = T();
					t_atom.set(code_atom.instruction, code_atom.close_parenthesis, code_atom.type);
					push(t_atom);
				}
			}
		}

		template <class T>
		inline void push(Genome_section<ExecAtom> genome_section)
		{
			Genome<ExecAtom> exec_genome = get_stack<ExecAtom>();

			if (genome_section.size == 0)
			{
				push(T("{:instruction EXEC.NOOP_OPEN_PAREN :close 1}"));
			}
			else
			{
				for (int n = genome_section.ending_position; n >= (int)genome_section.starting_position; n--)
				{
					ExecAtom exec_atom = exec_genome.get_atom(n);
					T t_atom = T();
					t_atom.set(exec_atom.instruction, exec_atom.close_parenthesis, exec_atom.type);
					push(t_atom);
				}
			}
		}

		template <typename T>
		inline T pop()
		{
			T val = get_stack<T>().get_top();
			get_stack<T>().pop();
			return val;
		}

		//template <>
		//inline CodeAtom pop<CodeAtom>()
		//{
		//	std::stringstream error_message;
		//	error_message << "reference Environment::pop<CodeAtom>() - Function not defined for this type of stack";

		//	throw std::domain_error(error_message.str());
		//}

		//template <>
		//inline ExecAtom pop<ExecAtom>()
		//{
		//	std::stringstream error_message;
		//	error_message << "reference Environment::pop<ExecAtom>() - Function not defined for this type of stack";

		//	throw std::domain_error(error_message.str());
		//}

		template <class T>
		inline Genome_section<T> pop_genome()
		{
			std::stringstream error_message;
			error_message << "reference Environment::pop_genome() - Function not defined for this type of stack";

			throw std::domain_error(error_message.str());

			return;
		}

		template <>
		inline Genome_section<CodeAtom> pop_genome<CodeAtom>()
		{
			return get_stack<CodeAtom>().pop_genome();
		}

		template <>
		inline Genome_section<ExecAtom> pop_genome<ExecAtom>()
		{
			return get_stack<ExecAtom>().pop_genome();
		}

		template <class T>
		inline Genome_section<T> peek_genome(unsigned int position)
		{
			std::stringstream error_message;
			error_message << "reference Environment::peek_genome() - Function not defined for this type of stack";

			throw std::domain_error(error_message.str());

			return;
		}

		template <>
		inline Genome_section<CodeAtom> peek_genome<CodeAtom>(unsigned int position)
		{
			return get_stack<CodeAtom>()[position];
		}

		template <>
		inline Genome_section<ExecAtom> peek_genome<ExecAtom>(unsigned int position)
		{
			return get_stack<ExecAtom>()[position];
		}

		//template <class T>
		//inline unsigned int pop(Genome<T> &other_stack)
		//{
		//	return get_stack<T>().pop_genome(other_stack);
		//}

		template <typename T>
		inline T get_top()
		{
			T val = get_stack<T>().get_top();
			return val;
		}

		template <class T>
		inline unsigned int get_top(Genome<T> &other_stack)
		{
			return get_stack<T>().get_top(other_stack);
		}

		template <typename T>
		inline T set_top(T val)
		{
			get_stack<T>().get_top() = val;
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
			Genome<CodeAtom>& genome = get_stack<CodeAtom>();

			if (genome.number_of_blocks() < sz)
				return false;

			else
				return true;
		}

		template <>
		inline bool has_elements<ExecAtom>(unsigned sz)
		{
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
				return get_stack<T>()[get_stack<T>().size() - index - 1];

			else
				throw;
		}
	};
}
