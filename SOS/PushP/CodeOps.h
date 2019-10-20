#pragma once
#include <iterator>

#include "Env.h"
#include "CodeUtils.h"
#include "Literal.h"

namespace Push 
{
	inline
		unsigned _cons(Env & _env)
	{

		if (first<Code>(_env)->size() + second<Code>(_env)->size() >= _env.local().parameters.max_points_in_program) return 1;

		Code a = pop<Code>(_env);
		Code b = pop<Code>(_env);
		push(_env, cons(b, a));
		return 1;
	}

	inline
		unsigned _list(Env & _env)
	{
		if (1 + first<Code>(_env)->size() + second<Code>(_env)->size() >= _env.local().parameters.max_points_in_program) return 1;

		Code a = pop<Code>(_env);
		Code b = pop<Code>(_env);
		push(_env, list(b, a));
		return 1;
	}

	inline unsigned _length(Env & _env)
	{
		Code a = pop<Code>(_env);
		push<int>(_env, a->len());
		return 1;
	}

	inline unsigned _size(Env & _env) 
	{
		Code a = pop<Code>(_env);
		push<int>(_env, a->size());
		return a->size();
	}

	/* Few Executable/quoting constructs */

	inline unsigned _do(Env & _env)
	{
		_env.local().push_code_to_exec_stack(code_pop);
		_env.local().push_code_to_exec_stack(top<Code>(_env));
		return 1;
	}

	inline unsigned _do_star(Env & _env) 
	{
		_env.local().push_code_to_exec_stack(pop<Code>(_env));
		return 1;
	}

	inline unsigned _quote(Env & _env) 
	{
		push<Code>(_env, pop<Exec>(_env).to_CodeBase());
		return 1;
	}

	inline unsigned _if(Env & _env)
	{
		Code first = pop<Code>(_env);
		Code second = pop<Code>(_env);

		if (pop<bool>(_env))
			_env.local().push_code_to_exec_stack(second);
		else
			_env.local().push_code_to_exec_stack(first);

		return 1;
	}

	//* From */
	template <class T>
	inline unsigned _from_T(Env & _env)
	{
		T a = pop<T>(_env);

		if (typeid(a) == typeid(int))
			push<Code>(_env, Code(parallel_intLiteralFactory.local().createLiteral(a)));  
		
		else if (typeid(a) == typeid(double))
			push<Code>(_env, Code(parallel_floatLiteralFactory.local().createLiteral(a)));
		
		else if (typeid(a) == typeid(bool))
			push<Code>(_env, Code(parallel_boolLiteralFactory.local().createLiteral(a)));
		
		return 1;
	}

	// Purpose: 
	//   Push the Nth element from the input array onto the Code stack.
	//
	// Parameters:
	//   None
	//
	// Return value:
	//   1
	//
	// Side Effects:
	//   If successful, a number is pushed to the Code stack
	//
	// Thread Safe:
	//   Yes
	//
	// Remarks:
	//   N is popped from the Integer stack first. If N < 0, or if N >= size of input array, or the Integer 
	//	 stack is empty then a NO-OP is executed instead.
	//
	inline unsigned in2code(Env & _env)
	{
		int index = pop<int>(_env);

		index = std::abs((int)(index % _env.local().input.size()));
		double value = _env.local().input[index];
		push<Code>(_env, Code(parallel_floatLiteralFactory.local().createLiteral(value)));

		return 1;
	}

	// Purpose: 
	//   Push all elements from the input array onto a stack.
	//
	// Parameters:
	//   None
	//
	// Return value:
	//   1
	//
	// Side Effects:
	//   If successful, all numbers are pushed to the a stack
	//
	// Thread Safe:
	//   Yes
	//
	// Remarks:
	//   if input array is empty, NO-OP is executed.
	//
	inline unsigned inall2code(Env & _env)
	{
		if (_env.local().input.size() > 0)
		{
			for (int index = 0; index < _env.local().input.size(); index++)
			{
				double value = _env.local().input[index];
				push<Code>(_env, Code(parallel_floatLiteralFactory.local().createLiteral(value)));
			}
		}

		return 1;
	}

