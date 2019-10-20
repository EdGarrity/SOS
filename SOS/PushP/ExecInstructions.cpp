#include "ExecInstruction.h"

#include <windows.h>
#include <ppl.h>
#include <array>
#include <numeric>
#include <iostream>

using namespace concurrency;
using namespace std;

namespace Push
{
//	thread_local DoRangeClassFactory *doRangeClassFactory;
	combinable<DoRangeClassFactory> parallel_doRangeClassFactory;

	extern Code quote;
	extern Code zero;
	extern Code int_pop;
	
	unsigned s(Env & _env)
	{
		Exec x = pop<Exec>(_env);
		Exec y = pop<Exec>(_env);
		Exec z = pop<Exec>(_env);

		if (z->size() + y->size() + 1 >= _env.local().parameters.max_points_in_program)
		{
			push(_env, z);
			push(_env, y);
			push(_env, x);

			return 1;
		}

		_env.local().push_code_to_exec_stack(list(y.to_CodeBase(), z.to_CodeBase()));
		push(_env, z);
		push(_env, x);

		return 1;
	}

	unsigned k(Env & _env)
	{
		Exec x = pop<Exec>(_env);
		pop<Exec>(_env);
		push(_env, x);

		return 1;
	}

	unsigned y(Env & _env)
	{
		Exec x = pop<Exec>(_env);

		if (2 + x->size() >= _env.local().parameters.max_points_in_program)
		{
			push(_env, x); // too big
			return 1;
		}

		_env.local().push_code_to_exec_stack(list(ycode, x.to_CodeBase()));
		push(_env, x);

		return 1;
	}

	unsigned exec_if(Env & _env)
	{
		bool val = pop<bool>(_env);
		Exec a = pop<Exec>(_env);
		Exec b = pop<Exec>(_env);

		if (val)
			push(_env, a);

		else
			push(_env, b);

		return 1;
	}

	unsigned do_range(Env & _env)
	{
		int n = pop<int>(_env);
		int i = pop<int>(_env);

		Exec code = pop<Exec>(_env);
		
		CodeArray vec(4);
		vec[0] = code.to_CodeBase();
		vec[1] = MyDoRange;
		vec[2] = Code(parallel_intLiteralFactory.local().createLiteral(n));
		vec[3] = Code(parallel_intLiteralFactory.local().createLiteral(i));
		
		Code result = Code(parallel_doRangeClassFactory.local().createDoRangeClass(vec));  //  new DoRangeClass(vec));
		_env.local().push_code_to_exec_stack(result);
		
		return 1;
	}

	static unsigned do_count(Env & _env)
	{
		int n = pop<int>(_env);
		Exec code = pop<Exec>(_env);

		if (n < 0)
			return 1;

		CodeArray vec(4);
		vec[0] = code.to_CodeBase();
		vec[1] = MyDoRange;
		vec[2] = Code(parallel_intLiteralFactory.local().createLiteral(n - 1));
		vec[3] = zero;

		Code result = Code(parallel_doRangeClassFactory.local().createDoRangeClass(vec));  //  new DoRangeClass(vec));
		_env.local().push_code_to_exec_stack(result);

		return 1;
	}

	static unsigned do_times(Env & _env)
	{
		int n = pop<int>(_env);
		Exec code = pop<Exec>(_env);

		if (n <= 0)
			return 1;

		CodeArray vec(4);
		vec[0] = cons(int_pop, code.to_CodeBase());
		vec[1] = MyDoRange;
		vec[2] = Code(parallel_intLiteralFactory.local().createLiteral(n - 1));
		vec[3] = zero;

		Code result = Code(new DoRangeClass(vec)); // Potetial memory leak
		_env.local().push_code_to_exec_stack(result);

		return 1;
	}

	unsigned exec_while(Env & _env)
	{
		if (is_empty<bool>(_env))
			pop<Exec>(_env);

		//if (top<bool>() == false)
		if (get_stack<bool>(_env).back() == false)
		{
			pop<bool>(_env);
			pop<Exec>(_env);
		}

		else
		{
			Exec block = pop<Exec>(_env);

			_env.local().push_code_to_exec_stack(parse("EXEC.WHILE"));
			push(_env, block);
			pop<bool>(_env);
		}

		return 1;
	}

	unsigned do_while(Env & _env)
	{
		Exec block = pop<Exec>(_env);

		_env.local().push_code_to_exec_stack(parse("EXEC.WHILE"));
		push(_env, block);

		return 1;
	}

	unsigned exec_when(Env & _env)
	{
		bool cond = get_stack<bool>(_env).back();
		pop<bool>(_env);

		if (!cond)
			pop<Exec>(_env);

		return 1;
	}

	void initExec()
	{
		static bool initialized = false;

		if (initialized) return;

		initialized = true;
		make_instruction((Operator)k, "EXEC.K", execType + execType, execType, 2);
		make_instruction((Operator)s, "EXEC.S", execType + execType + execType, execType + execType + execType, 3);
		make_instruction((Operator)y, "EXEC.Y", execType, execType + execType, 1);
		make_instruction((Operator)exec_if, "EXEC.IF", execType + execType + boolType, execType, 2);
		make_instruction((Operator)do_range, "EXEC.DO*RANGE", integerType + integerType + execType, execType, 1);
		make_instruction((Operator)do_count, "EXEC.DO*COUNT", integerType + execType, execType, 1);
		make_instruction((Operator)do_times, "EXEC.DO*TIMES", integerType + execType, execType, 1);
		make_instruction((Operator)exec_while, "EXEC.WHILE", boolType + execType, execType, 1);
		make_instruction((Operator)do_while, "EXEC.DO*WHILE", execType, execType, 1);
		make_instruction((Operator)exec_when, "EXEC.WHEN", boolType + execType, execType, 1);

		set_parentheses("EXEC.ROT", 3);
		set_parentheses("EXEC.POP", 1);
		set_parentheses("EXEC.DUP", 1);
		set_parentheses("EXEC.SWAP", 2);
		set_parentheses("EXEC.ROT", 3);
		set_parentheses("EXEC.SHOVE", 1);
	}
}