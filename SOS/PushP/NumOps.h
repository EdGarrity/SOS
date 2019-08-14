#pragma once
#include <iostream>
#include <exception>
#include "Env.h"

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
