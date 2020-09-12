#pragma once
#include <string>
#include <map>
#include "Processor.h"

namespace Plush
{
	typedef unsigned(*Operator)(Environment &env);

	typedef std::map<std::string, Operator> Func2CodeMapType;
	extern 	Func2CodeMapType Func2CodeMap;

//	typedef std::vector<std::string> Names;
//	extern Names function_names;
//	extern std::vector<std::string> function_names;

	// Declared in Processor.h
	//typedef std::map<std::string, unsigned int> Func2BlockWantsMapType;
	//extern Func2BlockWantsMapType Func2BlockWantsMap;

	class StaticInit
	{
	private:
		std::vector<std::string> function_names;

	public:
		StaticInit();

		/* Registers a function and makes it globally available through the 'instructions' Code */
		void register_pushfunc(Operator op, std::string type, std::string name);
		void set_parentheses(std::string type, std::string name, unsigned int block_wants);
		void set_parentheses(std::string name, unsigned int block_wants);

		unsigned int number_of_functions();
		std::string get_function_name(unsigned int function_index);
	};

	extern StaticInit static_initializer;

	inline void make_instruction(Operator op, std::string type, std::string name)
	{
		static_initializer.register_pushfunc(op, type, name);
	}

	inline void set_parentheses(std::string type, std::string name, unsigned int block_wants)
	{
		static_initializer.set_parentheses(type, name, block_wants);
	}

	inline void set_parentheses(std::string name, unsigned int block_wants)
	{
		static_initializer.set_parentheses(name, block_wants);
	}
}