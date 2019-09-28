#pragma once
#include <iostream>
#include <exception>
#include "Env.h"
#include <cstdlib>
#include <cmath>

namespace Push
{
	// plus, minus, multiplies, divides
	template <class T>
	inline unsigned plus()
	{
		//if (not has_elements<T>(env,2)) return 1;
		T first = pop<T>(env);
		get_stack<T>().back() += first;
		return 1;
	}

	template <class T>
	inline unsigned minus()
	{
		//if (not has_elements<T>(env,2)) return 1;
		T first = pop<T>(env);
		get_stack<T>().back() -= first;
		return 1;
	}

	template <class T>
	inline unsigned multiplies()
	{
		//if (not has_elements<T>(env,2)) return 1;
		T first = pop<T>(env);
		get_stack<T>().back() *= first;
		return 1;
	}

	template <class T>
	inline unsigned divides()
	{
		if ((has_elements<T>(2)) == false) return 1;	//Was commented out in original code.  I uncommented after encountering an exception which appeared to be caused by not having enough data on the stack.
		
		//if ( get_stack<T>( env ).back() == T() )
		//{
		//	env.error();
		//	return 1;
		//}

		//T first = pop<T>( env );
		//get_stack<T>( env ).back() /= first;
		//return 1;

		//T a;
		//if (typeid(a) == typeid(int))
		//	int a = 0;

		//bool hasTwo = has_elements<T>(2);

		//if (hasTwo == false) 
		//	return 1;	

		//auto temp = get_stack<T>().back();
		//auto temp2 = T();

		//if (get_stack<T>().back() == T())
		//{
		//	env.error();
		//	return 1;
		//}

		//T first = pop<T>();
		//T second = get_stack<T>().back();

		//get_stack<T>().back() /= first;
		//return 1;


		if (get_stack<T>().back() == T())
		{
//			env.error();
			return 1;
		}

		T first = pop<T>(env);

		T a;
		if (typeid(a) == typeid(int))
		{
			T second = get_stack<T>().back();
			if (second == INT_MIN)
			{
//				env.error();
				return 1;
			}
		}

		get_stack<T>().back() /= first;
		return 1;
	}

	template <class From, class To>
	inline unsigned push_cast()
	{
		//if (not has_elements<From>(env,1)) return 1;
		push<To>(static_cast<To>(pop<From>(env)));
		return 1;
	}

	inline unsigned int2bool()
	{
		return push_cast<int, bool>();
	}
	inline unsigned int2float()
	{
		return push_cast<int, double>();
	}

	inline unsigned bool2int()
	{
		return push_cast<bool, int>();
	}
	inline unsigned bool2float()
	{
		return push_cast<bool, double>();
	}

	inline unsigned float2int()
	{
		return push_cast<double, int>();
	}
	inline unsigned float2bool()
	{
		return push_cast<double, bool>();
	}

	//// Pushes the data item from the current data record.  The data item is 
	//// indexed by the top item of the INTEGER stack. The integer used as the index 
	//// is taken modulo the number of data items in the current data record.  A 
	//// positive index refers to the data value.  A negative index refers to the 
	//// first derivative of the data item.
	//inline unsigned data2float()
	//{
	//	double value = 0.0;

	//	int column_index = pop<int>(env);

	//	value = env.parameters.pBroker->get_value_from_datatable(env.data_record_index, column_index);
	//	push<double>(value);

	//	return 1;
	//}


	// Purpose: 
	//   Push the Nth element from the input array onto the Float stack.
	//
	// Parameters:
	//   None
	//
	// Return value:
	//   1
	//
	// Side Effects:
	//   If successful, a number is pushed to the Float stack
	//
	// Thread Safe:
	//   Yes
	//
	// Remarks:
	//   N is popped from the Integer stack. If N < 0, or if N >= size of input array, the mod of N is used.
	//
	inline unsigned input2float()
	{
		if (env.input.size() > 0)
		{
			int index = pop<int>(env);

			index = std::abs((int)(index % env.input.size()));
			double value = env.input[index];
			push<double>(value);
		}

		return 1;
	}

	// Purpose: 
	//   Pop the top element from the Float stack and copy it to the Nth element of the output array.
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
	//   N is popped from the Integer stack. If N < 0, the absolute value is used.  If the Integer 
	//   stack is empty then a NO-OP is executed instead. If N >= the size of the output array then 
	//   the number is copied to the element after the last element of the output array.
	//
	inline unsigned float2output()
	{
		int index = std::abs((int)pop<int>(env));
		double value = pop<double>(env);

		if (std::isnan(value))
			return 1;

		if (index < env.output.size())
			env.output[index] = value;

		else
			env.output.push_back(value);

		return 1;
	}

