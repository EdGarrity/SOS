#include "CodeOps.h"

using namespace std;

namespace Push
{
	unsigned _nth(Env & _env)
	{
		Code first = pop<Code>(_env);
		int val = pop<int>(_env);

		if (first->get_stack().size() == 0) // nil or atom
		{
			push(_env, first);
			return 1;
		}

		const CodeArray & stack = first->get_stack();
		val = std::abs(val) % stack.size();
		push(_env, stack[stack.size() - val - 1]);

		return first->len();
	}

	unsigned _nthcdr(Env & _env)
	{
		Code first = pop<Code>(_env);
		int val = pop<int>(_env);

		if (first->get_stack().size() == 0) // nil or atom
		{
			push(_env, first);
			return 1;
		}

		CodeArray stack = first->get_stack();
		val = std::abs(val) % stack.size();

		while (--val >= 0) 
			stack.pop_back();

		push(_env, Code(parallel_codeListFactory.local().createCodeList(stack)));  // new CodeList(stack)));

		return first->len();
	}

	unsigned _null(Env & _env)
	{
		Code first = pop<Code>(_env);
		push(_env, equal_to(nil, first));
		return 1;
	}

	unsigned _position(Env & _env)
	{
		Code first = pop<Code>(_env);
		Code second = pop<Code>(_env);

		if (first->get_stack().size() == 0)
		{
			if (equal_to(first, second))
				push(_env, 0);

			else
				push(_env, -1);

			return 1;
		}

		const CodeArray & stack = first->get_stack();

		for (int i = stack.size() - 1; i >= 0; --i)
		{
			if (equal_to(stack[i], second))
			{
				push<int>(_env, stack.size() - i - 1);
				return stack.size() * second->len();
			}
		}

		push<int>(_env, -1);
		return stack.size() * second->len();
	}
}