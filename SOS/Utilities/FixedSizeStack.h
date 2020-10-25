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
			if (other->top_ >= N)
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

		// These need to be rewritten to reference stack from top, not absolute
		inline const_reference operator [] (int index) const
		{
			if (index >= N)
			{
				std::stringstream error_message;
				error_message << "const_reference Utilities::FixedSizeStack::operator [] - Stack overflow.  index = " << index;

				throw std::overflow_error(error_message.str());
			}
			return stack_[index];
		}

		inline reference operator [] (int index)
		{
			if (index >= N)
			{
				std::stringstream error_message;
				error_message << "reference Utilities::FixedSizeStack::operator [] - Stack overflow.  index = " << index;

				throw std::overflow_error(error_message.str());
			}
			return stack_[index];
		}

		// Purpose: 
		//   Returns a reference to the genome container (the atom array)
		//
		// Parameters:
		//   None
		// 
		// Return value:
		//   Reference to the genome's FixedSizeStack object
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  
		//
		// Remarks:
		//
		inline const T& get_stack_element(int index)
		{
			return stack_[index];
		}

		inline T& get_stack_element_ref(int index)
		{
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
		inline void push(value_type& value)
		{
			if (top_ >= N)
			{
				std::stringstream error_message;
				error_message << "Utilities::FixedSizeStack::push() - Stack overflow.  top = " << top_;

				throw std::overflow_error(error_message.str());
			}

			stack_[top_] = value;
			top_++;
		}

		// Pushes the given element value to the top of the stack.
		inline void push(std::string& program)
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
		inline void shove_it(T& atom, int position)
		{
			if ((top_ + 1) > N)
			{
				std::stringstream error_message;
				error_message << "Utilities::FixedSizeStack::shove() - Stack overflow.";

				throw std::overflow_error(error_message.str());
			}

			position = (position < 0) ? 0 : position;
			position = (position > top_) ? top_ : position;
			int index = top_ - position;

			for (int i = top_; i >= index; i--)
				stack_[i] = stack_[i - 1];

			stack_[index] = atom;

			top_++;
		}

		//// Purpose: 
		////   Insert provided stack deep in the stack
		////
		//// Parameters:
		////   other	- Reference to the other stack to insert
		////   n		- Positin where to insert the other stack.  0 or less refers to the top of the stack.  
		////			  Values greater than the size of the stack will insert the other stack at the bottom.
		//// 
		//// Return value:
		////   None
		////
		//// Side Effects:
		////   Stack updated with inserted stack.
		////
		//// Thread Safe:
		////   Yes.  As long as no other thread attemps to write to the child.
		////
		//// Remarks:
		////
		//inline void shove(FixedSizeStack<T>& other, int n)
		//{
		//	if ((top_ + other.size()) > N)
		//	{
		//		std::stringstream error_message;
		//		error_message << "Utilities::FixedSizeStack::shove() - Stack overflow.";

		//		throw std::overflow_error(error_message.str());
		//	}

		//	n = (n < 0) ? 0 : n;
		//	n = (n > top_) ? top_ : n;
		//	n = n - top_;

		//	for (int i = (top_ + other.size()); i > other.size() - 1; i--)
		//		stack_[i] = stack_[i - top_ + 1];

		//	for (int i = 0; i < other.size(); i++)
		//		stack_[i + n] = other[i];

		//	top_ += other.size();
		//}

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
		inline void shove_it(int destination_position, int source_position, int length)
		{
			if ((top_ + length) > N)
			{
				std::stringstream error_message;
				error_message << "Utilities::FixedSizeStack::shove() - Stack overflow.";

				throw std::overflow_error(error_message.str());
			}

			source_position = (source_position < 0) ? 0 : source_position;
			source_position = (source_position > top_) ? top_ : source_position;

			destination_position = (destination_position < 0) ? 0 : destination_position;
			destination_position = (destination_position > top_) ? top_ : destination_position;

			int source_index = top_ - source_position - 1;
			int destination_index = top_ - destination_position - 1;

			// Make space in this stack for the other stack items
			for (int i = 0, j = destination_index + length - 1, k = top_ + length - 1;
				i < length;
				i++, j--, k--)
				stack_[k] = stack_[j];

//			int n2 = other.size() - source_position;

			// Copy the other stack items into this stack
			for (int i = 0, j = source_index + length - 1, k = destination_index + length - 1;
				i < length; 
				i++, j--, k--)
				stack_[k] = stack_[j];

			top_ += length;
		}

		//// Purpose: 
		////   Insert section of provided stack section deep in the stack
		////
		//// Parameters:
		////   insert_position	- Positin where to insert the other stack.  0 or less refers to the top of the stack.  
		////						  Values greater than the size of the stack will insert the other stack at the bottom.
		////   offset				- Offset to start of section in other stack to insert
		////   length				- Length of section to insert
		////
		//// Return value:
		////   None
		////
		//// Side Effects:
		////   Stack updated with inserted stack.
		////
		//// Thread Safe:
		////   Yes.  As long as no other thread attemps to write to the child.
		////
		//// Remarks:
		////
		//inline void shove(int insert_position, int offset, int length)
		//{
		//	if ((top_ + length) > N)
		//	{
		//		std::stringstream error_message;
		//		error_message << "Utilities::FixedSizeStack::shove() - Stack overflow.";

		//		throw std::overflow_error(error_message.str());
		//	}

		//	insert_position = (insert_position < 0) ? 0 : insert_position;
		//	insert_position = (insert_position > top_) ? top_ : insert_position;

		//	// Make space in this stack for the other stack items
		//	for (int i = 0, j = insert_position + length - 1, k = top_ + length - 1;
		//		i < length;
		//		i++, j--, k--)
		//		stack_[k] = stack_[j];

		//	int n2 = size() - offset;

		//	// Copy the other stack items into this stack
		//	for (int i = 0, j = size() - offset - length, k = top_ - insert_position;
		//		i < length;
		//		i++)
		//		stack_[k] = stack_[j];

		//	top_ += length;
		//}

		//// Purpose: 
		////   Insert an atom deep in the stack
		////
		//// Parameters:
		////   atom				- Reference to Atom to insert
		////   insert_position	- Positin where to insert the atom.  0 or less refers to the top of the stack.  
		////						  Values greater than the size of the stack will insert the other stack at the bottom.
		////
		//// Return value:
		////   None
		////
		//// Side Effects:
		////   Stack updated with inserted stack.
		////
		//// Thread Safe:
		////   Yes.  As long as no other thread attemps to write to the child.
		////
		//// Remarks:
		////
		//inline void shove(T& atom, int insert_position)
		//{
		//	if ((top_ + 1) > N)
		//	{
		//		std::stringstream error_message;
		//		error_message << "Utilities::FixedSizeStack::shove() - Stack overflow.";

		//		throw std::overflow_error(error_message.str());
		//	}

		//	insert_position = (insert_position < 0) ? 0 : insert_position;
		//	insert_position = (insert_position > top_) ? top_ : insert_position;

		//	// Make space in this stack for the other stack items
		//	for (int i = 0, j = insert_position, k = top_;
		//		i < 1;
		//		i++, j--, k--)
		//		stack_[k] = stack_[j];

		//	int n2 = other.size();

		//	// Copy the other stack items into this stack
		//	stack_[top_ - insert_position] = atom;

		//	top_ += 1;
		//}

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
		inline void remove_items(int position, int length)
		{
			if (position < 0)
				position = 0;

			if (length > top_)
				length = top_;

			if (position + length > top_)
				position = top_ - length;

			if (position > 0)
			{
				//for (int j = position + length - 1, k = position - 1;
				//	j < top_;
				//	j++, k++)
				//	stack_[k] = stack_[j];

				for (int j = top_ - position, k = (top_ - position - 1) - (length - 1);
					j < top_;
					j++, k++)
					stack_[k] = stack_[j];
			}

			top_ -= length;
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
		inline void replace(T& other, int n)
		{
			if (n >= N)
			{
				std::stringstream error_message;
				error_message << "reference Utilities::FixedSizeStack::replace() - Stack overflow.  index = " << n;

				throw std::overflow_error(error_message.str());
			}

			n = (n < 0) ? 0 : n;
			n = (n > top_) ? top_ - 1 : n;

			stack_[top_ - n - 1] = other;
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
		inline void replace_section(int source_position, int target_position, int length)
		{
			if (source_position < 0)
			{
				std::stringstream error_message;
				error_message << "reference Utilities::FixedSizeStack::replace_section() - Stack underflow.  source_position = " << source_position;

				throw std::underflow_error(error_message.str());
			}

			if (target_position < 0)
			{
				std::stringstream error_message;
				error_message << "reference Utilities::FixedSizeStack::replace_section() - Stack underflow.  target_position = " << target_position;

				throw std::underflow_error(error_message.str());
			}

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
			int source_index = top_ - source_position;
			int target_index = top_ - target_position;

			// Copy source to target
			if (source_index > target_index)
			{
				for (int n = 0; n < length; n--)
					stack_[target_index + n] = stack_[source_index + n];
			}
			 
			top_ = target_index + length;
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
		inline void yankdup_item(int position, int length)
		{
			if (position < 0)
				position = 0;

			if (position > top_)
				position = top_;

			if (length < 0)
				position = 0;

			if (length > top_)
				length = top_;

			if (top_ - position - length >= 0)
			{
				for (int i = 0, j = top_ - position - length, k = top_;
					(i < length) && (k < N);
					i++, j++, k++)
					stack_[k] = stack_[j];

				top_ += length;
			}
		}

		// Returns the number of elements in the underlying container
		inline size_type size() const
		{
			return top_;
		}

		// Returns the number of available elements in the underlying container
		inline size_type free() const
		{
			return N - top_ - 1;
		}

		//virtual unsigned int split(Utilities::FixedSizeStack<T> &left_half,
		//	Utilities::FixedSizeStack<T> &right_half, 
		//	unsigned int split_position) 
		//{ 
		//	return 0; 
		//};

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

		inline const_reference get_atom(int position) const
		{
			if (top_ == 0)
			{
				std::stringstream error_message;
				error_message << "Utilities::FixedSizeStack::get_item() - Stack empty.";

				throw std::underflow_error(error_message.str());
			}

			position = (position < 0) ? 0 : position;
			position = (position >= top_) ? top_ - 1: position;

			return stack_[top_ - position - 1];
		}

		inline reference get_atom(int position) 
		{
			if (top_ == 0)
			{
				std::stringstream error_message;
				error_message << "Utilities::FixedSizeStack::get_item() - Stack empty.";

				throw std::underflow_error(error_message.str());
			}

			position = (position < 0) ? 0 : position;
			position = (position >= top_) ? top_ - 1 : position;

			return stack_[top_ - position - 1];
		}

		// Purpose: 
		//   Returns a reference to the genome container (the atom array)
		//
		// Parameters:
		//   None
		// 
		// Return value:
		//   Reference to the genome's FixedSizeStack object
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  
		//
		// Remarks:
		//
		inline T& get_atom_ref(int position)
		{
			return get_atom(position);
		}


	protected:
		// Zero-based index to the first empty slot on the stack
		size_type top_ = 0;

		// The containter for the stack
		std::array<T, N> stack_;
	};
}