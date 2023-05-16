	#pragma once
//#include <array>
//#include <iostream>
//#include <new>
//#include <atomic>
#include <stdlib.h>  
#include <stdexcept>
#include <sstream>
#include <mutex>
#include "Debug.h"

namespace Utilities
{
	extern std::mutex array_access_;

	template <class T, size_t N1, size_t N2>
	class ThreadSafeArray_2D
	{
	private:
		T *data_array;
		size_t n1;
		size_t n2;

	public:
		ThreadSafeArray_2D<T,N1,N2>();
		~ThreadSafeArray_2D<T,N1, N2>(void);

		T load(size_t n1, size_t n2);
		void store(const unsigned int env_index, size_t y, size_t x, T d);
	};

	template<class T, size_t N1, size_t N2>
	inline ThreadSafeArray_2D<T, N1, N2>::ThreadSafeArray_2D()
	{
#if DLEVEL > 0
		std::string debug_message = "Allocating,N1=" + std::to_string(N1) + ",N2=" + std::to_string(N2);
		Utilities::debug_log_nolock(-1, "ThreadSafeArray_2D", debug_message);
#endif
		data_array = (T*)calloc(N1 * N2, sizeof(T));
		n1 = N1;
		n2 = N2;

		if (data_array == nullptr)
		{
			{
				std::stringstream error_message;
				error_message << "ThreadSafeArray_2D - Out of memory";

#if DLEVEL > 0
				std::string debug_message = error_message.str();
				Utilities::debug_log(-1, "ThreadSafeArray_2D", debug_message);
#endif
				throw std::runtime_error(error_message.str());
			}
		}
	}

	template<class T, size_t N1, size_t N2>
	inline ThreadSafeArray_2D<T, N1, N2>::~ThreadSafeArray_2D(void)
	{
#if DLEVEL > 0
		std::string debug_message = "Deallocating,N1=" + std::to_string(N1) + ",N2=" + std::to_string(N2);
		Utilities::debug_log_nolock(-1, "ThreadSafeArray_2D", debug_message);
#endif
		free(data_array);
	}
	template<class T, size_t N1, size_t N2>
	inline T ThreadSafeArray_2D<T, N1, N2>::load(size_t y, size_t x)
	{
		if ((y >= N1) || (x >= N2))
		{
			std::stringstream error_message;
			error_message << "ThreadSafeArray_2D::load(" << y << "," << x << ") - Index out of bounds";

#if DLEVEL > 0
			std::string debug_message = error_message.str();
			Utilities::debug_log(-1, "ThreadSafeArray_2D", debug_message);
#endif
			throw std::out_of_range(error_message.str());
		}

		std::unique_lock<std::mutex> array_access_lock(array_access_);
		T data = data_array[y * n2 + x];
		array_access_lock.unlock();

		return data;
	}
	template<class T, size_t N1, size_t N2>
	inline void ThreadSafeArray_2D<T, N1, N2>::store(const unsigned int env_index, size_t y, size_t x, T d)
	{
		if ((y >= N1) || (x >= N2))
		{
			std::stringstream error_message;
			error_message << "ThreadSafeArray_2D::store(" << y << "," << x << ") - Index out of bounds";

#if DLEVEL > 0
			std::string debug_message = error_message.str();
			Utilities::debug_log(env_index, "ThreadSafeArray_2D", debug_message);
#endif
			throw std::out_of_range(error_message.str());
		}

		std::unique_lock<std::mutex> array_access_lock(array_access_);
		data_array[y * n2 + x] = d;
		array_access_lock.unlock();
	}
}