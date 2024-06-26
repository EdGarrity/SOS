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
		size_t data_array_size = 0;
		size_t upper_bound_dimension_1 = 0;
		size_t upper_bound_dimension_2 = 0;

	public:
		ThreadSafeArray_2D_V2<T>(size_t upper_bound_dimension_1, size_t upper_bound_dimension_2);
		ThreadSafeArray_2D_V2<T>() : data_array(nullptr), upper_bound_dimension_1(0), upper_bound_dimension_2(0) {};
		~ThreadSafeArray_2D_V2<T>(void);

		void resize(size_t upper_bound_dimension_1, size_t N2);
		T load(size_t index_1, size_t index_2);
		void store(const unsigned int env_index, size_t index_1, size_t index_2, T d);
	};

	template<class T>
	inline ThreadSafeArray_2D_V2<T>::ThreadSafeArray_2D_V2(size_t parameter_upper_bound_dimension_1, size_t parameter_upper_bound_dimension_2)
	{
#if DLEVEL > 0
		std::string debug_message = "Allocating,N1=" + std::to_string(N1) + ",N2=" + std::to_string(N2);
		Utilities::debug_log_nolock(-1, "ThreadSafeArray_2D_V2", debug_message);
#endif
		// Allocate one more than requested as a safety margin
		upper_bound_dimension_1 = parameter_upper_bound_dimension_1 + 1;
		upper_bound_dimension_2 = parameter_upper_bound_dimension_2 + 1;
		data_array_size = upper_bound_dimension_1 * upper_bound_dimension_2;
		data_array = (T*)std::calloc(data_array_size, sizeof(T));

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
	inline void ThreadSafeArray_2D_V2<T>::resize(size_t parameter_upper_bound_dimension_1, size_t parameter_upper_bound_dimension_2)
	{
		if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
		{
			std::ostringstream ss;
			ss << ",upper_bound_dimension_1=" << parameter_upper_bound_dimension_1
				<< ",upper_bound_dimension_2=" << parameter_upper_bound_dimension_2
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

		// Allocate one more than requested as a safety margin
		upper_bound_dimension_1 = parameter_upper_bound_dimension_1 + 1;
		upper_bound_dimension_2 = parameter_upper_bound_dimension_2 + 1;
		data_array_size = upper_bound_dimension_1 * upper_bound_dimension_2;
		data_array = (T*)std::calloc(data_array_size, sizeof(T));

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
					ss << ",upper_bound_dimension_1=" << upper_bound_dimension_1
						<< ",upper_bound_dimension_2=" << upper_bound_dimension_2
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
	inline T ThreadSafeArray_2D_V2<T>::load(size_t index_1, size_t index_2)
	{
		if ((upper_bound_dimension_1 == 0) || (upper_bound_dimension_2 == 0))
		{
			std::stringstream error_message;
			error_message << "ThreadSafeArray_2D_V2::load(" << index_1 << "," << index_2 << ") - Array bounds not defined";

			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			{
				std::ostringstream ss;
				ss << ",method=ThreadSafeArray_2D_V2.load"
					<< ",diagnostic_level=9"
					<< ",index_1=" << index_1
					<< ",index_2=" << index_2
					<< ",upper_bound_dimension_1=" << upper_bound_dimension_1
					<< ",upper_bound_dimension_2=" << upper_bound_dimension_2
					<< ",message=Array_bounds_not_defined";
				Utilities::logline_threadsafe << ss.str();
			}

			throw std::out_of_range(error_message.str());
		}

		// Check for out of bounds
		if ((index_1 >= upper_bound_dimension_1) || (index_2 >= upper_bound_dimension_2))
		{
			std::stringstream error_message;
			error_message << "ThreadSafeArray_2D_V2::load(" << index_1 << "," << index_2 << ") - Index out of bounds";

#if DLEVEL > 0
			std::string debug_message = error_message.str();
			Utilities::debug_log(-1, "ThreadSafeArray_2D_V2", debug_message);
#endif
			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			{
				std::ostringstream ss;
				ss << ",method=ThreadSafeArray_2D_V2.load"
					<< ",diagnostic_level=9"
					<< ",index_1=" << index_1
					<< ",index_2=" << index_2
					<< ",upper_bound_dimension_1=" << upper_bound_dimension_1
					<< ",upper_bound_dimension_2=" << upper_bound_dimension_2
					<< ",message=Index_out_of_bounds";
				Utilities::logline_threadsafe << ss.str();
			}

			throw std::out_of_range(error_message.str());
		}

		std::unique_lock<std::mutex> ThreadSafeArray_V2_array_access_lock(ThreadSafeArray_V2_array_access_);

		size_t index = index_1 * upper_bound_dimension_2 + index_2;
		if (index >= data_array_size)
		{
			std::ostringstream ss;
			ss << ",method=ThreadSafeArray_2D_V2.load"
				<< ",diagnostic_level=9"
				<< ",index_1=" << index_1
				<< ",index_2=" << index_2
				<< ",upper_bound_dimension_1=" << upper_bound_dimension_1
				<< ",upper_bound_dimension_2=" << upper_bound_dimension_2
				<< ",index=" << index
				<< ",data_array_size=" << data_array_size
				<< ",message=Index_out_of_bounds";
			Utilities::logline_threadsafe << ss.str();

			throw std::out_of_range("ThreadSafeArray_2D_V2::load - Index out of bounds");
		}

		T data = data_array[index];
		ThreadSafeArray_V2_array_access_lock.unlock();

		return data;
	}

	template<class T>
	inline void ThreadSafeArray_2D_V2<T>::store(const unsigned int env_index, size_t index_1, size_t index_2, T new_data)
	{
		if ((index_1 >= upper_bound_dimension_1) || (index_2 >= upper_bound_dimension_2))
		{
			std::stringstream error_message;
			error_message << "ThreadSafeArray_2D_V2::store(" << index_1 << "," << index_2 << ") - Index out of bounds";

#if DLEVEL > 0
			std::string debug_message = error_message.str();
			Utilities::debug_log(env_index, "ThreadSafeArray_2D_V2", debug_message);
#endif

			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			{
				std::ostringstream ss;
				ss << ",method=ThreadSafeArray_2D_V2.store"
					<< ",diagnostic_level=9"
					<< ",index_1=" << index_1
					<< ",index_2=" << index_2
					<< ",upper_bound_dimension_1=" << upper_bound_dimension_1
					<< ",upper_bound_dimension_2=" << upper_bound_dimension_2
					<< ",message=Index_out_of_bounds";
				Utilities::logline_threadsafe << ss.str();
			}

			throw std::out_of_range(error_message.str());
		}

		std::unique_lock<std::mutex> ThreadSafeArray_V2_array_access_lock(ThreadSafeArray_V2_array_access_);
		size_t index = index_1 * upper_bound_dimension_2 + index_2;

		if (index >= data_array_size)
		{
			std::ostringstream ss;
			ss << ",method=ThreadSafeArray_2D_V2.store"
				<< ",diagnostic_level=9"
				<< ",index_1=" << index_1
				<< ",index_2=" << index_2
				<< ",upper_bound_dimension_1=" << upper_bound_dimension_1
				<< ",upper_bound_dimension_2=" << upper_bound_dimension_2
				<< ",index=" << index
				<< ",data_array_size=" << data_array_size
				<< ",message=Index_out_of_bounds";
			Utilities::logline_threadsafe << ss.str();

			throw std::out_of_range("ThreadSafeArray_2D_V2::store - Index out of bounds");
		}

		data_array[index] = new_data;
		ThreadSafeArray_V2_array_access_lock.unlock();
	}
}