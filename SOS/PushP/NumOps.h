#pragma once
#include <iostream>
#include <exception>
#include "Env.h"
#include <cstdlib>
#include <cmath>
#include <SafeInt.h>

namespace Push
{
	// plus, minus, multiplies, divides
	template <class T>
	inline unsigned plus(Env & _env)
	{
		double first = 0.0;
		double second = 0.0;
		double result = 0.0;

		if (typeid(T) == typeid(double))
		{
			first = get_stack<double>(_env).back();
			second = get_stack<double>(_env)[get_stack<double>(_env).size() - 2];

			result = second + first;

			if ((!std::isnan(result)) && (!std::isinf(result)))
			{
				T another_first = pop<T>(_env);
				get_stack<T>(_env).back() += another_first;
			}
		}

		else
		{
			T another_first = pop<T>(_env);
			get_stack<T>(_env).back() += another_first;
		}

		return 1;
	}

	template <class T>
	inline unsigned minus(Env & _env)
	{
		double first = 0.0;
		double second = 0.0;
		double result = 0.0;

		if (typeid(T) == typeid(double))
		{
			first = get_stack<double>(_env).back();
			second = get_stack<double>(_env)[get_stack<double>(_env).size() - 2];

			result = second - first;

			if ((!std::isnan(result)) && (!std::isinf(result)))
			{
				T another_first = pop<T>(_env);
				get_stack<T>(_env).back() -= another_first;
			}
		}

		else
		{
			T another_first = pop<T>(_env);
			get_stack<T>(_env).back() -= another_first;
		}

		return 1;
	}

	template <class T>
	inline unsigned multiplies(Env & _env)
	{
		double first = 0.0;
		double second = 0.0;
		double result = 0.0;

		if (typeid(T) == typeid(double))
		{
			first = get_stack<double>(_env).back();
			second = get_stack<double>(_env)[get_stack<double>(_env).size() - 2];

			result = second * first;

			if ((!std::isnan(result)) && (!std::isinf(result)))
			{
				T another_first = pop<T>(_env);
				get_stack<T>(_env).back() *= another_first;
			}
		}

		else
		{
			T another_first = pop<T>(_env);
			get_stack<T>(_env).back() *= another_first;
		}

		return 1;
	}

	template <class T>
	inline unsigned divides(Env & _env)
	{
		if ((has_elements<T>(_env, 2)) == false) return 1;	//Was commented out in original code.  I uncommented after encountering an exception which appeared to be caused by not having enough data on the stack.
		
		if (get_stack<T>(_env).back() == T())
			return 1;

		T first = pop<T>(_env);

		T a;
		if (typeid(a) == typeid(int))
		{
			T second = get_stack<T>(_env).back();

			if (second == INT_MIN)
				return 1;
		}

		get_stack<T>(_env).back() /= first;

		return 1;
	}

	template <class From, class To>
	inline unsigned push_cast(Env & _env)
	{
		push<To>(_env, static_cast<To>(pop<From>(_env)));

		return 1;
	}

	inline unsigned int2bool(Env & _env)
	{
		return push_cast<int, bool>(_env);
	}

	inline unsigned int2float(Env & _env)
	{
		return push_cast<int, double>(_env);
	}

	inline unsigned bool2int(Env & _env)
	{
		return push_cast<bool, int>(_env);
	}

	inline unsigned bool2float(Env & _env)
	{
		return push_cast<bool, double>(_env);
	}

	inline unsigned float2int(Env & _env)
	{
		return push_cast<double, int>(_env);
	}

	inline unsigned float2bool(Env & _env)
	{
		return push_cast<double, bool>(_env);
	}

	// Purpose: 
	//   Push the Nth element from the input array onto a stack.
	//
	// Parameters:
	//   None
	//
	// Return value:
	//   1
	//
	// Side Effects:
	//   If successful, a number is pushed to a stack
	//
	// Thread Safe:
	//   Yes
	//
	// Remarks:
	//   N is popped from the Integer stack. If N < 0, or if N >= size of input array, the mod of N is used.
	//
	template <class T>
	inline unsigned in(Env & _env)
	{
		if (_env.local().input.size() > 0)
		{
			int index = pop<int>(_env);

			index = std::abs((int)(index % _env.local().input.size()));
			T value = _env.local().input[index];
			push<T>(_env, value);
		}

		return 1;
	}

