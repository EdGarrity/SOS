#pragma once

#include <vector>
#include <map>
#include <set>
#include <stdexcept>
#include "Genome.h"
#include "Type.h"
#include "TypeDef.h"
#include <sstream>

namespace Plush
{
	//typedef std::map<std::string, unsigned int> Func2BlockWantsMapType;
	//extern Func2BlockWantsMapType Func2BlockWantsMap;

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

		// Dynamic Instruction Set
		//typedef std::set<std::string> KnownInstructionsMapType;
		//KnownInstructionsMapType KnownInstructionsMap;

		//typedef std::map<std::string, bool> KnownInstructionsMapType;
		////typedef std::map<int, bool> KnownInstructionsMapType;
		//KnownInstructionsMapType KnownInstructionsMap;

		typedef std::map<std::string, Instruction*> Func2CodeMapType;
		Func2CodeMapType Func2CodeMap;

	public:
		// Temp array of genes.  Used to reverse order of program prior to pushing program onto code stack.
		std::array<std::string, domain::argmap::maximum_stack_size> temp_genes;

		// State of Worker Thread
		enum class RunningState
		{
			Idle = 0,
			Waiting,
			Running
		}; //running_state;

		Environment()
		{
			{
				std::ostringstream ss;
				ss << ",method=Plush_Environment"
					<< ",diagnostic_level=1"
					<< ",message=Constructing";
				Utilities::logline_threadsafe << ss.str();
			}

			clear_stacks();

			if (domain::argmap::static_instruction_set)
			{
				int num_of_instructions = Plush::static_initializer.number_of_functions();

				for (int n = 0; n < num_of_instructions; n++)
					enable_function(n);
			}

			else
			{
				enable_function("EXEC.ENABLE*INSTRUCTION");
				enable_function("EXEC.ENABLE*INSTRUCTIONS");
			}
		}

		// Pointer to input & output data
		size_t record_index;
		//std::vector<double> input = null_input;
		std::vector<double> output;

		// Debug - Rember current state
		std::string current_instruction;
		unsigned long current_effort;
		unsigned long current_unit;
		unsigned long current_thread = 0;
		unsigned long individual_index = 0;
		size_t example_case = 0;

		inline void set_current_thread(int new_current_thread)
		{
			current_thread = new_current_thread;

			exec_stack_.set_current_thread(new_current_thread);
			int_stack_.set_current_thread(new_current_thread);
			code_stack_.set_current_thread(new_current_thread);
			bool_stack_.set_current_thread(new_current_thread);
			double_stack_.set_current_thread(new_current_thread);
		};

		inline void set_current_individual_index(int new_individual_index, size_t new_example_case)
		{
			individual_index = new_individual_index;
			example_case = new_example_case;

			g_individual_index = new_individual_index;
			g_example_case = new_example_case;
		}

		virtual void clear_stacks()
		{
			exec_stack_.clear();
			int_stack_.clear();
			code_stack_.clear();
			bool_stack_.clear();
			double_stack_.clear();

			null_input.clear();
			record_index = 0;
			//input = null_input;
			output.clear();
		}

		virtual void initialize(std::vector<double> & _input)
		{
			clear_stacks();
			record_index = 0;
			//input = _input;
			output.clear();
		}

		virtual void initialize(size_t record_number)
		{
			clear_stacks();
			record_index = record_number;
			//input = null_input;
			output.clear();
		}

