#pragma once

#include "TypedAtom.h"
#include "Env.h"
#include "..\Utilities\MyException.h"
#include <ppl.h>

using namespace concurrency;

namespace Push
{
	template <class T>
	inline bool does_equal(const T & a, const T & b)
	{
		return a == b;
	}

	template <class T>
	class Literal : public TypedAtom
	{
		T value;
	public:

		Literal(T val) : value(val) {}

		unsigned operator()(Env & _env) const
		{
			push<T>(_env, value);
			return 1;
		}

		std::string to_string() const
		{
			std::ostringstream os;
			os.precision(17);
			os.setf(std::ios_base::showpoint);
			os << value;
			return os.str();
		}

		std::string to_code(const std::string& _name) const
		{
			std::string codeString = "";

			if (typeid(value) == typeid(bool))
				codeString = "( " + _name + " " + to_string() + " BOOLEAN.DEFINE  )";

			else if (typeid(value) == typeid(int))
				codeString = "( " + _name + " " + to_string() + " INTEGER.DEFINE  )";

			else if (typeid(value) == typeid(double))
				codeString = "( " + _name + " " + to_string() + " FLOAT.DEFINE  )";

			else
			{
				std::stringstream errorMessage;

				errorMessage << "Error to_code";

				throw MyException(errorMessage);
			}

			return codeString;
		}

		TYPE_ID get_type2()
		{
			if (typeid(value) == typeid(bool))
				return TYPE_ID::boolean;

			else if (typeid(value) == typeid(int))
				return TYPE_ID::integer;

			else if (typeid(value) == typeid(double))
				return TYPE_ID::floating_point;

			else
				return TYPE_ID::unknown;
		}

		T get() const
		{
			return value;
		}

		bool equal_to(const CodeBase & other) const
		{
			return does_equal(value, static_cast<const Literal<T>&>(other).value);
		}
	};

	// specialization for bool
	template <>
	inline std::string Literal<bool>::to_string() const
	{
		return value ? "TRUE" : "FALSE";
	}

	//
	// Literal memory manager
	//
	template<class T>
	class LiteralRegisterNode
	{
		Literal<T>* _p;
		LiteralRegisterNode* _next;

		template<class T>
		friend class LiteralRegister;

	public:
		LiteralRegisterNode(Literal<T> *p_, LiteralRegisterNode* next_)
		{
			_p = p_;
			_next = next_;
		}
	};

	template<class T>
	class LiteralRegister
	{
		LiteralRegisterNode<T>* _head;

	public:
		LiteralRegister() : _head(nullptr) {}

		~LiteralRegister()
		{
			clean_up();
		}

		void record(Literal<T>* p)
		{
			LiteralRegisterNode<T>* node = new LiteralRegisterNode<T>(p, _head);
			_head = node;
		}

		void clean_up()
		{
			LiteralRegisterNode<T>* current_node = _head;

			while (current_node != nullptr)
			{
				LiteralRegisterNode<T>* next_node = current_node->_next;

				delete current_node->_p;
				current_node->_p = nullptr;

				delete current_node;
				current_node = nullptr;

				current_node = next_node;
			}

			_head = nullptr;
		}
	};

	template <class T>
	class LiteralFactory
	{
		LiteralRegister<T> literalRegister;

	public:
		Literal<T>* createLiteral(T val);

		void clean_up()
		{
			literalRegister.clean_up();
		}
	};

	template<class T>
	inline Literal<T> * LiteralFactory<T>::createLiteral(T val)
	{
		Literal<T>* lp = new Literal<T>(val);
		literalRegister.record(lp);
		return lp;
	}

	extern combinable<LiteralFactory<int>> parallel_intLiteralFactory;
	extern combinable<LiteralFactory<double>> parallel_floatLiteralFactory;
	extern combinable<LiteralFactory<bool>> parallel_boolLiteralFactory;

	/* Packs a single type in a piece of code */
	template <class T>
	inline Code pack(Env & _env)
	{
		T a = pop<T>(_env);

		if (typeid(a) == typeid(int))
			return Code(parallel_intLiteralFactory.local().createLiteral(a));

		else if (typeid(a) == typeid(double))
			return Code(parallel_floatLiteralFactory.local().createLiteral(a));

		else if (typeid(a) == typeid(bool))
			return Code(parallel_boolLiteralFactory.local().createLiteral(a));

		return Code();
	}

	/* Specialization for Code, just return it */
	template <>
	inline Code pack<Code>(Env & _env)
	{
		return pop<Code>(_env);
	}

	/* Specialization for Exec, return the code */
	template <>
	inline Code pack<Exec>(Env & _env)
	{
		return pop<Exec>(_env).to_CodeBase();
	}

	/* new untyped version of Set, called DEFINE: NAME.DEFINE, INTEGER.DEFINE, etc.*/
	//template <class T>
	//inline unsigned name_define(Env & env)
	//{
	//	static Type type = make_type<T, name_t>(); // also takes care of <name_t,name_t>

	//	if (!type.can_pop_from(env)) return 1;

	//	name_t name = pop<name_t>(env);
	//	Code code = pack<T>(env);
	//	set_code(name, code);
	//	return 1;
	//}

	//extern unsigned edefine(Env & env);
	//template <>
	//inline unsigned name_define<Exec>(Env & env)
	//{
	//	return edefine(env);
	//}

	/* Typed version */
	Code pack(Env & _env, const Type & type);
}