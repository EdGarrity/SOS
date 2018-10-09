#pragma once
#include <iterator>

#include "Env.h"
#include "CodeUtils.h"
#include "Literal.h"

namespace push 
{
	inline
		unsigned _cons() {

		if (first<Code>()->size() + second<Code>()->size() >= env.parameters.max_points_in_program) return 1;

		Code a = pop<Code>(env);
		Code b = pop<Code>(env);
		push(cons(b, a));
		return 1;
	}

	inline
		unsigned _list() {
		if (1 + first<Code>()->size() + second<Code>()->size() >= env.parameters.max_points_in_program) return 1;

		Code a = pop<Code>(env);
		Code b = pop<Code>(env);
		push(list(b, a));
		return 1;
	}

	inline unsigned _length() {
		Code a = pop<Code>(env);
		push<int>(a->len());
		return 1;
	}

	inline unsigned _size() {
		Code a = pop<Code>(env);
		push<int>(a->size());
		return a->size();
	}

	/* Few Executable/quoting constructs */

	inline unsigned _do() {
		static Code code_pop = parse("CODE.POP");
		env.push_guarded(code_pop);
		env.push_guarded(top<Code>());
		return 1;
	}

	inline unsigned _do_star() {
		env.push_guarded(pop<Code>(env));
		return 1;
	}

	inline unsigned _quote() {
		push<Code>(pop<Exec>(env).lock());
		return 1;
	}

	inline unsigned _if() {
		Code first = pop<Code>(env);
		Code second = pop<Code>(env);

		if (pop<bool>(env))
			env.push_guarded(second);
		else
			env.push_guarded(first);

		return 1;
	}

	///* From */
	template <class T>
	inline unsigned _from_T() {
		T a = pop<T>(env);
		push<Code>(Code(new Literal<T>(a)));
		return 1;
	}


	inline unsigned _instructions() 
	{ 
		push(env.function_set); 
		return 1; 
	}

	inline unsigned _append() {
		if (1 + first<Code>()->size() + second<Code>()->size() >= env.parameters.max_points_in_program) return 1;

		Code first = pop<Code>(env);
		Code second = pop<Code>(env);

		CodeArray first_stack = first->get_stack();
		if (first_stack.empty()) first_stack.push_back(first);

		CodeArray second_stack = second->get_stack();
		if (second_stack.empty()) second_stack.push_back(second);

		second_stack.reserve(second_stack.size() + first_stack.size());
		std::copy(first_stack.begin(), first_stack.end(), std::back_inserter(second_stack));

		push<Code>(CodeList::adopt(second_stack)); //push<Code>(new CodeList(second_stack)); // push<Code>(CodeList::adopt(second_stack)); //Code(new CodeList(second_stack)));
		return first_stack.size();
	}

	inline unsigned _atom() 
	{
		if (pop<Code>(env)->get_stack().size())
			push<bool>(false);
		else
			push<bool>(true);

		return 1;
	}

	inline unsigned _car() 
	{
		if (top<Code>()->get_stack().size())
			push<Code>(pop<Code>(env)->get_stack().back());
		
		return 1;
	}

	inline unsigned _cdr() 
	{
		if (top<Code>()->get_stack().size()) {
			CodeArray stack = pop<Code>(env)->get_stack();
			stack.pop_back();
			push<Code>(CodeList::adopt(stack)); //Code(new CodeList(stack)));
		}

		return 1;
	}

	inline unsigned _container() 
	{
		Code first = pop<Code>(env);
		Code second = pop<Code>(env);

		push(find_container(first, second));

		return first->size() * second->size();
	}

	//inline unsigned _contains() {

	//	Code first = pop<Code>();
	//	Code second = pop<Code>();

	//	push(contains(first, second));
	//	return first.size() * second.size();
	//}

	inline unsigned _extract() {
		Code code = pop<Code>(env);
		int val = pop<int>(env);

		if (code->size() == 0) 
		{ // nil
			push(code);
			return 1;
		}

		val = std::abs(val) % code->size();

		push(extract(code, val));
		return code->size(); // Complexity is O(n) 
	}

	inline unsigned _insert() {

		Code one = first<Code>();
		Code other = second<Code>();
		int val = first<int>();

		val = std::abs(val) % one->size();

		Code result = insert(one, val, other);
		if (result->size() > env.parameters.max_points_in_program) 
			return 1;

		pop<Code>(env);
		pop<Code>(env);
		pop<int>(env);

		push(result);

		return 1;
	}

	inline unsigned _member() 
	{
		Code first = pop<Code>(env);
		Code second = pop<Code>(env);

		push(member(first, second));

		return 1;
	}

	inline unsigned _noop() { return 1; }

	/* large parts of the stuff below should go to a sourc file */

	unsigned _nth();
	unsigned _nthcdr();
	unsigned _null();
	unsigned _position();
}
