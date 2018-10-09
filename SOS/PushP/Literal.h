#pragma once

#include "TypedAtom.h"
//#include "Word.h"
#include "Env.h"
#include "..\Utilities\MyException.h"

namespace push
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
		return pop<Exec>(env).lock();
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