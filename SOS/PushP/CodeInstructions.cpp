#include "Instruction.h"
#include "Env.h"

#include "CodeOps.h"
#include "Literal.h"
#include "RandOps.h"

namespace Push
{
	unsigned code_do_range(Env & _env)
	{
		int n = pop<int>(_env);
		int i = pop<int>(_env);
		Code code = pop<Code>(_env);
		int direction = 1;

		if (i > n) direction = -1;

		push(_env, i);

		if (i != n)
		{
			i += direction;
			CodeArray vec(5);
			vec[0] = MyDoRange;
			vec[1] = code;
			vec[2] = quote;
			vec[3] = Code(parallel_intLiteralFactory.local().createLiteral(n));
			vec[4] = Code(parallel_intLiteralFactory.local().createLiteral(i));
			_env.local().push_code_to_exec_stack(Code(parallel_codeListFactory.local().createCodeList(vec)));  // new CodeList(vec)));  //CodeList::adopt(vec)); //env.exec_stack.push_back(*new CodeList(vec));  //env.push_guarded(CodeList::adopt(vec)); //Code(new CodeList(vec)));
		}

		_env.local().push_code_to_exec_stack(code);

		return 1;
	}

	unsigned do_count(Env & _env)
	{
		int n = pop<int>(_env);
		Code code = pop<Code>(_env);

		if (n <= 0)
			return 1;

		CodeArray vec(5);
		vec[0] = MyDoRange;
		vec[1] = quote;
		vec[2] = code;
		vec[3] = Code(parallel_intLiteralFactory.local().createLiteral(n - 1));
		vec[4] = zero;
		_env.local().push_code_to_exec_stack(Code(parallel_codeListFactory.local().createCodeList(vec)));  // new CodeList(vec)));  //CodeList::adopt(vec)); //Code(new CodeList(vec)));

		return 1;
	}

	unsigned do_times(Env & _env)
	{
		int n = pop<int>(_env);
		Code code = pop<Code>(_env);

		if (n <= 0)
			return 1;

		CodeArray vec(5);
		vec[0] = MyDoRange;
		vec[1] = quote;
		vec[2] = cons(int_pop, code);
		vec[3] = Code(parallel_intLiteralFactory.local().createLiteral(n - 1));
		vec[4] = zero;
		_env.local().push_code_to_exec_stack(Code(parallel_codeListFactory.local().createCodeList(vec)));  // new CodeList(vec)));  //CodeList::adopt(vec)); //Code(new CodeList(vec)));

		return 1;
	}

	void initCode()
	{
		static bool initialized = false;

		if (initialized) return;

		initialized = true;

		/* CODE */
		Type binaryCode = codeType + codeType;
		make_instruction((Operator)_cons, "CODE.CONS", binaryCode, codeType);
		make_instruction((Operator)_list, "CODE.LIST", binaryCode, codeType);
		make_instruction((Operator)_do, "CODE.DO", codeType, execType);
		make_instruction((Operator)_do_star, "CODE.DO*", codeType, execType);
		make_instruction((Operator)_quote, "CODE.QUOTE", execType, codeType, 1);
		make_instruction((Operator)_if, "CODE.IF", boolType + binaryCode, execType);
		make_instruction((Operator)_length, "CODE.LENGTH", codeType, integerType);
		make_instruction((Operator)_size, "CODE.SIZE", codeType, integerType);
		make_instruction((Operator)_from_T<int>, "CODE.FROMINTEGER", integerType, codeType);
		make_instruction((Operator)_from_T<double>, "CODE.FROMFLOAT", floatType, codeType);
		make_instruction((Operator)_from_T<bool>, "CODE.FROMBOOLEAN", boolType, codeType);
//		make_instruction((Operator)_from_T<name_t>, "CODE.FROMNAME", nameType, codeType);
		make_instruction((Operator)_instructions, "CODE.INSTRUCTIONS", nullType, codeType);
		make_instruction((Operator)_append, "CODE.APPEND", binaryCode, codeType);
		make_instruction((Operator)_atom, "CODE.ATOM", codeType, boolType);
		make_instruction((Operator)_car, "CODE.CAR", codeType, codeType);
		make_instruction((Operator)_cdr, "CODE.CDR", codeType, codeType);
		make_instruction((Operator)_container, "CODE.CONTAINER", binaryCode, codeType);
		make_instruction((Operator)_extract, "CODE.EXTRACT", codeType + integerType, codeType);
		make_instruction((Operator)_insert, "CODE.INSERT", binaryCode + integerType, codeType);
		make_instruction((Operator)_member, "CODE.MEMBER", binaryCode, boolType);
		make_instruction((Operator)_noop, "CODE.NOOP", nullType, nullType);
		make_instruction((Operator)_nth, "CODE.NTH", integerType + codeType, codeType);
		make_instruction((Operator)_nthcdr, "CODE.NTHCDR", integerType + codeType, codeType);
		make_instruction((Operator)_null, "CODE.NULL", codeType, boolType);
		make_instruction((Operator)_position, "CODE.POSITION", binaryCode, integerType);
//		make_instruction((Operator)rand_code, "CODE.RAND", nullType, codeType);
//		make_instruction((Operator)rand_code, "CODE.ERC", nullType, codeType);
		make_instruction((Operator)code_do_range, "CODE.DO*RANGE", integerType + integerType + codeType, execType + execType);
		make_instruction((Operator)do_count, "CODE.DO*COUNT", integerType + codeType, execType);
		make_instruction((Operator)do_times, "CODE.DO*TIMES", integerType + codeType, execType);
		make_instruction((Operator)in2code, "CODE.IN", integerType, codeType);
		make_instruction((Operator)inall2code, "CODE.INALL", nullType, codeType);
		make_instruction((Operator)inallrev2code, "CODE.INALLREV", nullType, codeType);
	}
}