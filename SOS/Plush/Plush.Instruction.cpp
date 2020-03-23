#include "Plush.Instruction.h"
#include "Plush.StaticInit.h"

namespace Plush
{
//	typedef unsigned(*Operator)(Environment&);

#define GENERIC(name, func, block_wants)\
		make_instruction((Operator)func<ExecAtom>, "EXEC", #name, block_wants);\
		make_instruction((Operator)func<long>, "INTEGER", #name, block_wants);\
		make_instruction((Operator)func<double>, "FLOAT", #name, block_wants);\
		make_instruction((Operator)func<bool>, "BOOLEAN", #name, block_wants);\
		make_instruction((Operator)func<CodeAtom>, "CODE", #name, block_wants);

	void initGenerics()
	{
		GENERIC(YANKDUP, yankdup, 0);
		GENERIC(DUP, dup, 1);
		GENERIC(=, equals, 0);
	}

}