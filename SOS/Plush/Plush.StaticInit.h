#pragma once
#include <string>
#include <map>
#include "Processor.h"

namespace Plush
{
	typedef unsigned(*Operator)(Environment &env);
	typedef std::map<std::string, Operator> String2CodeMapType;
	extern 	String2CodeMapType String2CodeMap;

	class StaticInit
	{
	public:
		StaticInit();

		/* Registers a function and makes it globally available through the 'instructions' Code */
		void register_pushfunc(Operator op, std::string type, std::string name);
	};

	extern StaticInit static_initializer;

	inline
		void make_instruction(Operator op, std::string type, std::string name)
	{
		static_initializer.register_pushfunc(op, type, name);
	}
}