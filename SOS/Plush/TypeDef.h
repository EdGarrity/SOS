#pragma once
#include "Type.h"

namespace Plush
{
	enum {
		EXEC_STACK = 0,
		INTEGER_STACK = 1,
		CODE_STACK = 2,
		BOOL_STACK = 3,
		FLOAT_STACK = 4,
		UNKNOWN = 5
	};

	template <typename T>  inline size_t get_stack_num() { /* typename_t_has_no_stack(); */ return UNKNOWN; }

	template <> inline size_t get_stack_num<ExecAtom>() { return EXEC_STACK; }
	template <> inline size_t get_stack_num<long>() { return INTEGER_STACK; }
	template <> inline size_t get_stack_num<CodeAtom>() { return CODE_STACK; }
	template <> inline size_t get_stack_num<bool>() { return BOOL_STACK; }
	template <> inline size_t get_stack_num<double>() { return FLOAT_STACK; }

	template <typename T>
	inline Type make_type() 
	{
		int n = get_stack_num<T>();
		Type type(n + 1, n);
		return type;
	}

	template <typename T, typename U> 
	inline Type make_type() 
	{
		return make_type<T>() + make_type<U>();
	}

	template <typename T1, typename T2, typename T3> 
	inline Type make_type() 
	{
		return make_type<T1>() + make_type<T2, T3>();
	}
}