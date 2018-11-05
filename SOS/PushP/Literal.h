#pragma once

#include "TypedAtom.h"
//#include "Word.h"
#include "Env.h"
#include "..\Utilities\MyException.h"

namespace Push
{
	//class Env;

	//extern Type nullType;

	//template <typename T> inline
	//	const Type &get_type();

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

		const Type & get_precondition() const
		{
			return nullType;
		}
		const Type & get_postcondition() const
		{
			return get_type<T>();
		}

		Literal(T val) : value(val) {}

		unsigned operator()() const
		{
			push<T>(value);
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

	//template <class T>
	//class LiteralFactory
	//{
	//	std::forward_list <Literal<T>*> _list;

	//public:
	//	Literal<T>* newLiteral(T val);
	//};

	//template<class T>
	//inline Literal<T> * LiteralFactory<T>::newLiteral(T val)
	//{
	//	Literal<T>* lp = new Literal<T>(val);
	//	_list.emplace_front(lp);
	//	return lp;
	//}

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

		void reset()
		{
			_head = nullptr;
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

		void reset()
		{
			literalRegister.reset();
		}

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

	template <class T>
	extern thread_local LiteralFactory<T> literalFactory;
	extern thread_local LiteralFactory<int> intLiteralFactory;
	extern thread_local LiteralFactory<double> floatLiteralFactory;
	extern thread_local LiteralFactory<bool> boolLiteralFactory;

	/* Packs a single type in a piece of code */
	template <class T>
	inline Code pack()
	{
		return Code
		(
			new Literal<T>
			(
				pop<T>(env)
				)
		);


		//T a = pop<T>(env);

		//if (typeid(a) == typeid(int))
		//	return Code(intLiteralFactory.createLiteral(a));

		//else if (typeid(a) == typeid(double))
		//	return Code(floatLiteralFactory.createLiteral(a));

		//else if (typeid(a) == typeid(bool))
		//	return Code(boolLiteralFactory.createLiteral(a));

		//else
		//	return nil;
	}

	/* Specialization for Code, just return it */
	template <>
	inline Code pack<Code>()
	{
		return pop<Code>(env);
	}

	/* Specialization for Exec, return the code */
	template <>
	inline Code pack<Exec>()
	{
		return pop<Exec>(env).to_CodeBase();
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
	Code pack(const Type & type);
}