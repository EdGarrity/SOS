#pragma once

#include "StaticInit.h"
#include "TypedAtom.h"

namespace push
{
	class Env;

	class Instruction : public TypedAtom
	{
		const Operator op_; // const, so the compiler might be able to inline the call
		std::string name_;

		Type intype_;
		Type outtype_;

		bool static_typed_;

	public:
		Instruction(Operator op, std::string name) : op_(op), name_(name) {}
		Instruction(Operator op, std::string name, Type intype, Type outtype, bool static_ = true) :
			op_(op), name_(name), intype_(intype), outtype_(outtype), static_typed_(static_) {}

		Operator get_op() const
		{
			return op_;
		}

		const Type &get_precondition() const
		{
			return intype_;
		}
		const Type &get_postcondition() const
		{
			return outtype_;
		}
		bool is_static() const
		{
			return static_typed_;
		}

		std::string to_string() const
		{
			return name_;
		}

		std::string to_code(const std::string& _name) const
		{
			return name_;
		}

		virtual bool can_run() const
		{
			return intype_.can_pop_from();
		}

		virtual unsigned operator()() const
		{
			if (can_run()) 
				op_(env); // Remove env?

			return 1;
		} // currently ignores 'effort'

	protected:
		bool equal_to(const CodeBase &other) const
		{
			return op_ == static_cast<const Instruction &>(other).op_;
		}
	};

	inline
		Code make_instruction(Operator op, std::string type, std::string name)
	{
		return static_initializer.register_pushfunc(Code(new Instruction(op, type + "." + name, Type(), Type(), true)));
	}

	inline
		Code make_instruction(Operator op, std::string type_name)
	{
		return static_initializer.register_pushfunc(Code(new Instruction(op, type_name, Type(), Type(), true)));
	}

	inline
		Code make_instruction(Operator op, std::string type, std::string name, Type in, Type out, bool static_ = true)
	{
		//    std::cout << "Instruction " << type << "." << name << " has type " << in << " -> " << out << std::endl;
		
		
		//return static_initializer.register_pushfunc(Code(new Instruction(op, type + "." + name, in, out, static_)));
		Instruction* ins = new Instruction(op, type + "." + name, in, out, static_);
		Code code1 = Code(ins);
		Code code2 = static_initializer.register_pushfunc(code1);
		return code2;
	}

	inline
		Code make_instruction(Operator op, std::string type_name, Type intype, Type outtype, bool static_ = true)
	{
		//    std::cout << "Instruction " << type_name << " has type " << intype << " -> " << outtype << std::endl;
		return static_initializer.register_pushfunc(Code(new Instruction(op, type_name, intype, outtype, static_)));
	}

	inline
		Code make_instruction(Operator op, const char* type_name, Type intype, Type outtype, bool static_ = true)
	{
		return make_instruction(op, std::string(type_name), intype, outtype, static_);
	}

	//class InstructionFactory
	//{
	//	std::forward_list <Instruction*> _list;

	//public:
	//	Instruction* newInstruction(Operator op, std::string name);
	//	Instruction* newInstruction(Operator op, std::string name, Type intype, Type outtype, bool static_ = true);
	//};
}