	// Purpose: 
	//   Push the Nth element from the input array onto the Integer stack.
	//
	// Parameters:
	//   None
	//
	// Return value:
	//   1
	//
	// Side Effects:
	//   If successful, a number is pushed to the Integer stack
	//
	// Thread Safe:
	//   Yes
	//
	// Remarks:
	//   N is popped from the Integer stack. If N < 0, or if N >= size of input array, the mod of N is used.
	//
	inline unsigned input2int()
	{
		if (env.input.size() > 0)
		{
			int index = pop<int>(env);

			index = std::abs((int)(index % env.input.size()));
			int value = env.input[index];
			push<int>(value);
		}

		return 1;
	}

	// Purpose: 
	//   Pop the top element from the Integer stack and copy it to the Nth element of the output array.
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
	inline unsigned int2output()
	{
		int value = pop<int>(env);
		int index = std::abs((int)pop<int>(env));

		if (index < env.output.size())
			env.output[index] = value;

		else
			env.output.push_back(value);

		return 1;
	}

	// Purpose: 
	//   Push the Nth element from the input array onto the Boolean stack.
	//
	// Parameters:
	//   None
	//
	// Return value:
	//   1
	//
	// Side Effects:
	//   If successful, a number is pushed to the Boolean stack
	//
	// Thread Safe:
	//   Yes
	//
	// Remarks:
	//   N is popped from the Integer stack. If N < 0, or if N >= size of input array, the mod of N is used.
	//
	inline unsigned input2bool()
	{
		if (env.input.size() > 0)
		{
			int index = pop<int>(env);

			index = std::abs((int)(index % env.input.size()));

			if (env.input[index] > 0.0)
				push<bool>(true);

			else
				push<bool>(false);
		}

		return 1;
	}

	// Purpose: 
	//   Pop the top element from the Boolean stack and copy it to the Nth element of the output array.
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
	//   N is popped from the Integer stack. If N < 0, the absolute value is used.  If the Integer 
	//   stack is empty then a NO-OP is executed instead. If N >= the size of the output array then 
	//   the number is copied to the element after the last element of the output array.
	//
	inline unsigned bool2output()
	{
		int index = std::abs((int)pop<int>(env));
		double value = pop<bool>(env) ? 1.0 : 0.0;

		if (index < env.output.size())
			env.output[index] = value;

		else
			env.output.push_back(value);

		return 1;
	}

	inline unsigned _exp()
	{
		std::vector<double> &stack = get_stack<double>();
		stack.back() = std::exp(stack.back());
		return 1;
	}

	inline unsigned _log()
	{
		std::vector<double> &stack = get_stack<double>();
		stack.back() = std::log(stack.back());
		return 1;
	}

	inline unsigned _fmod()
	{
		//if (not has_elements<double>(env,2)) return 1;
		if (get_stack<double>().back() == 0.0)
		{
//			env.error();
			return 1;
		}

		double first = pop<double>(env);
		double second = pop<double>(env);
		push<double>(fmod(second, first));
		return 1;
	}

	inline unsigned int_mod()
	{
		//if (not has_elements<int>(env,2)) return 1;
		if (get_stack<int>().back() == 0)
		{
//			env.error();
			return 1;
		}

		int first = pop<int>(env);
		int second = get_stack<int>().back();
		
		// Check for integer overflow
		if ((first != -1) || (second != INT_MIN))
			second %= first;

		return 1;
	}

	template <class T>
	inline unsigned smaller()
	{
		//if (not has_elements<T>(env,2)) return 1;
		T first = pop<T>(env);
		T second = pop<T>(env);
		push<bool>(second < first);
		return 1;
	}

	template <class T>
	inline unsigned greater()
	{
		//if (not has_elements<T>(env,2)) return 1;
		T first = pop<T>(env);
		T second = pop<T>(env);
		push<bool>(second > first);
		return 1;
	}

	template <class T>
	inline unsigned _max()
	{
		//if (not has_elements<T>(env,2)) return 1;
		T first = pop<T>(env);
		T second = pop<T>(env);
		push<T>(std::max(first, second));
		return 1;
	}

	template <class T>
	inline unsigned _min()
	{
		//if (not has_elements<T>(env,2)) return 1;
		T first = pop<T>(env);
		T second = pop<T>(env);
		push<T>(std::min(first, second));
		return 1;
	}

	inline unsigned _cos()
	{
		//if (not has_elements<double>(env,1)) return 1;
		std::vector<double> &stack = get_stack<double>();
		stack.back() = cos(stack.back());
		return 1;
	}
	inline unsigned _sin()
	{
		//if (not has_elements<double>(env,1)) return 1;
		std::vector<double> &stack = get_stack<double>();
		stack.back() = sin(stack.back());
		return 1;
	}
	inline unsigned _tan()
	{
		//if (not has_elements<double>(env,1)) return 1;
		std::vector<double> &stack = get_stack<double>();
		stack.back() = tan(stack.back()); // hmm, can go wrong, math error
		return 1;
	}
}
