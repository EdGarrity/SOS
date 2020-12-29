#pragma once

#include <vector>
#include <map>
#include <stdexcept>
#include "Genome.h"
#include "Type.h"
#include "TypeDef.h"

namespace Plush
{
	typedef std::map<std::string, unsigned int> Func2BlockWantsMapType;
	extern Func2BlockWantsMapType Func2BlockWantsMap;

	extern std::vector<double> null_input;

	class Environment
	{
	private:
		// Stacks
		Genome<ExecAtom> exec_stack_;
		Genome<CodeAtom> code_stack_;
		Genome<long> int_stack_;
		Genome<double> double_stack_;
		Genome<bool> bool_stack_;


	public:
		// State of Worker Thread
		enum RunningState
		{
			Idle = 0,
			Waiting,
			Running
		} running_state;

		Environment()
		{
			clear_stacks();
			null_input.clear();
			input = null_input;
			output.clear();

			running_state = Idle;
		}

		// Pointer to input & output data
		std::vector<double> input = null_input;
		std::vector<double> output;

		// Debug - Rember current state
		std::string current_instruction;
		size_t current_effort;
		size_t current_unit;
		int current_thread;

		virtual void clear_stacks()
		{
			exec_stack_.clear();
			int_stack_.clear();
			code_stack_.clear();
			bool_stack_.clear();
			double_stack_.clear();

			//current_instruction.clear();
			//current_effort = 0;
			//current_unit = 0;
			//current_thread = -99;
		}

		virtual void initialize(std::vector<double> & _input)
		{
			clear_stacks();
			input = _input;
			output.clear();
		}

		/* Debug functions */
		inline std::string print_state()
		{
			std::string debug_msg;

			debug_msg = "Instruction=" + current_instruction;
			debug_msg += ",effort=" + std::to_string(current_effort);
			debug_msg += ",unit=" + std::to_string(current_unit);
			debug_msg += ",exec_stack_size=" + std::to_string(exec_stack_.size());
			debug_msg += ",code_stack_=" + std::to_string(code_stack_.size());
			debug_msg += ",int_stack_=" + std::to_string(int_stack_.size());
			debug_msg += ",double_stack_=" + std::to_string(double_stack_.size());
			debug_msg += ",bool_stack_=" + std::to_string(bool_stack_.size());

			return debug_msg;
		}

		/* Helper Functions */

		template<typename T>
		size_t length()
		{
			Genome<T>& stack = get_stack<T>();
			return stack.size();
		}

		template<>
		size_t length<CodeAtom>()
		{
			Genome<CodeAtom>& genome = get_stack<CodeAtom>();
			return genome.number_of_blocks();
		}

		template<>
		size_t length<ExecAtom>()
		{
			Genome<ExecAtom>& genome = get_stack<ExecAtom>();
			return genome.number_of_blocks();
		}

		template<typename T>
		bool assert_length(unsigned int needed)
		{
			Genome<T>& stack = get_stack<T>();
			return stack.size() >= needed;
		}

		template<>
		bool assert_length<CodeAtom>(unsigned int needed)
		{
			Genome<CodeAtom>& genome = get_stack<CodeAtom>();
			//return genome.number_of_blocks() >= needed;

			return genome.number_of_blocks_at_least(needed);
		}

		template<>
		bool assert_length<ExecAtom>(unsigned int needed)
		{
			Genome<ExecAtom>& genome = get_stack<ExecAtom>();
			//return genome.number_of_blocks() >= needed;

			return genome.number_of_blocks_at_least(needed);
		}

		template <typename T> inline Genome<T>& get_stack(){}
		template <> inline Genome<ExecAtom>& get_stack()
		{
			return exec_stack_;
		}
		template <> inline Genome<CodeAtom>& get_stack()
		{
			return code_stack_;
		}
		template <> inline Genome<long>& get_stack()
		{
			return int_stack_;
		}
		template <> inline Genome<double>& get_stack()
		{
			return double_stack_;
		}
		template <> inline Genome<bool>& get_stack()
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
		inline unsigned int push(T value)
		{
			get_stack<T>().push(value);
			return 1;
		}

