#pragma once

#define NOMINMAX

#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <forward_list>
#include "CodeBasePtr.h"

namespace push
{
	/*
	 * program ::= instruction | literal | ( program* )
	 *
	 */

	class Env;
	class CodeBase;

	// This needs to be initialize in Push Initialze and stored in Thread Local Storage
	extern thread_local Env env;


	typedef push::detail::CodeBase_prt<CodeBase> Code;
	typedef push::detail::ExecBase_ptr<CodeBase> Exec;
	typedef std::vector<Code> CodeArray;
	typedef unsigned(*Operator)(Env &);

	typedef std::map<std::string, push::Code> String2CodeMap;
	typedef std::set<push::Code> CodeSet;

	/* Globals (Check StaticInit) */
	extern const String2CodeMap &str2code_map;
	extern const Code &instructions;
	extern const CodeSet &erc_set;
	extern const CodeSet &exec_set;  // DO, DO*, IF
	extern const Code &nil;

	class CodeBaseRegisterNode
	{
		CodeBase* _p;
		CodeBaseRegisterNode* _next;

		friend class CodeBaseRegister;

	public:
		CodeBaseRegisterNode(CodeBase *p_, CodeBaseRegisterNode* next_)
		{
			_p = p_;
			_next = next_;
		}
	};

	class CodeBaseRegister
	{
		CodeBaseRegisterNode* _head;

	public:
		CodeBaseRegister() : _head(nullptr) {}
		~CodeBaseRegister()
		{
			CodeBaseRegisterNode* next = _head;
			CodeBaseRegisterNode* node = _head;
			while (next != nullptr)
			{
				node = next;
				next = node->_next;

				delete node;
			}
		}
		void record(CodeBase* p)
		{
			CodeBaseRegisterNode* node = new CodeBaseRegisterNode(p, _head);
			_head = node;
		}
	};

	extern thread_local CodeBaseRegister codeBaseRegister;

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


	//protected:
		virtual bool equal_to(const CodeBase &other) const
		{
			return true;
		};

//		void * operator new(size_t size);

		// Comparitor fuctions required for std::set
		bool operator==(const CodeBase &other) const;
		bool operator!=(const CodeBase &other) const;
		bool operator<(const CodeBase &other) const;
	};

	//class ExecBase : public CodeBase
	//{
	//public:
	//	ExecBase(const CodeBase &code) : CodeBase(code) {}
	//	ExecBase() {};

	//	size_t binary_size() const
	//	{
	//		return 1;
	//	}
	//	unsigned size() const
	//	{
	//		return 1;
	//	}
	//	size_t len() const
	//	{
	//		return 1;
	//	}
	//};

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
		//std::string to_code(const std::string& name) const;
		unsigned operator()() const;

		// no equal_to overload, equality checked in parent

		// special 'constructor' that will destroy the argument (for efficiency reasons)
		static Code adopt(CodeArray &vec)  // beware, destructs vec!
		{
			CodeList* lst = new CodeList;
			lst->_stack.swap(vec); // destructs vec
			lst->calc_sizes();
			return Code(lst);
		}
	};

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

	//class CodeFactory
	//{
	//	std::forward_list <CodeList*> _codeList;

	//public:
	//	CodeList* newCodeList(const CodeArray &stack);
	//	CodeList* newCodeList(const CodeBase& a);
	//	void registerObject(const CodeBase * code);
	//};
}