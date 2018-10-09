#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <forward_list>

namespace push
{
	/*
	 * program ::= instruction | literal | ( program* )
	 *
	 */

	class Env;
	class CodeBase;

	typedef std::vector<CodeBase> CodeArray;
	typedef CodeBase Code;
	typedef unsigned(*Operator)(Env &);
	typedef std::map<std::string, push::CodeBase> String2CodeMap;
	typedef std::set<push::CodeBase> CodeSet;

	/* Globals (Check StaticInit) */
	extern const String2CodeMap &str2code_map;
	extern const CodeBase &instructions;
	extern const CodeSet &erc_set;
	extern const CodeSet &exec_set;  // DO, DO*, IF

	class CodeBase
	{
	private:
		friend class CodeList; // the only one allowed to touch the stack
		CodeArray _stack;

	public:

		CodeBase() {}
		CodeBase(const CodeArray &stack) : _stack(stack) {}
		CodeBase(const CodeBase &code) : _stack(code._stack) {}
		const CodeArray &get_stack() const
		{
			return _stack;
		}

		virtual ~CodeBase() {}

	//	bool operator==(const CodeBase &other) const;

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

	//	virtual std::string to_code(const std::string& name) const
	//	{
	//		return;
	//	}


	//protected:
	//	virtual bool equal_to(const CodeBase &other) const
	//	{
	//		return true;
	//	};

		// Comparitor fuctions required for std::set
		bool operator==(const CodeBase & o) const
		{
			return this == &o;
		}
		bool operator!=(const CodeBase & o) const
		{
			return this != &o;
		}
		bool operator<(const CodeBase & o) const
		{
			return this < &o;
		}

	};

	class ExecBase : public CodeBase
	{
	public:
		ExecBase() {}
		ExecBase(const CodeArray &stack) : CodeBase(stack) {}

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

	class CodeAtom : public ExecBase
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

	class CodeList : public ExecBase
	{
		unsigned _size;
		size_t _binary_size;

		void calc_sizes();

	public:

		CodeList() : _size(1u), _binary_size(1u) // _size( -1u ), _binary_size( -1u )
		{
			calc_sizes(); // Sets _size and _binary_size anyway.
		}
		CodeList(const CodeArray &stack) : ExecBase(stack), _size(1u), _binary_size(1u) // _size( -1u ), _binary_size( -1u )
		{
			calc_sizes();
		}
		CodeList(const CodeBase& a);

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

		//// no equal_to overload, equality checked in parent

		//// special 'constructor' that will destroy the argument (for efficiency reasons)
		//static Code adopt(CodeArray &vec)  // beware, destructs vec!
		//{
		//	CodeList* lst = new CodeList;
		//	lst->_stack.swap(vec); // destructs vec
		//	lst->calc_sizes();
		//	return Code(lst);
		//}

		//void shrink_to_fit();
	};

	inline std::string str(CodeBase code)
	{
		if (code.get_stack().empty()) return "no code";

		return code.to_string();
	}

	class CodeFactory
	{
		std::forward_list <CodeList*> _codeList;

	public:
		CodeList* newCodeList(const CodeArray &stack);
		CodeList* newCodeList(const CodeBase& a);
	};
}