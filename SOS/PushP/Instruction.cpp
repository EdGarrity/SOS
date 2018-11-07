#include "Instruction.h"
#include "Env.h"
#include "RNG.h"
#include "CodeUtils.h"
#include "Literal.h"

#define FULLGENERIC(name, func, parentheses)\
	cons(\
		make_instruction((Operator)func<Exec>, "EXEC", #name, func##_in<Exec>(), func##_out<Exec>(), parentheses),\
	cons(\
		make_instruction((Operator)func<int>, "INTEGER", #name, func##_in<int>(), func##_out<int>(), parentheses),\
	cons(\
		make_instruction((Operator)func<double>, "FLOAT", #name, func##_in<double>(), func##_out<double>(), parentheses),\
	cons(\
		make_instruction((Operator)func<bool>, "BOOLEAN", #name, func##_in<bool>(), func##_out<bool>(), parentheses),\
		make_instruction((Operator)func<Code>, "CODE", #name, func##_in<Code>(), func##_out<Code>(), parentheses)\
	))))


/* #define FULLGENERIC(name, func, static_)\
	cons(\
		make_instruction((Operator)func<bool>, "BOOLEAN", #name, func##_in<bool>(), func##_out<bool>(), static_),\
		make_instruction((Operator)func<Code>, "CODE", #name, func##_in<Code>(), func##_out<Code>(), static_)\
	)
*/

#define GENERIC(name,func) FULLGENERIC(name,func,0);

// Initialization of global Random Number Generator
RNG rng(Push::global_parameters.random_seed);

// definition of the full push language
namespace Push
{
	//Instruction* InstructionFactory::newInstruction(Operator op, std::string name)
	//{
	//	Instruction* p = new Instruction(op, name);
	//	_list.emplace_front(p);
	//	return p;
	//}

	//Instruction * InstructionFactory::newInstruction(Operator op, std::string name, Type intype, Type outtype, bool static_)
	//{
	//	Instruction* p = new Instruction(op, name, intype, outtype, static_);
	//	_list.emplace_front(p);
	//	return p;
	//}

	//inline
	//	CodeBase make_instruction(Operator op, std::string type, std::string name)
	//{
	//	return static_initializer.register_pushfunc(*new Instruction(op, type + "." + name, Type(), Type(), true));
	//}

	thread_local InstructionFactory instructionFactory;

	template <typename T> Type yankdup_in()
	{
		return make_type<T, int>();
	}
	template <typename T> Type yankdup_out()
	{
		return make_type<T>();
	}


	template <typename T> Type dup_in()
	{
		return make_type<T>();
	}
	template <typename T> Type dup_out()
	{
		return make_type<T, T>();
	}


	template <typename T> Type equals_in()
	{
		return make_type<T, T>();
	}
	template <typename T> Type equals_out()
	{
		return boolType;
	}


	template <typename T> Type swap_in()
	{
		return make_type<T, T>();
	}
	template <typename T> Type swap_out()
	{
		return make_type<T, T>();
	}


	template <typename T> Type protected_pop_in()
	{
		return make_type<T>();
	}
	template <typename T> Type protected_pop_out()
	{
		return nullType;
	}




	template <typename T> Type shove_in()
	{
		return make_type<T, int>();
	}
	template <typename T> Type shove_out()
	{
		return make_type<T>();
	}


	template <typename T> Type stackdepth_in()
	{
		return nullType;
	}
	template <typename T> Type stackdepth_out()
	{
		return integerType;
	}


	template <typename T> Type yank_in()
	{
		return make_type<T, int>();
	}
	template <typename T> Type yank_out()
	{
		return make_type<T>();
	}


	template <typename T> Type rot_in()
	{
		return make_type<T, T, T>();
	}
	template <typename T> Type rot_out()
	{
		return make_type<T, T, T>();
	}


	template <typename T> Type flush_in()
	{
		return nullType;
	}
	template <typename T> Type flush_out()
	{
		return nullType;
	}

	template <class T>
	unsigned flush(Env &env)
	{
		get_stack<T>().clear();
		return 1;
	}

	void initGenerics()
	{
		static bool initialized = false;

		if (initialized) return;

		initialized = true;

		GENERIC(YANKDUP, yankdup);
		GENERIC(DUP, dup);
		GENERIC(=, equals);
		GENERIC(SWAP, swap);
		GENERIC(POP, protected_pop);
//		GENERIC(DEFINE, name_define);
		GENERIC(SHOVE, shove);
		GENERIC(STACKDEPTH, stackdepth);
		GENERIC(YANK, yank);
		GENERIC(ROT, rot);
		GENERIC(FLUSH, flush);
	}

	unsigned int lookup_instruction_paren_groups(std::string name)
	{
		if (name == "NOOP_OPEN_PAREN")
			return 1;

		else
			return (*detail::str2parentheses_map_ptr)[name];
	}

	void set_parentheses(std::string name, unsigned int _parentheses)
	{
		(*detail::str2parentheses_map_ptr)[name] = _parentheses;

		Code code = (*detail::str2code_map_ptr)[name];

		code->set_parentheses(_parentheses);
	}

}