#include "Plush.Instruction.h"
#include "Plush.StaticInit.h"

namespace Plush
{
	typedef unsigned(*Operator)(Environment&);

#define GENERIC(name, func)\
		make_instruction((Operator)func<ExecAtom>, "EXEC", #name);\
		make_instruction((Operator)func<long>, "INTEGER", #name);\
		make_instruction((Operator)func<double>, "FLOAT", #name);\
		make_instruction((Operator)func<bool>, "BOOLEAN", #name);\
		make_instruction((Operator)func<CodeAtom>, "CODE", #name);

	void initGenerics()
	{
		GENERIC(YANKDUP, yankdup);
		GENERIC(DUP, dup);
		GENERIC(=, equals);
	}

}