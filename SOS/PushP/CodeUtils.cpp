// istringstream constructors.
#include <iostream>     // std::cout
#include <sstream>      // std::istringstream
#include <string>       // std::string

#include "CodeUtils.h"
#include "RNG.h"
#include "Env.h"
#include "Literal.h"

namespace Push
{
	Code find_container(Code tree, Code subtree)
	{
		// check if *this* is a container of subtree
		const CodeArray & stack = tree->get_stack();

		for (unsigned i = 0; i < stack.size(); ++i)
		{
			if (equal_to(subtree, stack[i]))
				return tree;
		}

		// else check if one of the children is a container (in reverse order)

		for (int i = stack.size() - 1; i >= 0; --i)
		{
			Code container = find_container(stack[i], subtree);

			if (container->get_stack().size() > 0) // container is always a list
				return container;
		}

		return nil;
	}

	Code extract(Code code, unsigned val)
	{
//		assert(val < code->size());

		if (val == 0) return code; // found

		val -= 1;
		const CodeArray & stack = code->get_stack();

		for (int i = stack.size() - 1; i >= 0; --i)
		{
			if (val < stack[i]->size())
				return extract(stack[i], val);

			val -= stack[i]->size();
		}

		// should not happen
		return code;
	}

	Code insert(Code code, unsigned val, Code subcode)
	{
//		assert(val < code->size());

		if (val == 0) return subcode;

		val -= 1;
		CodeArray stack = code->get_stack();
//		assert(stack.size());

		for (int i = stack.size() - 1; i >= 0; --i)
		{
			if (val < stack[i]->size())
			{
				Code newcode = insert(stack[i], val, subcode);
				stack[i] = newcode;
				return Code(codeListFactory->createCodeList(stack));  // new CodeList(stack));  //CodeList::adopt(stack); //Code( new CodeList(stack) );
			}

			val -= stack[i]->size();
		}

		return code; // should not happen
	}

	bool member(Code code, Code query)
	{
		const CodeArray & stack = code->get_stack();

		if (stack.size() == 0)
			return equal_to(code, query);

		for (unsigned i = 0; i < stack.size(); ++i)
		{
			if (equal_to(stack[i], query))
				return true;
		}

		return false;
	}

	/* packs things from a stack in a piece of code  */
	Code pack(const Type & _tp)
	{
		Type type = abs(_tp);

		if (!type.can_pop_from()) return nil;

		std::vector<int> tp = type.get();
		CodeArray resultvec;

		for (unsigned i = 0; i < tp.size(); ++i)
		{
			CodeArray codevec;

			for (unsigned j = 0; j < unsigned(tp[i]); ++j)
			{
				Code code = env.pop_stack_from_id(i);
				codevec.push_back(code);
			}

			if (codevec.size())
				resultvec.push_back(codeListFactory->createCodeList(codevec));  // new CodeList(codevec)/*CodeList::adopt(codevec)*/); //Code(new CodeList(codevec)));
		}

		return Code(codeListFactory->createCodeList(resultvec));  // new CodeList(resultvec));  //CodeList::adopt(resultvec); //new CodeList(resultvec); // CodeList::adopt(resultvec); //Code(new CodeList(resultvec));
	}

	Code make_terminal()
	{
//		static Code rnd = parse("CODE.RAND"); // special case
		const CodeArray &instruction_list = env.function_set->get_stack();
//		Code ins = instruction_list[rng.random(instruction_list.size())];  Debugging this line

		int n = rng.random(instruction_list.size());
		Code ins = instruction_list[n];

		if (ins != rnd && erc_set.find(ins) != erc_set.end())
		{
			(*ins)();
			// get everything that's produced in this env. Need to car twice to get rid of the 'list of lists' that are produced by atoms
			Code retval = car(car(pack(env.make_type())));
			return retval;
		}

		return ins;
	}

	std::vector<unsigned> decompose(int number, int max_parts)
	{
		std::vector<unsigned> result;

		if (number == 1 || max_parts == 1)
		{
			result.push_back(1);
			return result;
		}

		unsigned this_part = rng.random(number - 1) + 1;
		result.push_back(this_part);
		std::vector<unsigned> sub = decompose(number - this_part, max_parts - 1);

		for (unsigned i = 0; i < sub.size(); ++i)
			result.push_back(sub[i]);

		return result;
	}