		/* Debug functions */
		inline std::string print_state()
		{
			size_t output_stack_count = 0;
			double output_stack_top_value = 0.0;

			if (output.size() > 0)
			{
				output_stack_count = output.size();
				output_stack_top_value = output[0];
			}

			size_t int_stack_count = length<long>();
			long int_stack_top_value = 0;

			if (has_elements<long>(1))
				int_stack_top_value = get_atom_at_position<long>(0);

			size_t double_stack_count = length<long>();
			double double_stack_top_value = 0.0;

			if (has_elements<double>(1))
				double_stack_top_value = get_atom_at_position<double>(0);

			size_t bool_stack_count = length<bool>();
			bool bool_stack_top_value = false;

			if (has_elements<bool>(1))
				bool_stack_top_value = get_atom_at_position<bool>(0);


			std::string debug_msg;

			debug_msg = ",current_instruction=" + current_instruction;
			debug_msg += ",effort=" + std::to_string(current_effort);
			debug_msg += ",unit=" + std::to_string(current_unit);
			//debug_msg += ",current_thread=" + std::to_string(current_thread);


			debug_msg += ",exec_stack_size=" + std::to_string(exec_stack_.size());
			debug_msg += ",code_stack_size=" + std::to_string(code_stack_.size());
			debug_msg += ",int_stack_count=" + std::to_string(int_stack_count);
			debug_msg += ",int_stack_top_value=" + std::to_string(int_stack_top_value);
			debug_msg += ",double_stack_count=" + std::to_string(double_stack_count);
			debug_msg += ",double_stack_top_value=" + std::to_string(double_stack_top_value);
			debug_msg += ",bool_stack_count=" + std::to_string(bool_stack_count);
			debug_msg += ",bool_stack_top_value=" + std::to_string(bool_stack_top_value);

			debug_msg += ",output_stack_count=" + std::to_string(output_stack_count);
			debug_msg += ",output_stack_top_value=" + std::to_string(output_stack_top_value);

			return debug_msg;
		}

#if TRACE_LEVEL>0
		inline void stack_dump(std::string inst_enabled, size_t debug_ip)
		{
			if (individual_index != 1)
				return;

			std::string msg;

			msg = "," + std::to_string(current_thread);
			msg += "," + std::to_string(individual_index);
			msg += "," + std::to_string(example_case);
			msg += "," + std::to_string(debug_ip);
			msg += "," + current_instruction;
			msg += "," + inst_enabled;
			msg += "," + std::to_string(get_stack<ExecAtom>().size());
			msg += "," + get_stack<ExecAtom>().to_string_debug();
			msg += "," + std::to_string(get_stack<CodeAtom>().size());
			msg += "," + get_stack<CodeAtom>().to_string_debug();
			msg += "," + std::to_string(get_stack<long>().size());
			msg += "," + get_stack<long>().to_string_debug();
			msg += "," + std::to_string(get_stack<double>().size());
			msg += "," + get_stack<double>().to_string_debug();
			msg += "," + std::to_string(get_stack<bool>().size());
			msg += "," + get_stack<bool>().to_string_debug();

			msg += ",";

			for (auto it = Func2CodeMap.begin(); it != Func2CodeMap.end(); ++it)
				msg += it->first + " ";

			Utilities::trace_record(msg);
		}

		inline void stack_dump(std::string inst_enabled, std::string instruction, size_t debug_ip)
		{
			if (individual_index != 1)
				return;

			std::string msg;

			msg = "," + std::to_string(current_thread);
			msg += "," + std::to_string(individual_index);
			msg += "," + std::to_string(example_case);
			msg += "," + std::to_string(debug_ip);
			msg += "," + instruction;
			msg += "," + inst_enabled;
			msg += "," + std::to_string(get_stack<ExecAtom>().size());
			msg += "," + get_stack<ExecAtom>().to_string_debug();
			msg += "," + std::to_string(get_stack<CodeAtom>().size());
			msg += "," + get_stack<CodeAtom>().to_string_debug();
			msg += "," + std::to_string(get_stack<long>().size());
			msg += "," + get_stack<long>().to_string_debug();
			msg += "," + std::to_string(get_stack<double>().size());
			msg += "," + get_stack<double>().to_string_debug();
			msg += "," + std::to_string(get_stack<bool>().size());
			msg += "," + get_stack<bool>().to_string_debug();

			msg += ",";

			for (auto it = Func2CodeMap.begin(); it != Func2CodeMap.end(); ++it)
				msg += it->first + " ";

			Utilities::trace_record(msg);
		}
#endif

		/* Helper Functions */

		//void enable_function(std::string function_name)
		//{
		//	//KnownInstructionsMap.insert(function_name);
		//	KnownInstructionsMap[function_name] = true;
		//}

		//void disable_function(std::string function_name)
		//{
		//	//KnownInstructionsMap.erase(function_name);
		//	KnownInstructionsMap[function_name] = false;
		//}

		void enable_function(std::string function_name)
		{
			//KnownInstructionsMap[static_initializer.get_function_index(function_name)] = true;
			//KnownInstructionsMap[function_name] = true;

			Instruction* pInstruction = static_initializer.get_function(function_name);
			Func2CodeMap[function_name] = pInstruction;
		}