		template <class T>
		inline void push(Genome_section<CodeAtom> genome_section)
		{
			Genome<CodeAtom>& code_stack = get_stack<CodeAtom>();

			if (genome_section.size == 0)
				push(T("{:instruction EXEC.NOOP_OPEN_PAREN :close 1}"));

			else
			{
				size_t starting_index = code_stack.position_to_index(genome_section.ending_position);
				size_t ending_index = code_stack.position_to_index(genome_section.starting_position) + 1;

				for (size_t n = starting_index; n < ending_index; n++)
					push(T(code_stack.get_atom_at_index(n)));
			}
		}

		template <class T>
		inline void push(Genome_section<ExecAtom> genome_section)
		{
			Genome<ExecAtom>& stack = get_stack<ExecAtom>();

			if (genome_section.size == 0)
				push(T("{:instruction EXEC.NOOP_OPEN_PAREN :close 1}"));

			else
			{
				size_t starting_index = stack.position_to_index(genome_section.ending_position);
				size_t ending_index = stack.position_to_index(genome_section.starting_position) + 1;

				for (size_t n = starting_index; n < ending_index; n++)
					push(T(stack.get_atom_at_index(n)));
			}
		}

		template <typename T>
		inline T pop()
		{
			T val = get_stack<T>().get_top_atom();
			get_stack<T>().pop();
			return val;
		}

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

		template <typename T>
		inline T get_top_atom()
		{
			T val = get_stack<T>().get_top_atom();
			return val;
		}

		template <typename T>
		inline T set_top(T val)
		{
			get_stack<T>().get_top_atom() = val;
			return val;
		}

		template <typename T>
		inline bool has_elements(size_t sz)
		{
			if (get_stack<T>().size() < sz)
				return false;
			else
				return true;
		}

		template <typename T>
		inline T& get_atom_at_position(size_t position)
		{
			if (has_elements<T>(position + 1))
				return get_stack<T>().get_atom_at_position(position);

			else
			{
				std::stringstream error_message;
				error_message << "Environment::get_atom() - Index out of range.  position = " << position;

				throw std::out_of_range(error_message.str());
			}
		}

		virtual Type make_type() const;

		/* Needed for type checking of preconditions */
		inline size_t get_stack_size(unsigned int which) //const
		{
			switch (which)
			{
			case EXEC_STACK: return length<ExecAtom>();
			case INTEGER_STACK: return int_stack_.size();
			case CODE_STACK: return length<CodeAtom>();
			case BOOL_STACK: return bool_stack_.size();
			case FLOAT_STACK: return double_stack_.size();
			}
			return 0;
		}

		/* Needed for type checking of preconditions */
		inline bool check_stack_size_at_least(unsigned int which, unsigned int size_needed) //const
		{
			switch (which)
			{
			case EXEC_STACK: return assert_length<ExecAtom>(size_needed);
			case INTEGER_STACK: return int_stack_.size() >= size_needed;
			case CODE_STACK: return assert_length<CodeAtom>(size_needed);
			case BOOL_STACK: return bool_stack_.size() >= size_needed;
			case FLOAT_STACK: return double_stack_.size() >= size_needed;
			}
			return false;
		}

		/* Needed for type checking of post conditions */
		inline size_t get_stack_free(unsigned int which) //const
		{
			switch (which)
			{
			case EXEC_STACK: return exec_stack_.free();
			case INTEGER_STACK: return int_stack_.free();
			case CODE_STACK: return code_stack_.free();
			case BOOL_STACK: return bool_stack_.free();
			case FLOAT_STACK: return double_stack_.free();
			}
			return 0;
		}

		template <typename T>
		inline size_t position_to_index(size_t position)
		{
			return get_stack<T>().position_to_index(position);
		}
	};
}
