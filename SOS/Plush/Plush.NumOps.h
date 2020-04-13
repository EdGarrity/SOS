#pragma once
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
				first = _env.get_stack<double>().top();
				second = _env.get_stack<double>()[_env.get_stack<double>().size() - 2];

				result = second + first;

				if ((!std::isnan(result)) && (!std::isinf(result)))
				{
					T another_first = _env.pop<T>();
					_env.get_stack<T>().top() += another_first;
				}
			}

			else
			{
				T another_first = _env.pop<T>();
				_env.get_stack<T>().top() += another_first;
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
				first = _env.get_stack<double>().top();
				second = _env.get_stack<double>()[_env.get_stack<double>().size() - 2];

				result = second - first;

				if ((!std::isnan(result)) && (!std::isinf(result)))
				{
					T another_first = _env.pop<T>();
					_env.get_stack<T>().top() -= another_first;
				}
			}

			else
			{
				T another_first = _env.pop<T>();
				_env.get_stack<T>().top() -= another_first;
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
				first = _env.get_stack<double>().top();
				second = _env.get_stack<double>()[_env.get_stack<double>().size() - 2];

				result = second * first;

				if ((!std::isnan(result)) && (!std::isinf(result)))
				{
					T another_first = _env.pop<T>();
					_env.get_stack<T>().top() *= another_first;
				}
			}

			else
			{
				T another_first = _env.pop<T>();
				_env.get_stack<T>().top() *= another_first;
			}
		}

		return 1;
	}

	template <class T>
	inline unsigned divides(Environment & _env)
	{
		if (_env.has_elements<T>(2))
		{
			if (_env.get_stack<T>().top() == T())
				return 1;

			T first = _env.pop<T>();

			T a;
			if (typeid(a) == typeid(int))
			{
				T second = _env.get_stack<T>().top();

				if (second == INT_MIN)
					return 1;
			}

			_env.get_stack<T>().top() /= first;
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

}