#pragma once

#include "Code.h"
#include "Type.h"

namespace Push
{
	template <typename T>  inline size_t get_stack_num()
	{
		/* typename_t_has_no_stack(); */
	}

	enum
	{
		EXEC_STACK = 0,
		INTEGER_STACK = 1,
		CODE_STACK = 2,
		BOOL_STACK = 3,
		FLOAT_STACK = 4,
	};

	extern Type nullType;
	extern Type execType;
	extern Type integerType;
	extern Type codeType;
	extern Type boolType;
	extern Type floatType;
//	extern Type nameType;
	extern Type typeType;

	template <> inline size_t get_stack_num<Exec>()
	{
		return EXEC_STACK;
	}
	template <> inline size_t get_stack_num<int>()
	{
		return INTEGER_STACK;
	}
	template <> inline size_t get_stack_num<Code>()
	{
		return CODE_STACK;
	}
	template <> inline size_t get_stack_num<bool>()
	{
		return BOOL_STACK;
	}
	template <> inline size_t get_stack_num<double>()
	{
		return FLOAT_STACK;
	}

	template <typename T> inline
		Type make_type()
	{
		size_t n = get_stack_num<T>();
		Type type(n + 1, n);
		return type;
	}

	template <typename T, typename U> inline
		Type make_type()
	{
		return make_type<T>() + make_type<U>();
	}

	template <typename T1, typename T2, typename T3> inline
		Type make_type()
	{
		return make_type<T1>() + make_type<T2, T3>();
	}

	template <typename T1, typename T2, typename T3, typename T4>  inline
		Type make_type()
	{
		return make_type<T1, T2>() + make_type<T3, T4>();
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5>  inline
		Type make_type()
	{
		return make_type<T1, T2>() + make_type<T3, T4, T5>();
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>  inline
		Type make_type()
	{
		return make_type<T1, T2, T3>() + make_type<T4, T5, T6>();
	}

	template <typename T> inline
		const Type &get_type()
	{
		static Type static_type = make_type<T>();
		return static_type;
	}
}