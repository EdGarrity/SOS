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
	
	unsigned s()
	{
		Exec x = pop<Exec>(env);
		Exec y = pop<Exec>(env);
		Exec z = pop<Exec>(env);

		if (z->size() + y->size() + 1 >= env.parameters.max_points_in_program)
		{
			push(z);
			push(y);
			push(x);
			return 1;
		}

		env.push_code_to_exec_stack(list(y.to_CodeBase(), z.to_CodeBase()));
		push(z);
		push(x);
		return 1;
	}

	unsigned k()
	{
		Exec x = pop<Exec>(env);
		pop<Exec>(env);
		push(x);
		return 1;
	}

	unsigned y()
	{
		Exec x = pop<Exec>(env);
//		static Code ycode = parse("EXEC.Y");

		if (2 + x->size() >= env.parameters.max_points_in_program)
		{
			push(x); // too big
			return 1;
		}

		env.push_code_to_exec_stack(list(ycode, x.to_CodeBase()));
		push(x);

		return 1;
	}

	unsigned exec_if()
	{
		bool val = pop<bool>(env);
		Exec a = pop<Exec>(env);
		Exec b = pop<Exec>(env);

		if (val)
			push(a);

		else
			push(b);

		return 1;
	}

	unsigned do_range()
	{
		int n = pop<int>(env);
		int i = pop<int>(env);
		Exec code = pop<Exec>(env);
		CodeArray vec(4);
		vec[0] = code.to_CodeBase();
		vec[1] = MyDoRange;
		vec[2] = Code(parallel_intLiteralFactory.local().createLiteral(n));
		vec[3] = Code(parallel_intLiteralFactory.local().createLiteral(i));
		Code result = Code(parallel_doRangeClassFactory.local().createDoRangeClass(vec));  //  new DoRangeClass(vec));
		env.push_code_to_exec_stack(result);
		return 1;
	}

	static unsigned do_count()
	{
		int n = pop<int>(env);
		Exec code = pop<Exec>(env);

		if (n < 0)
			return 1;

		CodeArray vec(4);
		vec[0] = code.to_CodeBase();
		vec[1] = MyDoRange;
		vec[2] = Code(parallel_intLiteralFactory.local().createLiteral(n - 1));
		vec[3] = zero;

		Code result = Code(parallel_doRangeClassFactory.local().createDoRangeClass(vec));  //  new DoRangeClass(vec));
		env.push_code_to_exec_stack(result);

		return 1;
	}

	static unsigned do_times()
	{
		int n = pop<int>(env);
		Exec code = pop<Exec>(env);

		if (n <= 0)
			return 1;

		CodeArray vec(4);
		vec[0] = cons(int_pop, code.to_CodeBase());
		vec[1] = MyDoRange;
		vec[2] = Code(parallel_intLiteralFactory.local().createLiteral(n - 1));
		vec[3] = zero;

		Code result = Code(new DoRangeClass(vec)); // Potetial memory leak
		env.push_code_to_exec_stack(result);

		return 1;
	}

	unsigned exec_while()
	{
		if (is_empty<bool>())
			pop<Exec>(env);

		//if (top<bool>() == false)
		if (get_stack<bool>().back() == false)
		{
			pop<bool>(env);
			pop<Exec>(env);
		}

		else
		{
			Exec block = pop<Exec>(env);

			env.push_code_to_exec_stack(parse("EXEC.WHILE"));
			push(block);
			pop<bool>(env);
		}

		return 1;
	}

	unsigned do_while()
	{
		Exec block = pop<Exec>(env);

		env.push_code_to_exec_stack(parse("EXEC.WHILE"));
		push(block);

		return 1;
	}

	unsigned exec_when()
	{
		//bool cond = first<bool>();
		bool cond = get_stack<bool>().back();
		pop<bool>(env);

		if (!cond)
			pop<Exec>(env);

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