	//// Purpose: 
	////   Push the Nth element from the input array onto the Float stack.
	////
	//// Parameters:
	////   None
	////
	//// Return value:
	////   1
	////
	//// Side Effects:
	////   If successful, a number is pushed to the Float stack
	////
	//// Thread Safe:
	////   Yes
	////
	//// Remarks:
	////   N is popped from the Integer stack. If N < 0, or if N >= size of input array, the mod of N is used.
	////
	//inline unsigned input2float()
	//{
	//	if (env.input.size() > 0)
	//	{
	//		int index = pop<int>(env);

	//		index = std::abs((int)(index % env.input.size()));
	//		double value = env.input[index];
	//		push<double>(value);
	//	}

	//	return 1;
	//}

	//// Purpose: 
	////   Push the Nth element from the input array onto the Boolean stack.
	////
	//// Parameters:
	////   None
	////
	//// Return value:
	////   1
	////
	//// Side Effects:
	////   If successful, a number is pushed to the Boolean stack
	////
	//// Thread Safe:
	////   Yes
	////
	//// Remarks:
	////   N is popped from the Integer stack. If N < 0, or if N >= size of input array, the mod of N is used.
	////
	//inline unsigned input2bool()
	//{
	//	if (env.input.size() > 0)
	//	{
	//		int index = pop<int>(env);

	//		index = std::abs((int)(index % env.input.size()));

	//		if (env.input[index] > 0.0)
	//			push<bool>(true);

	//		else
	//			push<bool>(false);
	//	}

	//	return 1;
	//}

	// Purpose: 
	//   Push all elements from the input array onto a stack.
	//
	// Parameters:
	//   None
	//
	// Return value:
	//   1
	//
	// Side Effects:
	//   If successful, all numbers are pushed to the a stack
	//
	// Thread Safe:
	//   Yes
	//
	// Remarks:
	//   if input array is empty, NO-OP is executed.
	//
	template <class T>
	inline unsigned inall(Env & _env)
	{
		if (_env.local().input.size() > 0)
		{
			for (int index = 0; index < _env.local().input.size(); index++)
			{
				T value = _env.local().input[index];
				push<T>(_env, value);
			}
		}

		return 1;
	}

	// Purpose: 
	//   Push all elements from the input array onto a stack in reverse order.
	//
	// Parameters:
	//   None
	//
	// Return value:
	//   1
	//
	// Side Effects:
	//   If successful, all numbers are pushed to a stack
	//
	// Thread Safe:
	//   Yes
	//
	// Remarks:
	//   if input array is empty, NO-OP is executed.
	//
	template <class T>
	inline unsigned inallrev(Env & _env)
	{
		if (_env.local().input.size() > 0)
		{
			for (int index = _env.local().input.size() - 1; index >= 0; index--)
			{
				T value = _env.local().input[index];
				push<T>(_env, value);
			}
		}

		return 1;
	}

	// Purpose: 
	//   Pop the top element from a stack and copy it to the Nth element of the output array.
	//
	// Parameters:
	//   None
	//
	// Return value:
	//   1
	//
	// Side Effects:
	//   If successful, a number is pushed to the Output array
	//
	// Thread Safe:
	//   Yes
	//
	// Remarks:
	//   N is popped from the Integer stack after the value to be copied to the output array is poped 
	//   from the Integer stack. If N < 0, the absolute value is used.  If the Integer stack is empty 
	//   then a NO-OP is executed instead. If N >= the size of the output array then the number is 
	//   copied to the element after the last element of the output array.
	//
	template <class T>
	inline unsigned out(Env & _env)
	{
		T value = pop<T>(_env);
		int index = std::abs((int)pop<int>(_env));

		if (index < _env.local().output.size())
			_env.local().output[index] = value;

		else
			_env.local().output.push_back(value);

		return 1;
	}

