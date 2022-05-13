#pragma once
#include <array>
#include <stdexcept>
#include <sstream>
#include <atomic>
#include "../Domain/Arguments.h"
#include "..\Plush\Atom.h"
#include "..\Utilities\Debug.h"

#if DLEVEL > 0
extern std::atomic_bool debug_push;
#endif

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
	template <class T, unsigned long N = domain::argmap::maximum_stack_size>
	class FixedSizeStack
	{
	public:
		typedef typename std::array<T, N>::value_type value_type;
		typedef typename std::array<T, N>::reference reference;
		typedef typename std::array<T, N>::const_reference const_reference;
		typedef typename std::array<T, N>::size_type size_type;

		inline void set_current_thread(unsigned long new_current_thread)
		{
			current_thread = new_current_thread;
		};

		// Initializes stack
		inline void clear()
		{
			top_ = 0;
		};

		// Sets value of stack top.  Used when adding a block to the end of the stack.
		inline void set_top(unsigned long new_top)
		{
			if (new_top >= N)
			{
				std::stringstream error_message;
				error_message << "Utilities::FixedSizeStack::set_top() - Stack overflow.  new_top = " << new_top;

				throw std::overflow_error(error_message.str());
			}

			top_ = new_top;
		}

		// Purpose: 
		//   Returns a reference to the underlying container
		//
		// Parameters:
		//   None
		// 
		// Return value:
		//   Reference to the FixedSizeStack stack object
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  
		//
		// Remarks:
		//
		//inline std::array<T, N>& container()
		//{
		//	return stack_;
		//}

		// Checks if the underlying container has no elements
		inline bool is_empty() const
		{
			return top_ == 0;
		}

		//inline unsigned long copy(const FixedSizeStack<T>& other)
		//{
		//	if (other->top_ >= N)
		//	{
		//		std::stringstream error_message;
		//		error_message << "Utilities::FixedSizeStack::copy() - Stack overflow.  top = " << other->top_;

		//		throw std::overflow_error(error_message.str());
		//	}

		//	top_ = other->top_;

		//	for (unsigned long n = 0; n < top_; n++)
		//		stack_[n] = other->stack_[n];

		//	return top_;
		//}

		//FixedSizeStack operator= (const FixedSizeStack& other)
		//{
		//	copy(other);
		//}

		// These need to be rewritten to reference stack from top, not absolute
		inline const_reference operator [] (unsigned long index) const
		{
			if (index >= N)
			{
				std::stringstream error_message;
				error_message << "const_reference Utilities::FixedSizeStack::operator [] - Index Out Of Range.  index = " << index;

				throw std::out_of_range(error_message.str());
			}
			if (index >= top_)
			{
				std::stringstream error_message;
				error_message << "const_reference Utilities::FixedSizeStack::operator [] - Index Out Of Range.  index = " << index
					<< " top = " << top_;

				throw std::out_of_range(error_message.str());
			}
			return stack_[index];
		}

		inline reference operator [] (unsigned long index)
		{
			if (index >= N)
			{
				std::stringstream error_message;
				error_message << "reference Utilities::FixedSizeStack::operator [] - Index Out Of Range.  index = " << index;

				throw std::out_of_range(error_message.str());
			}
			if (index >= top_)
			{
				std::stringstream error_message;
				error_message << "reference Utilities::FixedSizeStack::operator [] - Index Out Of Range.  index = " << index
					<< " top = " << top_;

				throw std::out_of_range(error_message.str());
			}
			return stack_[index];
		}

		// Returns the number of elements in the underlying container
		inline unsigned long size() const
		{
			return top_;
		}

		// Returns the number of available elements in the underlying container
		inline unsigned long free() const
		{
			unsigned long free = 0;
			
			if (N > top_)
				free = N - top_ - 1;

			if (free > N)
				free = N;

			if (top_ >= N)
				free = 0;

			return free;
		}

		// Returns constant reference to the top element in the stack. 
		inline const_reference get_top_atom() const
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
		inline reference get_top_atom()
		{
			if (top_ == 0)
			{
				std::stringstream error_message;
				error_message << "Utilities::FixedSizeStack::top() - Stack empty.";

				throw std::underflow_error(error_message.str());
			}

			return stack_[top_ - 1];
		}

		inline std::string to_string()
		{
			std::string s;

			if (top_ > 0)
			{
				for (size_t n = 0; n < top_; n++)
				{
					s += " ";
					s += std::to_string(stack_[n]);
				}
			}

			return s;
		}

		inline std::string to_string_debug()
		{
			std::string s;

			if (top_ > 0)
			{
				unsigned long stack_bottom = top_ > 10 ? top_ - 10 : 0;

				for (unsigned long n = top_ - 1; n >= stack_bottom; n--)
				{
					s += " ";
					s += std::to_string(stack_[n]);
				}
			}

			return s;
		}

		//inline const_reference get_atom_at_position(unsigned int position) const
		//{
		//	if (top_ == 0)
		//	{
		//		std::stringstream error_message;
		//		error_message << "Utilities::FixedSizeStack::get_item() - Stack empty.";

		//		throw std::underflow_error(error_message.str());
		//	}

		//	position = (position >= top_) ? top_ - 1: position;

		//	return stack_[top_ - position - 1];
		//}

		inline reference get_atom_at_position(size_t position)
		{
			if (top_ == 0)
			{
				std::stringstream error_message;
				error_message << "Utilities::FixedSizeStack::get_item() - Stack empty.";

				throw std::underflow_error(error_message.str());
			}

			position = (position >= top_) ? top_ - 1 : position;

			return stack_[top_ - position - 1];
		}

		inline const_reference get_atom_at_index(size_t index) const
		{
			if (index >= N)
			{
				std::stringstream error_message;
				error_message << "const_reference Utilities::FixedSizeStack::get_atom_at_index - Index Out Of Range.  index = " << index;

				throw std::out_of_range(error_message.str());
			}
			if (index >= top_)
			{
				std::stringstream error_message;
				error_message << "const_reference Utilities::FixedSizeStack::get_atom_at_index - Index Out Of Range.  index = " << index
					<< " top = " << top_;

				throw std::out_of_range(error_message.str());
			}
			return stack_[index];
		}

		inline reference get_atom_at_index(size_t index)
		{
			if (index >= N)
			{
				std::stringstream error_message;
				error_message << "reference Utilities::FixedSizeStack::get_atom_at_index - Index Out Of Range.  index = " << index;

				throw std::out_of_range(error_message.str());
			}
			if (index >= top_)
			{
				std::stringstream error_message;
				error_message << "reference Utilities::FixedSizeStack::get_atom_at_index - Index Out Of Range.  index = " << index
					<< " top = " << top_;

				throw std::out_of_range(error_message.str());
			}
			return stack_[index];
		}

		inline reference get_atom_at_index_unmanaged(size_t index)
		{
			if (index >= N)
			{
				std::stringstream error_message;
				error_message << "reference Utilities::FixedSizeStack::get_atom_at_index_unmanaged - Index Out Of Range.  index = " << index;

				throw std::out_of_range(error_message.str());
			}
			return stack_[index];
		}

		// Removes the top element from the stack
		inline unsigned long pop()
		{
			if (top_ == 0)
			{
				std::stringstream error_message;
				error_message << "Utilities::FixedSizeStack::pop() - Stack empty.";

				throw std::underflow_error(error_message.str());
			}

			top_--;

			return 1;
		}

		// Pushes the given element value to the top of the stack.
		inline unsigned long push(value_type& value)
		{
			if (top_ >= N)
			{
				std::stringstream error_message;
				error_message << "Utilities::FixedSizeStack::push() - Stack overflow.  top = " << top_;

				throw std::overflow_error(error_message.str());
			}

			stack_[top_] = value;
			top_++;

			return 1;
		}

		// Pushes the given element value to the top of the stack.
		inline unsigned long push(std::string& program)
		{
			if (top_ >= N)
			{
				std::stringstream error_message;
				error_message << "Utilities::FixedSizeStack::push() - Stack overflow.  top = " << top_;

				throw std::overflow_error(error_message.str());
			}

			value_type value(program);
			stack_[top_] = value;
			top_++;

			return 1;
		}

		// Purpose: 
		//   Insert provided item deep in the stack
		//
		// Parameters:
		//   other	- Reference to the other item to insert
		//   n		- Positin where to insert the other item.  0 or less refers to the top of the stack.  
		//			  Values greater than the size of the stack will insert the other item at the bottom.
		// 
		// Return value:
		//   None
		//
		// Side Effects:
		//   Stack updated with inserted item.
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		inline unsigned long shove_it(T& atom, unsigned long position)
		{
			if ((top_ + 1) > N)
			{
				std::stringstream error_message;
				error_message << "Utilities::FixedSizeStack::shove() - Stack overflow.";

				throw std::overflow_error(error_message.str());
			}

			if (top_ > 0)
			{
				position = (position >= top_) ? top_ - 1 : position;

				unsigned long index = top_ - position;

				for (unsigned long i = top_; i >= index; i--)
					stack_[i] = stack_[i - 1];

				stack_[index] = atom;
			}

			else
				stack_[0] = atom;

			top_++;

			return position;
		}

		// Purpose: 
		//   Insert section of provided stack deep in the stack
		//
		// Parameters:
		//   other				- Reference to the other stack to insert
		//   insert_position	- Positin where to insert the other stack.  0 or less refers to the top of the stack.  
		//						  Values greater than the size of the stack will insert the other stack at the bottom.
		//   offset				- Offset to start of section in other stack to insert
		//   length				- Length of section to insert
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
		inline unsigned long shove_it(unsigned long destination_position, unsigned long source_position, unsigned long length)
		{
			if ((top_ + length) > N)
			{
				std::stringstream error_message;
				error_message << "Utilities::FixedSizeStack::shove() - Stack overflow.";

				throw std::overflow_error(error_message.str());
			}

			source_position = (source_position >= top_) ? top_ - 1: source_position;
			destination_position = (destination_position >= top_) ? top_ - 1: destination_position;

			unsigned long source_index = position_to_index(source_position);
			unsigned long destination_index = position_to_index(destination_position);

			// Make space in this stack for the other stack items
			for (unsigned long i = 0, j = top_ - 1, k = top_ - 1 + length;
				i < top_ - destination_index;
				i++, j--, k--)
				stack_[k] = stack_[j];

			for (unsigned long i = 0, j = top_, k = destination_index;
				i < length;
				i++, j++, k++)
				stack_[k] = stack_[j];

			return top_ - destination_index + length;
		}

		// Purpose: 
		//   Removes a section of the stack
		//
		// Parameters:
		//   position	- Positin of first item to remove from the stack.  0 or less refers to the top of the stack.  
		//				  Values greater than the size of the stack-length will remove items from the end.
		//   length		- Length of section to remove
		//
		// Return value:
		//   None
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		inline unsigned long remove_items(unsigned long position, unsigned long length)
		{
#if DLEVEL > 0
			if (debug_push.load(std::memory_order_acquire))
			{
				std::string debug = "entry,position=" + std::to_string(position) 
					+ ",length=" + std::to_string(length) 
					+ ",top_=" + std::to_string(top_);
				Utilities::debug_log(current_thread, "FixedSizeStack::remove_items", debug);
			}
#endif

			if (length > top_)
				length = top_;

			if (position + length > top_)
				position = top_ - length;

			if (position > 0)
			{
				unsigned long j = top_ - position, k = (top_ - position - 1) - (length - 1);
#if DLEVEL > 0
				if (debug_push.load(std::memory_order_acquire))
				{
					std::string debug = "for,position=" + std::to_string(position)
						+ ",length=" + std::to_string(length)
						+ ",top_=" + std::to_string(top_)
						+ ",j=" + std::to_string(j)
						+ ",k=" + std::to_string(k);
						Utilities::debug_log(current_thread, "FixedSizeStack::remove_items", debug);
				}
#endif

				for (/*size_t j = top_ - position, k = (top_ - position - 1) - (length - 1)*/;
					j < top_;
					j++, k++)
					stack_[k] = stack_[j];

				//size_t k = (top_ - position - 1) - (length - 1);
				//size_t j = top_ - position;
				//std::copy(stack_.data() + j, stack_.data() + top_, stack_.data() + k);
			}

			top_ -= length;

#if DLEVEL > 0
			if (debug_push.load(std::memory_order_acquire))
			{
				std::string debug = "exit,position=" + std::to_string(position)
					+ ",length=" + std::to_string(length)
					+ ",top_=" + std::to_string(top_);
				Utilities::debug_log(current_thread, "FixedSizeStack::remove_items", debug);
			}
#endif

			return position;
		}

		inline unsigned long position_to_index(unsigned long position)
		{
			position = (position >= top_) ? top_ - 1 : position;

			if ((position > 0) && (top_ > 0))
				return top_ - position - 1;
			else if (top_ > 0)
				return top_ - 1;
			else
				return 0;
		}

		// Purpose: 
		//   Replaces stack atom with provided atom
		//
		// Parameters:
		//   other	- Reference to the other item to replace with
		//   n		- Positin of item in stack to replace.  0 or less refers to the top of the stack.  
		//			  Values greater than the size of the stack will replace the item at the bottom.
		// 
		// Return value:
		//   None
		//
		// Side Effects:
		//   Stack updated with inserted item.
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		inline unsigned long replace(T& other, unsigned long n)
		{
			if (n >= N)
			{
				std::stringstream error_message;
				error_message << "reference Utilities::FixedSizeStack::replace() - Stack overflow.  index = " << n;

				throw std::overflow_error(error_message.str());
			}

			n = (n >= top_) ? top_ - 1 : n;

			stack_[top_ - n - 1] = other;

			return 1;
		}

		// Purpose: 
		//   Replaces stack section with provided section
		//
		// Parameters:
		//   source_position	- Positin of first item to copy from the stack.  0 or less refers to the top of the stack.  
		//   target_position	- Positin of where to copy the first item to.  0 or less refers to the top of the stack.  
		//   length				- Length of section to copy
		//
		// Return value:
		//   None
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		inline unsigned long replace_section(unsigned long source_position, unsigned long target_position, unsigned long length)
		{
			if ((source_position >= N) || (source_position >= top_))
			{
				std::stringstream error_message;
				error_message << "reference Utilities::FixedSizeStack::replace_section() - Stack overflow.  source_position = " << source_position;

				throw std::overflow_error(error_message.str());
			}

			if ((target_position >= N) || (target_position >= top_))
			{
				std::stringstream error_message;
				error_message << "reference Utilities::FixedSizeStack::replace_section() - Stack overflow.  target_position = " << target_position;

				throw std::overflow_error(error_message.str());
			}

			// Convert relative positions to absolute indexes
			unsigned long source_index = top_ - source_position - 1;
			unsigned long target_index = top_ - target_position - 1;

			// Copy source to target
			if (source_index > target_index)
			{
				for (unsigned long n = 0; n < length; n++)
					stack_[target_index + n] = stack_[source_index + n];
			}
			 
			set_top(target_index + length);

			return length;
		}

		// Purpose: 
		//   Pushes a section of the stack on top of the stack
		//
		// Parameters:
		//   position	- Positin of first item to copy from the stack.  0 or less refers to the top of the stack.  
		//				  Values greater than the size of the stack-length will copy items from the end.
		//   length		- Length of section to copy
		//
		// Return value:
		//   None
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		inline unsigned long yankdup_item(unsigned long position, unsigned long length)
		{
#if DLEVEL > 0
			if (debug_push.load(std::memory_order_acquire))
			{
				std::string debug = "entry,position=" + std::to_string(position)
					+ ",length=" + std::to_string(length)
					+ ",top_=" + std::to_string(top_);
				Utilities::debug_log(current_thread, "FixedSizeStack::yankdup_item", debug);
			}
#endif

			if (position >= top_)
				position = top_ - 1;

			if (length > top_)
				length = top_;

			if ((top_ - position - length >= 0) && ((top_ + length) < N))
			{
				unsigned long i = 0, j = top_ - position - length, k = top_;
#if DLEVEL > 0
				if (debug_push.load(std::memory_order_acquire))
				{
					std::string debug = "entry,position=" + std::to_string(position)
						+ ",length=" + std::to_string(length)
						+ ",top_=" + std::to_string(top_)
						+ ",N=" + std::to_string(N)
						+ ",i=" + std::to_string(i)
						+ ",j=" + std::to_string(j)
						+ ",k=" + std::to_string(k);
					Utilities::debug_log(current_thread, "FixedSizeStack::yankdup_item", debug);
				}
#endif
				for (/*size_t i = 0, j = top_ - position - length, k = top_*/;
					(i < length) && (k < N);
					i++, j++, k++)
					stack_[k] = stack_[j];

				//size_t k = top_;
				//size_t j = top_ - position - length;
				//std::copy(stack_.data() + j, stack_.data() + j + length, stack_.data() + k);

				set_top(top_ + length);
			}
			else
				length = 0;

#if DLEVEL > 0
			if (debug_push.load(std::memory_order_acquire))
			{
				std::string debug = "exit,position=" + std::to_string(position)
					+ ",length=" + std::to_string(length)
					+ ",top_=" + std::to_string(top_);
				Utilities::debug_log(current_thread, "FixedSizeStack::yankdup_item", debug);
			}

			if (debug_push.load(std::memory_order_acquire))
			{
				std::string debug = "exit,length=" + std::to_string(length);
				Utilities::debug_log(current_thread, "FixedSizeStack::yankdup_item", debug);
			}
#endif
			return length;
		}

	protected:
		// Zero-based index to the first empty slot on the stack
		unsigned long top_ = 0;

		// The containter for the stack
		std::array<T, N> stack_;

		unsigned int current_thread = 999;
	};
}