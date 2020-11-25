#include "Plush.Instruction.h"
#include "Plush.StaticInit.h"

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

#define MY_GENERIC(name, func)\
		make_instruction((Operator)func<ExecAtom>, "EXEC", #name);\
		make_instruction((Operator)func<long>, "INTEGER", #name);\
		make_instruction((Operator)func<double>, "FLOAT", #name);\
		make_instruction((Operator)func<bool>, "BOOLEAN", #name);\
		make_instruction((Operator)func<CodeAtom>, "CODE", #name);

#define PUSH_GENERIC(name, func)\
		push_make_instruction((Operator)func<ExecAtom>, "EXEC", #name, func##_in<ExecAtom>());\
		push_make_instruction((Operator)func<long>, "INTEGER", #name, func##_in<ExecAtom>());\
		push_make_instruction((Operator)func<double>, "FLOAT", #name, func##_in<ExecAtom>());\
		push_make_instruction((Operator)func<bool>, "BOOLEAN", #name, func##_in<ExecAtom>());\
		push_make_instruction((Operator)func<CodeAtom>, "CODE", #name, func##_in<ExecAtom>());

	template <typename T> Type yankdup_in()
	{
		return make_type<T, int>();
	}

	void initGenerics()
	{
		PUSH_GENERIC(YANKDUP, yankdup);
		MY_GENERIC(DUP, dup);
		MY_GENERIC(=, equals);
		MY_GENERIC(FLUSH, flush);
		MY_GENERIC(POP, protected_pop);
		MY_GENERIC(ROT, rot);
		MY_GENERIC(SHOVE, shove);
		MY_GENERIC(STACKDEPTH, stackdepth);
		MY_GENERIC(SWAP, swap);
		MY_GENERIC(YANK, yank);
	}
}