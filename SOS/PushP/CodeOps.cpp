#include "CodeOps.h"

using namespace std;

namespace Push
{
	unsigned _nth()
	{
		Code first = pop<Code>(env);
		int val = pop<int>(env);

		if (first->get_stack().size() == 0) // nil or atom
		{
			push(first);
			return 1;
		}

		const CodeArray & stack = first->get_stack();
		val = std::abs(val) % stack.size();
		push(stack[stack.size() - val - 1]);
		return first->len();
	}

	unsigned _nthcdr()
	{
		Code first = pop<Code>(env);
		int val = pop<int>(env);

		if (first->get_stack().size() == 0) // nil or atom
		{
			push(first);
			return 1;
		}

		CodeArray stack = first->get_stack();
		val = std::abs(val) % stack.size();

		while (--val >= 0) stack.pop_back();

		push(Code(codeListFactory.createCodeList(stack)));  // new CodeList(stack)));

		return first->len();
	}

	unsigned _null()
	{
		Code first = pop<Code>(env);
		push(equal_to(nil, first));
		return 1;
	}

	unsigned _position()
	{
		Code first = pop<Code>(env);
		Code second = pop<Code>(env);

		if (first->get_stack().size() == 0)
		{
			if (equal_to(first, second))
				push(0);

			else
				push(-1);

			return 1;
		}

		const CodeArray & stack = first->get_stack();

		for (int i = stack.size() - 1; i >= 0; --i)
		{
			if (equal_to(stack[i], second))
			{
				push<int>(stack.size() - i - 1);
				return stack.size() * second->len();
			}
		}

		push<int>(-1);
		return stack.size() * second->len();
	}
}