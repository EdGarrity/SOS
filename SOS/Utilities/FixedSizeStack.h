#pragma once
#include <array>
#include "..\PushGP\Globals.h"

// Purpose: 
//   Impliments a fixed-sized stack (FIFO)
//
//     template<class T> class FixedSizeStack;
//
//   The Utilities::FixedSizeStack class uses a fixed std::array container that gives the programmer 
//   the functionality of a stack - specifically, a LIFO (last-in, first-out) data structure.
//
//   The class template acts as a wrapper to the underlying container - only a specific set of functions 
//   is provided.  The stack pushes and pops the element from the back of the underlying container, 
//   known as the top of the stack.
//
// Template Parameters:
//   T - The type of the stored elements.  The behavior is undefined if T is not the same type as 
//   Container::value_type. (since C++17)
//

namespace Utilities
{
	// Template class definition
	template <class T, size_t N = domain::argmap::maximum_stack_size>
	class FixedSizeStack
	{
	public:
		typedef typename std::array<T, N>::value_type value_type;
		typedef typename std::array<T, N>::reference reference;
		typedef typename std::array<T, N>::const_reference const_reference;
		typedef typename std::array<T, N>::size_type size_type;

		// Initializes stack
		inline void clear()
		{
			top_ = 0;
		};

		// Returns constant reference to the top element in the stack. 
		inline const_reference top() const
		{
			if (top_ == 0)
				throw;

			return stack_[top_ - 1];
		}

		// Returns reference to the top element in the stack. 
		inline reference top()
		{
			if (top_ == 0)
				throw;

			return stack_[top_ - 1];
		}

		// Checks if the underlying container has no elements
		inline bool empty() const
		{
			return top_ == 0;
		}

		// Returns the number of elements in the underlying container
		inline size_type size() const
		{
			return top_;
		}

		// Pushes the given element value to the top of the stack.
		inline void push(const value_type& value)
		{
			if (top_ >= N)
				throw;

			stack_[top_] = value;
			top_++;
		}

		// Pushes the given element value to the top of the stack.
		inline void push(value_type&& value)
		{
			if (top_ >= N)
				throw;

			stack_[top_] = value;
			top_++;
		}

		// Removes the top element from the stack
		inline void pop()
		{
			if (top_ == 0)
				throw;

			top_--;
		}

		// Returns a reference to the underlying container
		inline std::array<T, N>& container()
		{
			return stack_;
		}

	private:
		// Zero-based index to the first empty slot on the stack
		size_type top_ = 0;

		// The containter for the stack
		std::array<T, N> stack_;

	};
}