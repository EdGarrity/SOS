#pragma once

#include <vector>
#include "Type.h"

namespace Plush
{
	typedef unsigned(*Operator)(Environment&);

	class Instruction //: public Atom
	{
		const Operator op_; // const, so the compiler might be able to inline the call
		std::string name_;
		Type intype_;
		Type outtype_;

	public:
		Instruction(Operator op, std::string name, Type intype, Type outtype) : op_(op), name_(name), intype_(intype), outtype_(outtype) {}

		virtual bool can_run(/*const*/ Environment& env) /*const*/ 
		{ 
			return intype_.can_pop_from(env) && outtype_.can_push_to(env);
		}

		std::string to_string() const { return name_; }
		Operator get_op() const { return op_; }
	};

	void initGenerics();
}