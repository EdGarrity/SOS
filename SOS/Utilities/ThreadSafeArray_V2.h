#pragma once
#include <stdlib.h>  
#include <stdexcept>
#include <sstream>
#include <mutex>
#include "Debug.h"

namespace Utilities
{
	extern std::mutex ThreadSafeArray_V2_array_access_;

	template <class T>
	class ThreadSafeArray_2D_V2
	{
	private:
		T* data_array = nullptr;
		size_t n1 = 0;
		size_t n2 = 0;

	public:
		ThreadSafeArray_2D_V2<T>(size_t N1, size_t N2);
		ThreadSafeArray_2D_V2<T>() : data_array(nullptr), n1(0), n2(0) {};
		~ThreadSafeArray_2D_V2<T>(void);

		void resize(size_t N1, size_t N2);
		T load(size_t n1, size_t n2);
		void store(const unsigned int env_index, size_t y, size_t x, T d);
	};

	//template<class T>
	//inline ThreadSafeArray_2D_V2<T>::ThreadSafeArray_2D_V2()
	//{
	//	{
	//		std::ostringstream ss;
	//		ss << ",method=ThreadSafeArray_2D_V2.load"
	//			<< ",message=Array_bounds_not_defined";
	//		Utilities::logline_threadsafe << ss.str();
	//	}

	//	throw std::runtime_error("ThreadSafeArray_2D_V2 - Array bounds not defined");
	//}

	template<class T>
	inline ThreadSafeArray_2D_V2<T>::ThreadSafeArray_2D_V2(size_t N1, size_t N2)
	{
#if DLEVEL > 0
		std::string debug_message = "Allocating,N1=" + std::to_string(N1) + ",N2=" + std::to_string(N2);
		Utilities::debug_log_nolock(-1, "ThreadSafeArray_2D_V2", debug_message);
#endif
		data_array = (T*)std::calloc(N1 * N2, sizeof(T));
		n1 = N1;
		n2 = N2;

		if (data_array == nullptr)
		{
			{
				std::stringstream error_message;
				error_message << "ThreadSafeArray_2D_V2 - Out of memory";

#if DLEVEL > 0
				std::string debug_message = error_message.str();
				Utilities::debug_log(-1, "ThreadSafeArray_2D_V2", debug_message);
#endif
				throw std::runtime_error(error_message.str());
			}
		}
	}

	template<class T>
	inline ThreadSafeArray_2D_V2<T>::~ThreadSafeArray_2D_V2(void)
	{
#if DLEVEL > 0
		std::string debug_message = "Deallocating,N1=" + std::to_string(n1) + ",N2=" + std::to_string(n2);
		Utilities::debug_log_nolock(-1, "ThreadSafeArray_2D_V2", debug_message);
#endif
		if (data_array != nullptr)
			std::free(data_array);
	}

	template<class T>
	inline void ThreadSafeArray_2D_V2<T>::resize(size_t N1, size_t N2)
	{
		if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
		{
			std::ostringstream ss;
			ss << ",N1=" << N1
				<< ",N2=" << N2
				<< ",method=ThreadSafeArray_2D_V2.resize"
				<< ",diagnostic_level=9"
				<< ",message=Enter";
			Utilities::logline_threadsafe << ss.str();
		}


#if DLEVEL > 0
		std::string debug_message = "Allocating,N1=" + std::to_string(N1) + ",N2=" + std::to_string(N2);
		Utilities::debug_log_nolock(-1, "ThreadSafeArray_2D_V2", debug_message);
#endif
		if (data_array != nullptr)
			std::free(data_array);

		n1 = N1 + 1;
		n2 = N2 + 1;
		data_array = (T*)std::calloc(n1 * n2, sizeof(T));

		if (data_array == nullptr)
		{
			{
				std::stringstream error_message;
				error_message << "ThreadSafeArray_2D_V2 - Out of memory";

#if DLEVEL > 0
				std::string debug_message = error_message.str();
				Utilities::debug_log(-1, "ThreadSafeArray_2D_V2", debug_message);
#endif
				if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
				{
					std::ostringstream ss;
					ss << ",N1=" << N1
						<< ",N2=" << N2
						<< ",method=ThreadSafeArray_2D_V2.resize"
						<< ",diagnostic_level=9"
						<< ",message=Out_of_memory";
					Utilities::logline_threadsafe << ss.str();
				}

				throw std::runtime_error(error_message.str());
			}
		}
	}

	template<class T>
	inline T ThreadSafeArray_2D_V2<T>::load(size_t y, size_t x)
	{
		if ((n1 == 0) || (n2 == 0))
		{
			std::stringstream error_message;
			error_message << "ThreadSafeArray_2D_V2::load(" << y << "," << x << ") - Array bounds not defined";

			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			{
				std::ostringstream ss;
				ss << ",method=ThreadSafeArray_2D_V2.load"
					<< ",diagnostic_level=9"
					<< ",y=" << y
					<< ",x=" << x
					<< ",n1=" << n1
					<< ",n2=" << n2
					<< ",message=Array_bounds_not_defined";
				Utilities::logline_threadsafe << ss.str();
			}

			throw std::out_of_range(error_message.str());
		}

		if ((y >= n1) || (x >= n2))
		{
			std::stringstream error_message;
			error_message << "ThreadSafeArray_2D_V2::load(" << y << "," << x << ") - Index out of bounds";

#if DLEVEL > 0
			std::string debug_message = error_message.str();
			Utilities::debug_log(-1, "ThreadSafeArray_2D_V2", debug_message);
#endif
			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			{
				std::ostringstream ss;
				ss << ",method=ThreadSafeArray_2D_V2.load"
					<< ",diagnostic_level=9"
					<< ",y=" << y
					<< ",x=" << x
					<< ",n1=" << n1
					<< ",n2=" << n2
					<< ",message=Index_out_of_bounds";
				Utilities::logline_threadsafe << ss.str();
			}

			throw std::out_of_range(error_message.str());
		}

		std::unique_lock<std::mutex> ThreadSafeArray_V2_array_access_lock(ThreadSafeArray_V2_array_access_);
		T data = data_array[y * n2 + x];
		ThreadSafeArray_V2_array_access_lock.unlock();

		return data;
	}

	template<class T>
	inline void ThreadSafeArray_2D_V2<T>::store(const unsigned int env_index, size_t y, size_t x, T d)
	{
		if ((y >= n1) || (x >= n2))
		{
			std::stringstream error_message;
			error_message << "ThreadSafeArray_2D_V2::store(" << y << "," << x << ") - Index out of bounds";

#if DLEVEL > 0
			std::string debug_message = error_message.str();
			Utilities::debug_log(env_index, "ThreadSafeArray_2D_V2", debug_message);
#endif
			throw std::out_of_range(error_message.str());
		}

		std::unique_lock<std::mutex> ThreadSafeArray_V2_array_access_lock(ThreadSafeArray_V2_array_access_);
		int index = y * n2 + x;
		data_array[index] = d;
		ThreadSafeArray_V2_array_access_lock.unlock();
	}
}