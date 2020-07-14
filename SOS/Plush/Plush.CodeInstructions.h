#pragma once
#include "Processor.h"

namespace Plush
{
	void initCode();

	//inline unsigned _cons(Environment & _env)
	//{
	//	if (_env.has_elements<CodeAtom>(2))

	//	if (first<CodeAtom>(_env)->size() + second<CodeAtom>(_env)->size() >= _env.local().parameters.max_points_in_program) return 1;

	//	if (_env.get_stack<CodeAtom>().size() + _env.get_stack<CodeAtom>()[_env.get_stack<CodeAtom>().size() - 2].size())

	//	CodeAtom a = pop<CodeAtom>(_env);
	//	CodeAtom b = pop<CodeAtom>(_env);
	//	push(_env, cons(b, a));
	//	return 1;
	//}

	//inline CodeAtom cons(CodeAtom a, CodeAtom b)
	//{
	//	CodeArray stack = b->get_stack();

	//	if (stack.size())
	//		stack.push_back(a);

	//	else
	//	{
	//		stack.resize(2);
	//		stack[0] = b;
	//		stack[1] = a;
	//	}

	//	// new CodeList(stack)); 
	//	//CodeList::adopt(stack); 
	//	//*new CodeList(stack); 
	//	// CodeList::adopt(stack); 
	//	//Code( new CodeList(stack) );
	//	return CodeAtom(parallel_codeListFactory.local().createCodeList(stack));  
	//}

}