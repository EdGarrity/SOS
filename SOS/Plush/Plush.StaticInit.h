#pragma once
#include <map>
#include <string>
#include <vector>
#include <thread>
#include <map>
#include "Plush.Instruction.h"

namespace Plush
{
	class Instruction;

	typedef unsigned(*Operator)(Environment& env);

	class StaticInit
	{
	private:
		std::vector<std::string> function_names;

		typedef std::map<std::string, unsigned int> Func2BlockWantsMapType;
		Func2BlockWantsMapType Func2BlockWantsMap;

		typedef std::map<std::string, Instruction*> Func2CodeMapType;
		Func2CodeMapType Func2CodeMap;

	public:
		StaticInit();

		// Registers a function and makes it globally available through the 'instructions' Code 
		void push_register_pushfunc(Instruction* pInstruction);
		void set_parentheses(std::string type, std::string name, unsigned int block_wants);
		//void set_parentheses(std::string name, unsigned int block_wants);

		// Helper functions
		unsigned long number_of_functions();
		std::string get_function_name(unsigned long function_index);
		unsigned long get_function_index(std::string function_name);
		unsigned int get_function_block_wants(std::string function_name);
		Instruction* get_function(std::string function_name);
		Instruction* get_function(unsigned long function_index);
		bool is_function_supported(std::string function_name);
	};

	extern thread_local StaticInit static_initializer;
	//extern StaticInit static_initializer;

	inline void set_parentheses(std::string type, std::string name, unsigned int block_wants)
	{
		static_initializer.set_parentheses(type, name, block_wants);
	}

	//inline void set_parentheses(std::string name, unsigned int block_wants)
	//{
	//	static_initializer.set_parentheses(name, block_wants);
	//}

	void push_make_instruction(Operator op, std::string type, std::string name, Type in, Type out);
}