	//// Purpose: 
	////   Pop the top element from the Float stack and copy it to the Nth element of the output array.
	////
	//// Parameters:
	////   None
	////
	//// Return value:
	////   1
	////
	//// Side Effects:
	////   If successful, a number is pushed to the Output array
	////
	//// Thread Safe:
	////   Yes
	////
	//// Remarks:
	////   N is popped from the Integer stack. If N < 0, the absolute value is used.  If the Integer 
	////   stack is empty then a NO-OP is executed instead. If N >= the size of the output array then 
	////   the number is copied to the element after the last element of the output array.
	////
	//inline unsigned float2output()
	//{
	//	int index = std::abs((int)pop<int>(env));
	//	double value = pop<double>(env);

	//	if (std::isnan(value))
	//		return 1;

	//	if (index < env.output.size())
	//		env.output[index] = value;

	//	else
	//		env.output.push_back(value);

	//	return 1;
	//}

	//// Purpose: 
	////   Pop the top element from the Boolean stack and copy it to the Nth element of the output array.
	////
	//// Parameters:
	////   None
	////
	//// Return value:
	////   1
	////
	//// Side Effects:
	////   If successful, a number is pushed to the Output array
	////
	//// Thread Safe:
	////   Yes
	////
	//// Remarks:
	////   N is popped from the Integer stack. If N < 0, the absolute value is used.  If the Integer 
	////   stack is empty then a NO-OP is executed instead. If N >= the size of the output array then 
	////   the number is copied to the element after the last element of the output array.
	////
	//inline unsigned bool2output()
	//{
	//	int index = std::abs((int)pop<int>(env));
	//	double value = pop<bool>(env) ? 1.0 : 0.0;

	//	if (index < env.output.size())
	//		env.output[index] = value;

	//	else
	//		env.output.push_back(value);

	//	return 1;
	//}

	inline unsigned _exp(Env & _env)
	{
		std::vector<double> &stack = get_stack<double>(_env);
		stack.back() = std::exp(stack.back());

		return 1;
	}

	inline unsigned _log(Env & _env)
	{
		std::vector<double> &stack = get_stack<double>(_env);
		double val = stack.back();

		if (val > 0.0)
			stack.back() = std::log(val);

		return 1;
	}

	inline unsigned _fmod(Env & _env)
	{
		if (get_stack<double>(_env).back() == 0.0)
			return 1;

		double first = pop<double>(_env);
		double second = pop<double>(_env);
		push<double>(_env, fmod(second, first));

		return 1;
	}

	inline unsigned int_mod(Env & _env)
	{
		if (get_stack<int>(_env).back() == 0)
			return 1;

		int first = pop<int>(_env);
		int second = get_stack<int>(_env).back();
		
		// Check for integer overflow
		if ((first != -1) || (second != INT_MIN))
			second %= first;

		return 1;
	}

	template <class T>
	inline unsigned smaller(Env & _env)
	{
		T first = pop<T>(_env);
		T second = pop<T>(_env);
		push<bool>(_env, second < first);

		return 1;
	}

	template <class T>
	inline unsigned greater(Env & _env)
	{
		T first = pop<T>(_env);
		T second = pop<T>(_env);
		push<bool>(_env, second > first);
		return 1;
	}

	template <class T>
	inline unsigned _max(Env & _env)
	{
		T first = pop<T>(_env);
		T second = pop<T>(_env);
		push<T>(_env, std::max(first, second));

		return 1;
	}

	template <class T>
	inline unsigned _min(Env & _env)
	{
		T first = pop<T>(_env);
		T second = pop<T>(_env);
		push<T>(_env, std::min(first, second));

		return 1;
	}

	inline unsigned _cos(Env & _env)
	{
		std::vector<double> &stack = get_stack<double>(_env);
		stack.back() = cos(stack.back());

		return 1;
	}

	inline unsigned _sin(Env & _env)
	{
		std::vector<double> &stack = get_stack<double>(_env);
		stack.back() = sin(stack.back());

		return 1;
	}

	inline unsigned _tan(Env & _env)
	{
		std::vector<double> &stack = get_stack<double>(_env);
		double val = stack.back();

		if (val != 0.0)
			stack.back() = tan(val);

		return 1;
	}
}
