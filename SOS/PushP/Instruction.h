#pragma once

#include "StaticInit.h"
#include "TypedAtom.h"

namespace Push
{
	class Env;

	unsigned int lookup_instruction_paren_groups(std::string name);
	void set_parentheses(std::string name, unsigned int _parentheses);

	class Instruction : public TypedAtom
	{
		const Operator op_; // const, so the compiler might be able to inline the call
		std::string name_;

		Type intype_;
		Type outtype_;

		unsigned int parentheses_;

	public:
		Instruction(Operator op, std::string name) : op_(op), name_(name) {}
		Instruction(Operator op, std::string name, Type intype, Type outtype, unsigned int parentheses = 0) :
			op_(op), name_(name), intype_(intype), outtype_(outtype), parentheses_(parentheses) {}

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

		virtual unsigned int instruction_paren_groups() const
		{
			return parentheses_;
		}

		virtual void set_parentheses(unsigned int _parentheses)
		{
			parentheses_ = _parentheses;
		}

		TYPE_ID get_type2()
		{
			return TYPE_ID::ins;
		}

	protected:
		bool equal_to(const CodeBase &other) const
		{
			return op_ == static_cast<const Instruction &>(other).op_;
		}
	};

	inline
		Code make_instruction(Operator op, std::string type, std::string name)
	{
		return static_initializer.register_pushfunc(Code(new Instruction(op, type + "." + name, Type(), Type())));
	}

	inline
		Code make_instruction(Operator op, std::string type_name)
	{
		return static_initializer.register_pushfunc(Code(new Instruction(op, type_name, Type(), Type())));
	}

	inline
		Code make_instruction(Operator op, std::string type, std::string name, Type in, Type out, unsigned int parentheses = 0)
	{
		//    std::cout << "Instruction " << type << "." << name << " has type " << in << " -> " << out << std::endl;
		
		
		//return static_initializer.register_pushfunc(Code(new Instruction(op, type + "." + name, in, out, static_)));
		Instruction* ins = new Instruction(op, type + "." + name, in, out, parentheses);
		Code code1 = Code(ins);
		Code code2 = static_initializer.register_pushfunc(code1);
		return code2;
	}

	inline
		Code make_instruction(Operator op, std::string type_name, Type intype, Type outtype, unsigned int parentheses = 0)
	{
		//    std::cout << "Instruction " << type_name << " has type " << intype << " -> " << outtype << std::endl;
		return static_initializer.register_pushfunc(Code(new Instruction(op, type_name, intype, outtype, parentheses)));
	}

	inline
		Code make_instruction(Operator op, const char* type_name, Type intype, Type outtype, unsigned int parentheses)
	{
		return make_instruction(op, std::string(type_name), intype, outtype, parentheses);
	}

	//class InstructionFactory
	//{
	//	std::forward_list <Instruction*> _list;

	//public:
	//	Instruction* newInstruction(Operator op, std::string name);
	//	Instruction* newInstruction(Operator op, std::string name, Type intype, Type outtype, bool static_ = true);
	//};
}