	// Purpose: 
	//   Push all elements from the input array onto a stack in reverse order.
	//
	// Parameters:
	//   None
	//
	// Return value:
	//   1
	//
	// Side Effects:
	//   If successful, all numbers are pushed to a stack
	//
	// Thread Safe:
	//   Yes
	//
	// Remarks:
	//   if input array is empty, NO-OP is executed.
	//
	inline unsigned inallrev2code(Env & _env)
	{
		if (_env.local().input.size() > 0)
		{
			for (int index = _env.local().input.size() - 1; index >= 0; index--)
			{
				double value = _env.local().input[index];
				push<Code>(_env, Code(parallel_floatLiteralFactory.local().createLiteral(value)));
			}
		}

		return 1;
	}

	inline unsigned _instructions(Env & _env)
	{ 
		push(_env, _env.local().function_set);
		return 1; 
	}

	inline size_t _append(Env & _env)
	{
		if (1 + first<Code>(_env)->size() + second<Code>(_env)->size() >= _env.local().parameters.max_points_in_program)
			return 1;

		Code first = pop<Code>(_env);
		Code second = pop<Code>(_env);

		CodeArray first_stack = first->get_stack();
		if (first_stack.empty()) 
			first_stack.push_back(first);

		CodeArray second_stack = second->get_stack();
		if (second_stack.empty()) 
			second_stack.push_back(second);

		second_stack.reserve(second_stack.size() + first_stack.size());
		std::copy(first_stack.begin(), first_stack.end(), std::back_inserter(second_stack));

		push<Code>(_env, Code(parallel_codeListFactory.local().createCodeList(second_stack)));  // new CodeList(second_stack))); //CodeList::adopt(second_stack)); //push<Code>(new CodeList(second_stack)); // push<Code>(CodeList::adopt(second_stack)); //Code(new CodeList(second_stack)));
		return first_stack.size();
	}

	inline unsigned _atom(Env & _env)
	{
		if (pop<Code>(_env)->get_stack().size())
			push<bool>(_env, false);
		else
			push<bool>(_env, true);

		return 1;
	}

	inline unsigned _car(Env & _env)
	{
		if (top<Code>(_env)->get_stack().size())
			push<Code>(_env, pop<Code>(_env)->get_stack().back());
		
		return 1;
	}

	inline unsigned _cdr(Env & _env)
	{
		if (top<Code>(_env)->get_stack().size()) 
		{
			CodeArray stack = pop<Code>(_env)->get_stack();
			stack.pop_back();
			push<Code>(_env, Code(parallel_codeListFactory.local().createCodeList(stack)));  // new CodeList(stack)));  //CodeList::adopt(stack)); //Code(new CodeList(stack)));
		}

		return 1;
	}

	inline unsigned _container(Env & _env)
	{
		Code first = pop<Code>(_env);
		Code second = pop<Code>(_env);

		push(_env, find_container(first, second));

		return first->size() * second->size();
	}

	inline unsigned _extract(Env & _env) 
	{
		Code code = pop<Code>(_env);
		int val = pop<int>(_env);

		if (code->size() == 0) 
		{ // nil
			push(_env, code);
			return 1;
		}

		val = std::abs(val) % code->size();

		push(_env, extract(code, val));
		return code->size(); // Complexity is O(n) 
	}

	inline unsigned _insert(Env & _env) 
	{
		Code one = first<Code>(_env);
		Code other = second<Code>(_env);
		int val = first<int>(_env);

		val = std::abs(val) % one->size();

		Code result = insert(one, val, other);
		if (result->size() > _env.local().parameters.max_points_in_program)
			return 1;

		pop<Code>(_env);
		pop<Code>(_env);
		pop<int>(_env);

		push(_env, result);

		return 1;
	}

	inline unsigned _member(Env & _env)
	{
		Code first = pop<Code>(_env);
		Code second = pop<Code>(_env);

		push(_env, member(first, second));

		return 1;
	}

	inline unsigned _noop() { return 1; }

	/* large parts of the stuff below should go to a sourc file */

	unsigned _nth(Env & _env);
	unsigned _nthcdr(Env & _env);
	unsigned _null(Env & _env);
	unsigned _position(Env & _env);
}
