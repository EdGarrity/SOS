#include "Plush.CodeInstructions.h"
#include "Processor.h"
#include "Plush.StaticInit.h"

namespace Plush
{
	//unsigned code_append(Environment & _env)
	//{
	//	if (_env.has_elements<CodeAtom>(2))
	//	{
	//		Utilities::FixedSizeStack<ExecAtom> block_a;
	//		Utilities::FixedSizeStack<ExecAtom> block_b;

	//		bool s = _env.pop<bool>();
	//		int unmatched_a = _env.pop<ExecAtom>(block_a);
	//		int unmatched_b = _env.pop<ExecAtom>(block_b);

	//		if ((block_a.size() > 0) && (block_b.size() > 0))
	//		{
	//			// Remove one closing paranthesis from block A before pushing the blocks back on stack
	//			Atom atom = block_a.top();
	//			block_a.pop();

	//			atom.close_parentheses = (atom.close_parentheses > 0) ? atom.close_parentheses - 1 : atom.close_parentheses;
	//			block_a.push(atom);

	//			_env.push<ExecAtom>(block_a);
	//			_env.push<ExecAtom>(block_b);
	//		}

	//		else
	//		{
	//			if (block_a.size() > 0)
	//				_env.push<ExecAtom>(block_a);

	//			if (block_b.size() > 0)
	//				_env.push<ExecAtom>(block_b);
	//		}
	//	}

	//	return 1;
	//}

	void initCodeA()
	{
		static bool initialized = false;

		if (initialized) return;

		initialized = true;
		//make_instruction((Operator)_cons, "CODE", "CONS", 0);

		//make_instruction((Operator)_cons, "CODE.CONS", binaryCode, codeType);
		//make_instruction((Operator)_list, "CODE.LIST", binaryCode, codeType);
		//make_instruction((Operator)_do, "CODE.DO", codeType, execType);
		//make_instruction((Operator)_do_star, "CODE.DO*", codeType, execType);
		//make_instruction((Operator)_quote, "CODE.QUOTE", execType, codeType, 1);
		//make_instruction((Operator)_if, "CODE.IF", boolType + binaryCode, execType);
		//make_instruction((Operator)_length, "CODE.LENGTH", codeType, integerType);
		//make_instruction((Operator)_size, "CODE.SIZE", codeType, integerType);
		//make_instruction((Operator)_from_T<int>, "CODE.FROMINTEGER", integerType, codeType);
		//make_instruction((Operator)_from_T<double>, "CODE.FROMFLOAT", floatType, codeType);
		//make_instruction((Operator)_from_T<bool>, "CODE.FROMBOOLEAN", boolType, codeType);
		//make_instruction((Operator)_instructions, "CODE.INSTRUCTIONS", nullType, codeType);
		//make_instruction((Operator)_append, "CODE.APPEND", binaryCode, codeType);
		//make_instruction((Operator)_atom, "CODE.ATOM", codeType, boolType);
		//make_instruction((Operator)_car, "CODE.CAR", codeType, codeType);
		//make_instruction((Operator)_cdr, "CODE.CDR", codeType, codeType);
		//make_instruction((Operator)_container, "CODE.CONTAINER", binaryCode, codeType);
		//make_instruction((Operator)_extract, "CODE.EXTRACT", codeType + integerType, codeType);
		//make_instruction((Operator)_insert, "CODE.INSERT", binaryCode + integerType, codeType);
		//make_instruction((Operator)_member, "CODE.MEMBER", binaryCode, boolType);
		//make_instruction((Operator)_noop, "CODE.NOOP", nullType, nullType);
		//make_instruction((Operator)_nth, "CODE.NTH", integerType + codeType, codeType);
		//make_instruction((Operator)_nthcdr, "CODE.NTHCDR", integerType + codeType, codeType);
		//make_instruction((Operator)_null, "CODE.NULL", codeType, boolType);
		//make_instruction((Operator)_position, "CODE.POSITION", binaryCode, integerType);
		//make_instruction((Operator)code_do_range, "CODE.DO*RANGE", integerType + integerType + codeType, execType + execType);
		//make_instruction((Operator)do_count, "CODE.DO*COUNT", integerType + codeType, execType);
		//make_instruction((Operator)do_times, "CODE.DO*TIMES", integerType + codeType, execType);
		//make_instruction((Operator)in2code, "CODE.IN", integerType, codeType);
		//make_instruction((Operator)inall2code, "CODE.INALL", nullType, codeType);
		//make_instruction((Operator)inallrev2code, "CODE.INALLREV", nullType, codeType);

//		make_instruction((Operator)code_append, "CODE", "APPEND", 0);
	}
}