		void disable_function(std::string function_name)
		{
			//KnownInstructionsMap[static_initializer.get_function_index(function_name)] = false;
			//KnownInstructionsMap[function_name] = false;

			if (!domain::argmap::static_instruction_set)
				Func2CodeMap.erase(function_name);
		}

		void enable_function(unsigned long function_index)
		{
			//KnownInstructionsMap.insert(static_initializer.get_function_name(function_index));
			//KnownInstructionsMap[static_initializer.get_function_name(function_index)] = true;

			Instruction* pInstruction = static_initializer.get_function(function_index);
			Func2CodeMap[pInstruction->to_string()] = pInstruction;
		}

		void disable_function(int function_index)
		{
			//KnownInstructionsMap.erase(static_initializer.get_function_name(function_index));
			//KnownInstructionsMap[static_initializer.get_function_name(function_index)] = false;

			if (!domain::argmap::static_instruction_set)
			{
				Instruction* pInstruction = static_initializer.get_function(function_index);

				if (pInstruction != nullptr)
				{
					std::string function_name = pInstruction->to_string();
					Func2CodeMap.erase(function_name);
				}
			}
		}

		//bool is_function_enabled(std::string function_name)
		//{
		//	if (function_name == "EXEC.ENABLE*INSTRUCTION")
		//		return true;

		//	else if (function_name == "EXEC.ENABLE*INSTRUCTIONS")
		//		return true;

		//	else
		//	{
		//		//return KnownInstructionsMap.find(function_name) != KnownInstructionsMap.end();
		//		return KnownInstructionsMap[function_name];	// Returns false if function is not in the list.
		//	}
		//}

		Instruction* get_function(std::string function_name)
		{
			auto search = Func2CodeMap.find(function_name);

			if (search != Func2CodeMap.end())
				return Func2CodeMap[function_name];

			else
				return nullptr;
		}

		template<typename T>
		unsigned long length()
		{
			Genome<T>& stack = get_stack<T>();
			return stack.size();
		}

		template<>
		unsigned long length<CodeAtom>()
		{
			Genome<CodeAtom>& genome = get_stack<CodeAtom>();
			return genome.number_of_blocks();
		}

		template<>
		unsigned long length<ExecAtom>()
		{
			Genome<ExecAtom>& genome = get_stack<ExecAtom>();
			return genome.number_of_blocks();
		}

		template<typename T>
		bool assert_length(unsigned long needed)
		{
			Genome<T>& stack = get_stack<T>();
			return stack.size() >= needed;
		}

		template<>
		bool assert_length<CodeAtom>(unsigned long needed)
		{
			Genome<CodeAtom>& genome = get_stack<CodeAtom>();
			//return genome.number_of_blocks() >= needed;

			return genome.number_of_blocks_at_least(needed);
		}

		template<>
		bool assert_length<ExecAtom>(unsigned long needed)
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
			return (get_stack<T>().is_empty());
		}

		template <typename T> inline void clear()
		{
			get_stack<T>().clear();
		}

		/* pushing and popping */

		template <typename T>
		inline unsigned long push(T value)
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
				unsigned long starting_index = code_stack.position_to_index(genome_section.ending_position);
				unsigned long ending_index = code_stack.position_to_index(genome_section.starting_position) + 1;

				for (unsigned long n = starting_index; n < ending_index; n++)
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
				unsigned long starting_index = stack.position_to_index(genome_section.ending_position);
				unsigned long ending_index = stack.position_to_index(genome_section.starting_position) + 1;

				for (unsigned long n = starting_index; n < ending_index; n++)
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
		inline Genome_section<T> peek_genome(unsigned long position)
		{
			std::stringstream error_message;
			error_message << "reference Environment::peek_genome() - Function not defined for this type of stack";

			throw std::domain_error(error_message.str());

			return;
		}

		template <>
		inline Genome_section<CodeAtom> peek_genome<CodeAtom>(unsigned long position)
		{
			return get_stack<CodeAtom>()[position];
		}

		template <>
		inline Genome_section<ExecAtom> peek_genome<ExecAtom>(unsigned long position)
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
		inline unsigned long get_stack_size(unsigned int which) //const
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
		inline bool check_stack_size_at_least(unsigned long which, unsigned long size_needed) //const
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
		inline unsigned long get_stack_free(unsigned long which) //const
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
		inline unsigned long position_to_index(unsigned long position)
		{
			return get_stack<T>().position_to_index(position);
		}
	};
}
