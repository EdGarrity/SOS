#include "Code.h"
#include "Literal.h"
#include "Env.h"
#include "CodeUtils.h"

using namespace std;

namespace Push
{
	bool code_initialized = false;

	Code MyDoRange;
	Code zero;
	Code quote;
	Code int_pop;
	Code code_pop;
	Code rnd;
	Code ycode;

//	int init_push();

	void init_static_PushP_instructions()
	{
//		init_push();	Must be called prior to calling this funtion.

		//if (code_initialized == false)
		//{
			MyDoRange = parse("EXEC.DO*RANGE");
			zero = Code(intLiteralFactory->createLiteral(0));
			quote = parse("CODE.QUOTE");
			int_pop = parse("INTEGER.POP");
			code_pop = parse("CODE.POP");
			rnd = parse("CODE.RAND");
			ycode = parse("EXEC.Y");

			code_initialized = true;
		//}
	}

	CodeBase::CodeBase()
	{
	}

	CodeBase::CodeBase(const CodeArray & stack) : _stack(stack)
	{
	}

	CodeBase::CodeBase(const CodeBase & code) : _stack(code._stack)
	{
	}

	CodeBase::CodeBase(const CodeBase * code) : _stack(code->_stack)
	{
	}

	istream& operator>>(istream& is, Code& code) {
		code = parse(is);
		return is;
	}

	bool CodeBase::operator==(const CodeBase & b) const
	{
		const CodeBase & a = *this;

		if (&a == &b)		  
			return true;

		if (typeid(a) != typeid(b))   
			return false;

		if (a.len() != b.len())	  
			return false;

		const CodeArray & astack = a.get_stack();
		const CodeArray & bstack = b.get_stack();

		for (unsigned i = 0; i < astack.size(); ++i)
		{
			if (*astack[i] != *bstack[i])
				return false;
		}

		return a.equal_to(b);
	}

	bool CodeBase::operator!=(const CodeBase & b) const
	{
		const CodeBase & a = *this;

		if (&a == &b)		  
			return false;

		if (typeid(a) != typeid(b))   
			return true;

		if (a.len() != b.len())	  
			return true;

		const CodeArray & astack = a.get_stack();
		const CodeArray & bstack = b.get_stack();

		for (unsigned i = 0; i < astack.size(); ++i)
		{
			if (*astack[i] != *bstack[i])
				return true;
		}

		return !(a.equal_to(b));
	}

	bool CodeBase::operator<(const CodeBase & b) const
	{
		const CodeBase & a = *this;

		if (&a == &b)
			return false;

		if (typeid(a) != typeid(b))
			return false;

		if (a.len() > b.len())
			return false;

		const CodeArray & astack = a.get_stack();
		const CodeArray & bstack = b.get_stack();

		for (unsigned i = 0; i < astack.size(); ++i)
		{
			if (!(*astack[i] < *bstack[i]) && (*astack[i] != *bstack[i]))
				return false;
		}

		return true;
	}

	bool smallerCode(Code &a, Code &b)
	{
		if (equal_to(a, b)) return false;

		if (a->len() < b->len()) return true;

		if (a->len() > b->len()) return false;

		// same size now check contents...
		const CodeArray & as = a->get_stack();
		const CodeArray & bs = b->get_stack();

		//if (as.size() == 0)
		//{
		//	return a.get() < b.get(); // pointer compare, atoms are unique (hmm, except constantcode...)
		//}

		for (unsigned i = 0; i < as.size(); ++i)
		{
			Code suba = as[i];
			Code subb = bs[i];

			if (!equal_to(suba, subb))
				return smallerCode(suba, subb);
		}

		return false;
	}

	CodeList::CodeList(Code a)
	{
		_stack.resize(1);
		_stack[0] = a;
		_size = a->size() + 1;
		_binary_size = a->binary_size(); // or +1? don't know
	}

	void CodeList::calc_sizes()
	{
		_size = 1;
		_binary_size = (_stack.size() == 0) 
			? 0 
			: _stack.size() - 1; // internal nodes

		// debug
		//if (_stack.size() > 0)
		//{
		//	Code c1 = _stack[0];
		//	CodeBase p1 = c1.get();
		//	size_t s1 = c1->size();
		//}

		for (CodeArray::const_iterator it = _stack.begin(); it != _stack.end(); ++it)
		{
			_size += (*it)->size();
			_binary_size += (*it)->binary_size();
		}

		//_size = 1;
		//_binary_size = _stack.size() - 1; // internal nodes

		//for (CodeArray::const_iterator it = _stack.begin(); it != _stack.end(); ++it) 
		//{
		//	_size += (*it)->size();
		//	_binary_size += (*it)->binary_size();
		//}
	}

	std::string CodeList::to_string() const
	{
		std::string r = "(";

		if (_stack.size() > 0)
		{
			for (long i = _stack.size() - 1; i >= 0; --i)
				r += _stack[i]->to_string() + (i == 0 ? "" : " ");
		}

		r += ")";
		return r;
	}

	unsigned CodeList::operator()() const
	{
		for (CodeArray::const_iterator it = _stack.begin(); it != _stack.end(); ++it)
			env.exec_stack.push_back(Exec(*it));

		return 1;
	}
}
