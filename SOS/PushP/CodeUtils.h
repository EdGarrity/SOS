#pragma once

#include <iostream>
#include "Code.h"
#include "TypeDef.h"

namespace Push
{
	class Env;
	class Type;

	inline Code cons(Code a, Code b)
	{
		CodeArray stack = b->get_stack();

		if (stack.size())
			stack.push_back(a);

		else
		{
			stack.resize(2);
			stack[0] = b;
			stack[1] = a;
		}

		return Code(new CodeList(stack)); //CodeList::adopt(stack); //*new CodeList(stack); // CodeList::adopt(stack); //Code( new CodeList(stack) );
	}

	inline Code list(Code a, Code b)
	{
		CodeArray stack(2);
		stack[0] = b;
		stack[1] = a;
		return Code(new CodeList(stack)); //CodeList::adopt(stack); //return *new CodeList(stack); // return CodeList::adopt(stack); //Code( new CodeList(stack) );
	}

	inline Code car(Code a)
	{
		const CodeArray &stack = a->get_stack();

		if (stack.size() == 0) return a;

		return stack.back();
	}

	inline Code cdr(Code a)
	{
		CodeArray stack = a->get_stack();

		if (stack.size() == 0) return nil;

		stack.pop_back();
		return Code(new CodeList(stack)); //CodeList::adopt(stack); //Code( new CodeList(stack));
	}

	extern Code random_code(unsigned maxpoints);
	extern Code find_container(Code tree, Code subtree);

	extern Code extract(Code code, unsigned val);
	extern Code insert(Code code, unsigned val, Code subcode);
	extern bool member(Code code, Code query);
	extern Code pack(const Type &type);
	
	extern Code parse(std::string s);
	extern Code parse(std::istream &is);
	extern Code parse1(std::istream &is);

	extern Code make_terminal();
}