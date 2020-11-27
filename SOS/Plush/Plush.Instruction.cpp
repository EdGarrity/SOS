#include "Plush.Instruction.h"
#include "Plush.StaticInit.h"
#include "GenericInstructions.h"

namespace Plush
{
//#define FULLGENERIC(name, func)\
//    cons(\
//	    make_instruction(func<ExecAtom>, "EXEC", #name, func##_in<ExecAtom>()),\
//    cons(\
//	    make_instruction(func<int>, "INTEGER", #name, func##_in<int>()),\
//    cons(\
//	    make_instruction(func<double>, "FLOAT", #name, func##_in<double>()),\
//    cons(\
//	    make_instruction(func<bool>, "BOOLEAN", #name, func##_in<bool>()),\
//	    make_instruction(func<CodeAtom>, "CODE", #name, func##_in<CodeAtom>())\
//	))))
//
//#define GENERIC(name,func) FULLGENERIC(name,func);

//	typedef unsigned(*Operator)(Environment&);

//#define MY_GENERIC(name, func, block_wants)\

//#define MY_GENERIC(name, func)\
//		make_instruction((Operator)func<ExecAtom>, "EXEC", #name);\
//		make_instruction((Operator)func<long>, "INTEGER", #name);\
//		make_instruction((Operator)func<double>, "FLOAT", #name);\
//		make_instruction((Operator)func<bool>, "BOOLEAN", #name);\
//		make_instruction((Operator)func<CodeAtom>, "CODE", #name);

#define PUSH_GENERIC(name, func)\
		push_make_instruction((Operator)func<ExecAtom>, "EXEC", #name, func##_in<ExecAtom>());\
		push_make_instruction((Operator)func<long>, "INTEGER", #name, func##_in<long>());\
		push_make_instruction((Operator)func<double>, "FLOAT", #name, func##_in<double>());\
		push_make_instruction((Operator)func<bool>, "BOOLEAN", #name, func##_in<bool>());\
		push_make_instruction((Operator)func<CodeAtom>, "CODE", #name, func##_in<CodeAtom>());

	template <typename T> Type yankdup_in()
	{
		return make_type<T, long>();
	}

	template <typename T> Type dup_in()
	{
		return make_type<T>();
	}

	template <typename T> Type equals_in()
	{
		return make_type<T, T>();
	}

	template <typename T> Type flush_in()
	{
		return nullType;
	}

	template <typename T> Type protected_pop_in()
	{
		return make_type<T>();
	}

	template <typename T> Type rot_in()
	{
		return make_type<T, T, T>();
	}

	template <typename T> Type shove_in()
	{
		return make_type<T, long>();
	}

	template <typename T> Type stackdepth_in()
	{
		return nullType;
	}

	template <typename T> Type swap_in()
	{
		return make_type<T, T>();
	}

	template <typename T> Type yank_in()
	{
		return make_type<T, long>();
	}

	void initGenerics()
	{
		PUSH_GENERIC(YANKDUP, yankdup);
		PUSH_GENERIC(DUP, dup);
		PUSH_GENERIC(=, equals);
		PUSH_GENERIC(FLUSH, flush);
		PUSH_GENERIC(POP, protected_pop);
		PUSH_GENERIC(ROT, rot);
		PUSH_GENERIC(SHOVE, shove);
		PUSH_GENERIC(STACKDEPTH, stackdepth);
		PUSH_GENERIC(SWAP, swap);
		PUSH_GENERIC(YANK, yank);
	}
}