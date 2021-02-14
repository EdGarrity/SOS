#pragma once
#include <array>
#include <iostream>
#include <new>
#include <atomic>

namespace Utilities
{
	template <class T, size_t N>
	class ThreadSafeArray
	{
	private:
		std::atomic<T>* data;
		size_t n;

	public:
		ThreadSafeArray<T, N>();
		~ThreadSafeArray<T, N>(void);

		T load(size_t n);
		void store(size_t n, T d);
	};

	template<class T, size_t N>
	inline ThreadSafeArray<T, N>::ThreadSafeArray()
	{
		data = new std::atomic<T>[N];
		n = N;
	}

	template<class T, size_t N>
	inline ThreadSafeArray<T, N>::~ThreadSafeArray(void)
	{
		delete[] data;
	}

	template<class T, size_t N>
	inline T ThreadSafeArray<T, N>::load(size_t n)
	{
		return data[n].load(std::memory_order_acquire);
	}
	template<class T, size_t N>
	inline void ThreadSafeArray<T, N>::store(size_t n, T d)
	{
		data[n].store(d, std::memory_order_release);
	}


	template <class T, size_t N1, size_t N2>
	class ThreadSafeArray_2D
	{
	private:
		ThreadSafeArray<T, N1>* data_array;
		size_t n1;
		size_t n2;

	public:
		ThreadSafeArray_2D<T,N1,N2>();
		~ThreadSafeArray_2D<T,N1, N2>(void);

		T load(size_t n1, size_t n2);
		void store(size_t n1, size_t n2, T d);
	};

	template<class T, size_t N1, size_t N2>
	inline ThreadSafeArray_2D<T, N1, N2>::ThreadSafeArray_2D()
	{
		data_array = new ThreadSafeArray<T, N1>[N2];
		n1 = N1;
		n2 = N2;
	}

	template<class T, size_t N1, size_t N2>
	inline ThreadSafeArray_2D<T, N1, N2>::~ThreadSafeArray_2D(void)
	{
		delete[] data_array;
	}
	template<class T, size_t N1, size_t N2>
	inline T ThreadSafeArray_2D<T, N1, N2>::load(size_t n1, size_t n2)
	{
		return data_array[n2].load(n1);
	}
	template<class T, size_t N1, size_t N2>
	inline void ThreadSafeArray_2D<T, N1, N2>::store(size_t n1, size_t n2, T d)
	{
		data_array[n2].store(n1, d);
	}
}