#include "Instruction.h"
#include "Env.h"
#include "CodeOps.h"

using namespace std;

namespace Push
{
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

		env.push_code_to_exec_stack(list(y.lock(), z.lock()));
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
		static Code ycode = parse("EXEC.Y");

		if (2 + x->size() >= env.parameters.max_points_in_program)
		{
			push(x); // too big
			return 1;
		}

		env.push_code_to_exec_stack(list(ycode, x.lock()));
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

	class DoRangeClass : public CodeList
	{
	public:
		DoRangeClass(const CodeArray & vec) : CodeList(vec)
		{
			//			assert(vec.size() == 4);
		}

		unsigned operator()() const
		{
			CodeArray vec = get_stack();
			int i = static_cast<Literal<int>*>(vec[3].get())->get();
			int n = static_cast<Literal<int>*>(vec[2].get())->get();
			int direction = 1;

			if (i > n) direction = -1;

			push(i);
			Exec code = Exec(vec[0]);

			if (i != n)
			{
				vec[3] = Code(new Literal<int>(i + direction));
				Code ranger = CodeList::adopt(vec);
				env.push_code_to_exec_stack(ranger);
			}

			push(code);
			return 1;
		}
	};

	unsigned do_range()
	{
		int n = pop<int>(env);
		int i = pop<int>(env);
		static Code DoRange = parse("EXEC.DO*RANGE");
		Exec code = pop<Exec>(env);
		CodeArray vec(4);
		vec[0] = code.lock();
		vec[1] = DoRange;
		vec[2] = Code(new Literal<int>(n));
		vec[3] = Code(new Literal<int>(i));
		Code result = Code(new DoRangeClass(vec));
		env.push_code_to_exec_stack(result);
		return 1;
	}

	static unsigned do_count()
	{
		int n = pop<int>(env);
		Exec code = pop<Exec>(env);

		if (n < 0)
			return 1;

		static Code DoRange = parse("EXEC.DO*RANGE");
		CodeArray vec(4);
		static Code zero = Code(new Literal<int>(0));
		vec[0] = code.lock();
		vec[1] = DoRange;
		vec[2] = Code(new Literal<int>(n - 1));
		vec[3] = zero;
		Code result = Code(new DoRangeClass(vec));
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
		static Code zero = Code(new Literal<int>(0));
		static Code int_pop = parse("INTEGER.POP");
		static Code DoRange = parse("EXEC.DO*RANGE");
		vec[0] = cons(int_pop, code.lock());
		vec[1] = DoRange;
		vec[2] = Code(new Literal<int>(n - 1));
		vec[3] = zero;
		Code result = Code(new DoRangeClass(vec));
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