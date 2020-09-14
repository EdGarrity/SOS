#pragma once
#include <array>
#include <stdexcept>
#include <sstream>
//#include "..\PushGP\Globals.h"
#include "../Domain/Arguments.h"
#include "..\Plush\Atom.h"

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

		inline reference bottom()
		{
			if (top_ == 0)
			{
				std::stringstream error_message;
				error_message << "Utilities::FixedSizeStack::bottom() - Stack empty.";

				throw std::underflow_error(error_message.str());
			}

			return stack_[0];
		}

		// Initializes stack
		inline void clear()
		{
			top_ = 0;
		};

		// Returns a reference to the underlying container
		inline std::array<T, N>& container()
		{
			return stack_;
		}

		// Checks if the underlying container has no elements
		inline bool empty() const
		{
			return top_ == 0;
		}

		inline void copy(const FixedSizeStack<T>& other)
		{
			if (other->top_ > N)
			{
				std::stringstream error_message;
				error_message << "Utilities::FixedSizeStack::copy() - Stack overflow.  top = " << other->top_;

				throw std::overflow_error(error_message.str());
			}

			top_ = other->top_;

			for (int n = 0; n < top_; n++)
				stack_[n] = other->stack_[n];
		}

		FixedSizeStack operator= (const FixedSizeStack& other)
		{
			copy(other);
		}

		inline const_reference operator [] (int index) const
		{
			if (index > N)
			{
				std::stringstream error_message;
				error_message << "const_reference Utilities::FixedSizeStack::operator [] - Stack overflow.  index = " << index;

				throw std::overflow_error(error_message.str());
			}
			return stack_[index];
		}

		inline reference operator [] (int index)
		{
			if (index > N)
			{
				std::stringstream error_message;
				error_message << "reference Utilities::FixedSizeStack::operator [] - Stack overflow.  index = " << index;

				throw std::overflow_error(error_message.str());
			}
			return stack_[index];
		}

		// Removes the top element from the stack
		inline void pop()
		{
			if (top_ == 0)
			{
				std::stringstream error_message;
				error_message << "Utilities::FixedSizeStack::pop() - Stack empty.";

				throw std::underflow_error(error_message.str());
			}

			top_--;
		}

		// Pushes the given element value to the top of the stack.
		//inline void push(const value_type& value)
		//{
		//	if (top_ >= N)
		//	{
		//		std::stringstream error_message;
		//		error_message << "Utilities::FixedSizeStack::push() - Stack overflow.";

		//		throw std::overflow_error(error_message.str());
		//	}

		//	stack_[top_] = value;
		//	top_++;
		//}

		// Pushes the given element value to the top of the stack.
		//inline void push(value_type&& value)
		//{
		//	if (top_ >= N)
		//	{
		//		std::stringstream error_message;
		//		error_message << "Utilities::FixedSizeStack::push() - Stack overflow.";

		//		throw std::overflow_error(error_message.str());
		//	}

		//	stack_[top_] = value;
		//	top_++;
		//}

		// Pushes the given element value to the top of the stack.
		inline void push(value_type value)
		{
			if (top_ > N)
			{
				std::stringstream error_message;
				error_message << "Utilities::FixedSizeStack::push() - Stack overflow.  top = " << top_;

				throw std::overflow_error(error_message.str());
			}

			stack_[top_] = value;
			top_++;
		}

		// Purpose: 
		//   Insert provided stack deep in the stack
		//
		// Parameters:
		//   other	- Reference to the other stack to insert
		//   n		- Positin where to insert the other stack.  0 or less refers to the top of the stack.  
		//			  Values greater than the size of the stack will insert the other stack at the bottom.
		// 
		// Return value:
		//   None
		//
		// Side Effects:
		//   Stack updated with inserted stack.
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		inline void shove(FixedSizeStack<T>& other, int n)
		{
			if ((top_ + other.size()) > N)
			{
				std::stringstream error_message;
				error_message << "Utilities::FixedSizeStack::shove() - Stack overflow.";

				throw std::overflow_error(error_message.str());
			}

			n = (n < 0) ? 0 : n;
			n = (n > top_) ? top_ : n;
			n = n - top_;

			for (int i = (top_ + other.size()); i > other.size() - 1; i--)
				stack_[i] = stack_[i - top_ + 1];

			for (int i = 0; i < other.size(); i++)
				stack_[i + n] = other[i];

			top_ += other.size();
		}

		// Returns the number of elements in the underlying container
		inline size_type size() const
		{
			return top_;
		}

		virtual unsigned int split(Utilities::FixedSizeStack<T> &left_half, 
			Utilities::FixedSizeStack<T> &right_half, 
			unsigned int split_position) 
		{ 
			return 0; 
		};

		// Returns constant reference to the top element in the stack. 
		inline const_reference get_top() const
		{
			if (top_ == 0)
			{
				std::stringstream error_message;
				error_message << "Utilities::FixedSizeStack::top() - Stack empty.";

				throw std::underflow_error(error_message.str());
			}

			return stack_[top_ - 1];
		}

		// Returns reference to the top element in the stack. 
		inline reference get_top()
		{
			if (top_ == 0)
			{
				std::stringstream error_message;
				error_message << "Utilities::FixedSizeStack::top() - Stack empty.";

				throw std::underflow_error(error_message.str());
			}

			return stack_[top_ - 1];
		}

	protected:
		// Zero-based index to the first empty slot on the stack
		size_type top_ = 0;

		// The containter for the stack
		std::array<T, N> stack_;
	};
}