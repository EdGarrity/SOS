#pragma once
#include <algorithm>
#include "Processor.h"

namespace Plush
{
	template <class From, class To>
	inline unsigned push_cast(Environment & _env)
	{
		if (_env.has_elements<From>(1))
			_env.push<To>(static_cast<To>(_env.pop<From>()));

		return 1;
	}

	inline unsigned int2bool(Environment & _env)
	{
		return push_cast<long, bool>(_env);
	}

	inline unsigned int2float(Environment & _env)
	{
		return push_cast<long, double>(_env);
	}

	inline unsigned bool2int(Environment & _env)
	{
		return push_cast<bool, long>(_env);
	}

	inline unsigned bool2float(Environment & _env)
	{
		return push_cast<bool, double>(_env);
	}

	inline unsigned float2int(Environment & _env)
	{
		return push_cast<double, long>(_env);
	}

	inline unsigned float2bool(Environment & _env)
	{
		return push_cast<double, bool>(_env);
	}

	// plus, minus, multiplies, divides
	template <class T>
	inline unsigned plus(Environment & _env)
	{
		if (_env.has_elements<T>(2))
		{
			double first = 0.0;
			double second = 0.0;
			double result = 0.0;

			if (typeid(T) == typeid(double))
			{
				first = _env.get_stack<double>().get_top();
				second = _env.get_stack<double>()[_env.get_stack<double>().size() - 2];

				result = second + first;

				if ((!std::isnan(result)) && (!std::isinf(result)))
				{
					T another_first = _env.pop<T>();
					_env.get_stack<T>().get_top_ref() += another_first;
				}
			}

			else
			{
				T another_first = _env.pop<T>();
				_env.get_stack<T>().get_top_ref() += another_first;
			}
		}

		return 1;
	}

	template <class T>
	inline unsigned minus(Environment & _env)
	{
		if (_env.has_elements<T>(2))
		{
			double first = 0.0;
			double second = 0.0;
			double result = 0.0;

			if (typeid(T) == typeid(double))
			{
				first = _env.get_stack<double>().get_top();
				second = _env.get_stack<double>()[_env.get_stack<double>().size() - 2];

				result = second - first;

				if ((!std::isnan(result)) && (!std::isinf(result)))
				{
					T another_first = _env.pop<T>();
					_env.get_stack<T>().get_top_ref() -= another_first;
				}
			}

			else
			{
				T another_first = _env.pop<T>();
				_env.get_stack<T>().get_top_ref() -= another_first;
			}
		}

		return 1;
	}

	template <class T>
	inline unsigned multiplies(Environment & _env)
	{
		if (_env.has_elements<T>(2))
		{
			double first = 0.0;
			double second = 0.0;
			double result = 0.0;

			if (typeid(T) == typeid(double))
			{
				first = _env.get_stack<double>().get_top();
				second = _env.get_stack<double>()[_env.get_stack<double>().size() - 2];

				result = second * first;

				if ((!std::isnan(result)) && (!std::isinf(result)))
				{
					T another_first = _env.pop<T>();
					_env.get_stack<T>().get_top_ref() *= another_first;
				}
			}

			else
			{
				T another_first = _env.pop<T>();
				_env.get_stack<T>().get_top_ref() *= another_first;
			}
		}

		return 1;
	}

	template <class T>
	inline unsigned divides(Environment & _env)
	{
		if (_env.has_elements<T>(2))
		{
			if (_env.get_stack<T>().get_top_ref() == T())
				return 1;

			T first = _env.pop<T>();

			T a;
			if (typeid(a) == typeid(int))
			{
				T second = _env.get_stack<T>().get_top();

				if (second == INT_MIN)
					return 1;
			}

			_env.get_stack<T>().get_top_ref() /= first;
		}

		return 1;
	}

	template <class T>
	inline unsigned smaller(Environment & _env)
	{
		if (_env.has_elements<T>(2))
		{
			T first = _env.pop<T>();
			T second = _env.pop<T>();
			_env.push<bool>(second < first);
		}

		return 1;
	}

	template <class T>
	inline unsigned greater(Environment & _env)
	{
		if (_env.has_elements<T>(2))
		{
			T first = _env.pop<T>();
			T second = _env.pop<T>();
			_env.push<bool>(second > first);
		}

		return 1;
	}

	template <class T>
	inline unsigned _max(Environment & _env)
	{
		if (_env.has_elements<T>(2))
		{
			T first = _env.pop<T>();
			T second = _env.pop<T>();
			_env.push<T>(std::max(first, second));
		}

		return 1;
	}

	template <class T>
	inline unsigned _min(Environment & _env)
	{
		if (_env.has_elements<T>(2))
		{
			T first = _env.pop<T>();
			T second = _env.pop<T>();
			_env.push<T>(std::min(first, second));
		}

		return 1;
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
	inline unsigned in(Environment & _env)
	{
		if (_env.input.size() > 0)
		{
			int index = _env.pop<long>();

			index = std::abs((int)(index % _env.input.size()));
			T value = _env.input[index];
			_env.push<T>(value);
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
	//		int index = pop<long>(env);

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
	//		int index = pop<long>(env);

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
	inline unsigned inall(Environment & _env)
	{
		if (_env.input.size() > 0)
		{
			for (int index = 0; index < _env.input.size(); index++)
			{
				T value = _env.input[index];
				_env.push<T>(value);
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
	inline unsigned inallrev(Environment & _env)
	{
		if (_env.input.size() > 0)
		{
			for (int index = _env.input.size() - 1; index >= 0; index--)
			{
				T value = _env.input[index];
				_env.push<T>(value);
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
	inline unsigned out(Environment & _env)
	{
		if ((_env.has_elements<T>(1)) && (_env.has_elements<long>(1)))
		{
			T value = _env.pop<T>();
			int index = std::abs((int)_env.pop<long>());

			if (index < _env.output.size())
				_env.output[index] = value;

			else
				_env.output.push_back(value);
		}

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
	//	int index = std::abs((int)pop<long>(env));
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
	//	int index = std::abs((int)pop<long>(env));
	//	double value = pop<bool>(env) ? 1.0 : 0.0;

	//	if (index < env.output.size())
	//		env.output[index] = value;

	//	else
	//		env.output.push_back(value);

	//	return 1;
	//}
}