	Code random_code_with_size(unsigned points)
	{
		if (points == 1)
			return make_terminal();

		std::vector<unsigned> sizes_this_level = decompose(points - 1, points - 1);

		// shuffle
		for (int i = sizes_this_level.size() - 1; i > 0; --i)
		{
			int j = rng.random(i + 1);

			if (i != j)
				std::swap(sizes_this_level[i], sizes_this_level[j]);
		}

		CodeArray stack(sizes_this_level.size());

		for (unsigned i = 0; i < sizes_this_level.size(); ++i)
			stack[i] = random_code_with_size(sizes_this_level[i]);

		return Code(codeListFactory->createCodeList(stack));  // new CodeList(stack));  //CodeList::adopt(stack); //new CodeList(stack); // CodeList::adopt(stack); //Code(new CodeList(stack));
	}

	Code random_code(unsigned maxpoints)
	{
		unsigned points = rng.random(maxpoints) + 1;
		env.clear_stacks();
		Code result = random_code_with_size(points);
		return result;
	}

	Code parse(std::string s)
	{
		std::istringstream is(s);
		Code code = parse1(is);

		CodeArray stack = code->get_stack();

		if (stack.size() == 1)
			code = stack.back();		// Warning, produces memory leak.

		return code;
	}

	Code parse(std::istream & is)
	{
		Code code = parse1(is);

		CodeArray stack = code->get_stack();

		if (stack.size() == 1)
			code = stack.back();		// Warning, produces memory leak.

		return code;
	}

	Code parse1(std::istream & is)
	{
		CodeArray stack;
		int brackets_open = 0;
		char c = (char)is.get();

		while (is)
		{
			// skip whitespace
			if (!is) break;

			if (c == ' ' || c == '\n' || c == '\r' || c == '\t')
			{
				c = (char)is.get();
				continue;
			}

			if (c == '#') // skip to eoln
			{
				// this code will miss lines that end with just \r
				// while (is && char(is.get()) != '\n' && ) {}
				do
				{
					c = is.get();
				} while (is && c != '\n' && c != '\r');

				c = (char)is.get();
				continue;
			}

			if (c == '(') // sub
			{
				Code program = parse1(is);
				stack.push_back(program);
				c = (char)is.get();
				continue;
			}

			if (c == ')') // sub done
			{
				break;
			}

			// atom
			std::string atom;
			atom += c;
			c = char(is.get());

			while (is && c != ' ' && c != '\n' && c != '\t' && c != '\r' && c != ')' && c != '(')
			{
				atom += c;
				c = (char)is.get();
			}

			// check BOOLEAN
			if (atom == "TRUE" || atom == "FALSE")
			{
//				Code value = Code(new Literal<bool>(atom == "TRUE" ? true : false));
				Code value = Code(boolLiteralFactory->createLiteral(atom == "TRUE" ? true : false));
				stack.push_back(value);
				continue;
			}

			String2CodeMap::const_iterator it = str2code_map.find(atom);

			if (it != str2code_map.end())
			{
				stack.push_back(it->second);
				continue;
			}

			// now left are int and float, int first
			char* end;
			long val = strtol(atom.c_str(), &end, 10);

			if (end == atom.c_str() + atom.size()) // it's an int
			{
//				Code v = Code(new Literal<int>(val));
				Code v = Code(intLiteralFactory->createLiteral(val));
				stack.push_back(v);
				continue;
			}

			double dbl = strtod(atom.c_str(), &end);

			if (end == atom.c_str() + atom.size()) // it's a double
			{
//				Code v = Code(new Literal<double>(dbl));
				Code v = Code(floatLiteralFactory->createLiteral(dbl));
				stack.push_back(v);
				continue;
			}

		//	 // finally, if all else fails, it's a name
		//	name_t index = lookup(atom);
		//	Code name = Code(new Literal<name_t>(index));

		//	stack.push_back(name);

			continue;
		}

		CodeArray stack1;

		for (CodeArray::const_reverse_iterator it = stack.rbegin(); it != stack.rend(); ++it)
			stack1.push_back(*it);

		CodeList* lp = codeListFactory->createCodeList(stack1);  // new CodeList(stack1);
		return lp;
	}
}
