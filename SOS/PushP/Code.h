#pragma once

#define NOMINMAX

#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <forward_list>
#include <iostream>
#include <exception>
#include "CodeBasePtr.h"
//#include <windows.h>
#include <ppl.h>
//#include <array>
//#include <numeric>
//#include <iostream>

using namespace concurrency;

namespace Push
{
	/*
	 * program ::= instruction | literal | ( program* )
	 *
	 */

	class Env;
	class CodeBase;

	// This needs to be initialize in Push Initialze and stored in Thread Local Storage
	extern thread_local Env env;

	typedef Push::detail::CodeBase_prt<CodeBase> Code;
	typedef Push::detail::ExecBase_ptr<CodeBase> Exec;
	typedef std::vector<Code> CodeArray;
	typedef unsigned(*Operator)();

	typedef std::map<std::string, Push::Code> String2CodeMap;
	typedef std::map<std::string, unsigned int> String2ParenthesesMap;
	typedef std::set<Push::Code> CodeSet;

	/* Globals (Check StaticInit) */
	extern const String2CodeMap &str2code_map;
	extern const Code &instructions;
	extern const CodeSet &erc_set;
	extern const Code &nil;

	//
	// Code/Exec memory manager
	//

	template <class T>
	class LiteralFactory;

	extern combinable<LiteralFactory<int>> parallel_intLiteralFactory;
	extern combinable<LiteralFactory<double>> parallel_floatLiteralFactory;
	extern combinable<LiteralFactory<bool>> parallel_boolLiteralFactory;

	//
	// Static Push instructions
	//
	extern Code MyDoRange;
	extern Code zero;
	extern Code quote;
	extern Code int_pop;
	extern Code code_pop;
	extern Code rnd;
	extern Code ycode;

	Code parse(std::string s);

	void init_static_PushP_instructions();

	//
	// CodeBase
	//

	class CodeBase
	{
	private:
		friend class CodeList; // the only one allowed to touch the stack
		CodeArray _stack;

	public:

		CodeBase();
		CodeBase(const CodeArray &stack);
		CodeBase(const CodeBase &code);
		CodeBase(const CodeBase *code);
		const CodeArray &get_stack() const
		{
			return _stack;
		}

		virtual ~CodeBase() {}

	//	/* virtual functions */
		virtual unsigned operator()() const //virtual unsigned operator()(Env &env) const
//		virtual unsigned operator()(Env &env) const
		{
			return 1;
		}

		virtual size_t binary_size() const // Size of the thing if we look at it as a binary (cons-pair) tree
		{
			return 1;
		}
		virtual unsigned size() const
		{
			return 1;
		}
		virtual size_t len()  const
		{
			return 1;
		}

		virtual std::string to_string() const
		{
			return NULL;
		}

		virtual unsigned int instruction_paren_groups() const
		{
			return 0;
		}

		virtual void set_parentheses(unsigned int _parentheses)
		{
			return;
		}

		virtual bool equal_to(const CodeBase &other) const
		{
			return true;
		};

		// Comparitor fuctions required for std::set
		bool operator==(const CodeBase &other) const;
		bool operator!=(const CodeBase &other) const;
		bool operator<(const CodeBase &other) const;

		enum TYPE_ID
		{
			unknown = 0,
			ins,
			integer,
			floating_point,
			boolean
		};

		virtual TYPE_ID get_type2()
		{
			return TYPE_ID::unknown;
		}
	};

	class CodeAtom : public CodeBase
	{
	public:
		size_t binary_size() const
		{
			return 1;
		}
		unsigned size() const
		{
			return 1;
		}
		size_t len() const
		{
			return 1;
		}
	};

	class CodeList : public CodeBase
	{
		unsigned _size;
		size_t _binary_size;

		void calc_sizes();

	public:

		CodeList() : _size(1u), _binary_size(1u) // _size( -1u ), _binary_size( -1u )
		{
			calc_sizes(); // Sets _size and _binary_size anyway.
		}
		CodeList(const CodeArray &stack) : CodeBase(stack), _size(1u), _binary_size(1u) // _size( -1u ), _binary_size( -1u )
		{
			calc_sizes();
		}
		CodeList(Code a);

		// implementation of virtuals
		size_t binary_size() const
		{
			return _binary_size;
		}
		unsigned size() const
		{
			return _size;
		}
		size_t len()  const
		{
			return _stack.size();
		}

		std::string to_string() const;

		unsigned operator()() const;

		// no equal_to overload, equality checked in parent

		// special 'constructor' that will destroy the argument (for efficiency reasons)
		//static Code adopt(CodeArray &vec)  // beware, destructs vec!
		//{
		//	CodeList* lst = new CodeList;
		//	lst->_stack.swap(vec); // destructs vec
		//	lst->calc_sizes();
		//	return Code(lst);
		//}

		TYPE_ID get_type()
		{
			return TYPE_ID::unknown;
		}
	};

	//
	// CodeList memory manager
	//
	class CodeListRegisterNode
	{
		CodeList* _p;
		CodeListRegisterNode* _next;

		friend class CodeListRegister;

	public:
		CodeListRegisterNode(CodeList *p_, CodeListRegisterNode* next_)
		{
			_p = p_;
			_next = next_;
		}
	};

	class CodeListRegister
	{
		CodeListRegisterNode* _head;

	public:
		CodeListRegister() : _head(nullptr) {}

		~CodeListRegister()
		{
			clean_up();
		}

		void record(CodeList* p)
		{
			CodeListRegisterNode* node = new CodeListRegisterNode(p, _head);
			_head = node;
		}

		void clean_up()
		{
			CodeListRegisterNode* current_node = _head;

			while (current_node != nullptr)
			{
				CodeListRegisterNode* next_node = current_node->_next;

				delete current_node->_p;
				current_node->_p = nullptr;

				delete current_node;
				current_node = nullptr;

				current_node = next_node;
			}

			_head = nullptr;
		}
	};

	class CodeListFactory
	{
		CodeListRegister codeListRegister;

	public:
		CodeList* createCodeList(const CodeArray &stack);

		void clean_up()
		{
			codeListRegister.clean_up();
		}
	};

	inline CodeList* CodeListFactory::createCodeList(const CodeArray &stack)
	{
		CodeList* lp = new CodeList(stack);
		codeListRegister.record(lp);
		return lp;
	}

//	extern thread_local CodeListFactory *codeListFactory;
	extern combinable<CodeListFactory> parallel_codeListFactory;


	inline std::string str(Code code)
	{
		if (code.get() == 0) return "no code";

		return code->to_string();
	}

	inline std::ostream &operator<<(std::ostream &os, Code code)
	{
		os << str(code);
		return os;
	}

	/* Assumes an expression is found on the line itself */
	inline bool equal_to(Code a, Code b, int &effort)
	{
		bool res = *a == *b;
		effort = (std::max)(a->size(), b->size());	// Extra set of parentheses arround std:max added to prevent macro invocation.
		return res;
	}
	inline bool equal_to(Code a, Code b)
	{
		return *a == *b;
	}
	bool smallerCode(Code &a, Code &b);
}