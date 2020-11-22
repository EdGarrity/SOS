#pragma once
#include <string>
#include <vector>
#include <stack>
#include <map>
#include "..\Utilities\FixedSizeStack.h"
#include "..\Utilities\String.h"
//#include "Atom.h"

// Favor blocks over atoms algorithm:
// - An atoms is considered a single littoral or instruction.
// - A block is zero or more atoms surrounded by parenthesis.
// - An item is an atom or a block
// - A genome is a list of one or more items.  A genome must end in a block to mark the end of the genome if not the last genome.
// - A genome is processed as a list object, i.e., an open parenthesis is assumed to exist before the first item on the stack.
// - A genome block level starts at 0 (after first implied open parenthesis) and increments for each nested block
// - Nested blocks begin with the item after a block requiring items
// - Closing parenthesis on block 0 is interpreted as an close - open instruction
// - Closing parenthesis on any other level is an open - close if the block level expects more items of a close if not.
// - Block levels are closed when all expected items are found

namespace Plush
{
	typedef std::map<std::string, unsigned int> Func2BlockWantsMapType;
	extern Func2BlockWantsMapType Func2BlockWantsMap;

	// Purpose: 
	//   Returns a reference to a section of the genome.
	//
	//   The genome is a list object.  A genome section is used to reference a sectrion of the genome.
	//
	template <class T>
	class Genome_section
	{
	public:
		// Position of first atom in genome (0 = top)
		unsigned int starting_position = 0;

		// Position of last atom in genome (0 = top)
		unsigned int ending_position = 0;

		// Number of atomes in genome
		unsigned int size = 0;

		// Number of extra closing parenthesis in genome
		unsigned int extra_parenthesis = 0;

		// Default constructor
		Genome_section()
		{
			this->starting_position = 0;
			this->ending_position = 0;
			this->size = 0;
			this->extra_parenthesis = 0;
		}

		// Copy constructor (From CodeAtom)
		Genome_section(const Genome_section<CodeAtom>& other)
		{
			this->starting_position = other.starting_position;
			this->ending_position = other.ending_position;
			this->size = other.size;
			this->extra_parenthesis = other.extra_parenthesis;
		}

		// Copy constructor (From ExecAtom)
		Genome_section(const Genome_section<ExecAtom>& other)
		{
			this->starting_position = other.starting_position;
			this->ending_position = other.ending_position;
			this->size = other.size;
			this->extra_parenthesis = other.extra_parenthesis;
		}

		// Purpose: 
		//   Constructs a reference to a section of the genome
		//
		// Parameters:
		//   starting_position	- 0-based position of the first atom in the genome section
		//   size				- Number of atoms in the genome section
		//   extra_parenthesis	- Number of extra parenthesis at the end of the genome
		//
		// Return value:
		//   Genome section object
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  
		//
		// Remarks:
		//
		Genome_section(unsigned int starting_position, unsigned int size, unsigned int extra_parenthesis = 0)
		{
			this->starting_position = starting_position;
			this->size = size;
			this->extra_parenthesis = extra_parenthesis;

			if (size > 0)
				this->ending_position = starting_position + size - 1;

			else
				this->ending_position = starting_position;
		}

		// Purpose: 
		//   Sets a reference to a section of the genome
		//
		// Parameters:
		//   starting_position	- 0-based position of the first atom in the genome section
		//   size				- Number of atoms in the genome section
		//   extra_parenthesis	- Number of extra parenthesis at the end of the genome
		// 
		// Return value:
		//   None
		//
		// Side Effects:
		//   Genome section references provided section
		//
		// Thread Safe:
		//   Yes.  
		//
		// Remarks:
		//
		inline void set(unsigned int starting_position, unsigned int size, unsigned int extra_parenthesis = 0)
		{
			this->starting_position = starting_position;
			this->size = size;
			this->extra_parenthesis = extra_parenthesis;

			if (size > 0)
				this->ending_position = starting_position + size - 1;

			else
				this->ending_position = starting_position;
		}

		// Purpose: 
		//   Clears genome section
		//
		// Parameters:
		//   None
		// 
		// Return value:
		//   None
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  
		//
		// Remarks:
		//
		inline void clear()
		{
			this->starting_position = 0;
			this->ending_position = 0;
			this->size = 0;
			this->extra_parenthesis = 0;
		}

		// Purpose: 
		//   Advances the genome section reference to the next Nth atom in the genome section.  The
		//   genome section reference is left unchanged if advancing the reference by N would push
		//   the reference past the end of the genome section
		//
		// Parameters:
		//   n - The number of atoms to advance
		// 
		// Return value:
		//   A new genome section advanced by N
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  
		//
		// Remarks:
		//
		const Genome_section<T> operator+ (int n)
		{
			if (starting_position + n > ending_position)
			{
				std::stringstream error_message;
				error_message << "const_reference Genome_ref::operator+(int n) - Stack overflow.  n = " << n;

				throw std::overflow_error(error_message.str());
			}

			return Genome_section<T>(starting_position + n, size - n);
		}

		// Purpose: 
		//   Advances the genome section reference to the next atom in the genome section.  The
		//   genome section reference is left unchanged if advancing the reference by 1 would push
		//   the reference past the end of the genome section
		//
		// Parameters:
		//   None
		// 
		// Return value:
		//   Pointer to the current genome section after it has been advanced by 1 atom
		//
		// Side Effects:
		//   Current genome section is advanced by 1 atom
		//
		// Thread Safe:
		//   Yes.  
		//
		// Remarks:
		//
		Genome_section<T>& operator++ ()
		{
			if (starting_position + 1 > ending_position)
			{
				std::stringstream error_message;
				error_message << "const_reference Genome_ref::operator++ - Stack overflow.";

				throw std::overflow_error(error_message.str());
			}

			if (size > 1)
			{
				starting_position++;
				size--;
			}

			return *this;
		}

		// Purpose: 
		//   Advances the genome section reference to the next atom in the genome section.  The
		//   genome section reference is left unchanged if advancing the reference by 1 would push
		//   the reference past the end of the genome section
		//
		// Parameters:
		//   None
		// 
		// Return value:
		//   Pointer to the current genome section after it has been advanced by 1 atom
		//
		// Side Effects:
		//   Current genome section is advanced by 1 atom
		//
		// Thread Safe:
		//   Yes.  
		//
		// Remarks:
		//
		Genome_section<T>& operator++ (int n)
		{
			Genome_section<T> temp(*this);

			if (starting_position + 1 > ending_position)
			{
				std::stringstream error_message;
				error_message << "const_reference Genome_ref::operator++(int n) - Stack overflow.  n = " << n;

				throw std::overflow_error(error_message.str());
			}

			if (size > 1)
			{
				starting_position++;
				size--;
			}

			return temp;
		}
	};

	template <class T, size_t N = domain::argmap::maximum_stack_size>
	class Genome : public Utilities::FixedSizeStack<T>
	{
	private:
		typedef typename std::array<T, N>::value_type value_type;
		typedef typename std::array<T, N>::reference reference;
		typedef typename std::array<T, N>::const_reference const_reference;
		typedef typename std::array<T, N>::size_type size_type;

		// Human readable version of the Plush genome
		std::string genome_string_;

		// Convert genome to a Human readable string
		void convert_genome_to_string();

		// Ingest genome string
		void ingest_plush_genome(std::string _genome_str);

	public:
		// Default constructor
		Genome()
		{
			Utilities::FixedSizeStack<T>::top_ = 0;
		}

		// Purpose: 
		//   Copy constructor
		//
		//   A new Genome object with a new Fixed Stack object is constructed and populated with the 
		//   stack elemets of the provided genome.
		//
		// Parameters:
		//   other - Pointer to genome to copy
		// 
		// Return value:
		//   New genome
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  
		//
		// Remarks:
		//
		inline Genome(Genome *other)
		{
			if (other->top_ >= N)
			{
				std::stringstream error_message;
				error_message << "Utilities::Genome::Genome() - Stack overflow.  top = " << other->top_;

				throw std::overflow_error(error_message.str());
			}

			Utilities::FixedSizeStack<T>::top_ = other->top_;

			for (int n = 0; n < Utilities::FixedSizeStack<T>::top_; n++)
				Utilities::FixedSizeStack<T>::stack_[n] = other->stack_[n];
		}

		// Purpose: 
		//   Copy constructor
		//
		//   A new Genome object with a new Fixed Stack object is constructed and populated with the 
		//   stack elemets of the provided genome.
		//
		// Parameters:
		//   other - Reference of genome to copy
		// 
		// Return value:
		//   New genome
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  
		//
		// Remarks:
		//
		inline Genome(const Genome<T>& other)
		{
			copy(other);
		}

		// Purpose: 
		//   Copies the provided Genome into the current Genome
		//
		// Parameters:
		//   other - Reference of genome to copy
		// 
		// Return value:
		//   New genome
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  
		//
		// Remarks:
		//
		inline void copy(const Genome& other)
		{
			if (other.top_ > N)
			{
				std::stringstream error_message;
				error_message << "Utilities::Genome::copy() - Stack overflow.  top = " << other.top_;

				throw std::overflow_error(error_message.str());
			}

			Utilities::FixedSizeStack<T>::top_ = other.top_;

			for (int n = 0; n < Utilities::FixedSizeStack<T>::top_; n++)
				Utilities::FixedSizeStack<T>::stack_[n] = other.stack_[n];

			genome_string_ = other.genome_string_;
		}

		// Purpose: 
		//   Copies the provided Genome into the current Genome
		//
		// Parameters:
		//   other - Reference of genome to copy
		// 
		// Return value:
		//   New genome
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  
		//
		// Remarks:
		//
		inline void operator= (const Genome& other)
		{
			copy(other);
		}

		// Purpose: 
		//   Initializez Genome with provided genome string
		//
		// Parameters:
		//   genome_string - The genome to use to initialize with
		// 
		// Return value:
		//   None
		//
		// Side Effects:
		//   Current genome is cleared and initialize using provided genome string
		//
		// Thread Safe:
		//   Yes.  
		//
		// Remarks:
		//
		void set(std::string genome_string);

		// Same as the copy method
		inline void set(Genome<T, N>& other)
		{
			if (other.top_ >= N)
			{
				std::stringstream error_message;
				error_message << "Utilities::Genome::set() - Stack overflow.  top = " << other.top_;

				throw std::overflow_error(error_message.str());
			}

			Utilities::FixedSizeStack<T>::top_ = other.top_;

			for (int n = 0; n < Utilities::FixedSizeStack<T>::top_; n++)
				Utilities::FixedSizeStack<T>::stack_[n] = other.stack_[n];
		}

		// Purpose: 
		//   Clears the genome fields
		//
		// Parameters:
		//   None
		// 
		// Return value:
		//   None
		//
		// Side Effects:
		//   Genome is erased.
		//
		// Thread Safe:
		//   Yes.  
		//
		// Remarks:
		//
		inline void clear()
		{
			Utilities::FixedSizeStack<T>::clear();
			genome_string_.clear();
		};

		// Purpose: 
		//   Returns a reference to the genome in element Nth of the stack. 
		//
		//   This is a stack operation.  Each element of the stack is a genome.  This function will return 
		//   the genome located at element N on the stack where 0 is the top level of the stack.
		//
		//   If N is less than or equal to 0, then a reference to the genome at the top of the stack is 
		//   returned.  If N is greater than the number of elements in the stack, then a reference to the
		//   genome at the bottom of the stack is returned. 
		//
		// Parameters:
		//   n - Stack element to return (zero-based).
		// 
		// Return value:
		//   Reference to genome in requested stack element
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  
		//
		// Remarks:
		//
		inline const Genome_section<T> operator [] (unsigned int item_position) const
		{
			int s = 0;
			int l = 0;
			unsigned int extra_closing_parenthesis;

			// Find index to top of item after target item
			for (int n = 0; n <= item_position; n++)
			{
				s += l;
				l = number_of_atoms(extra_closing_parenthesis, n);
			}

			return Genome_section<T>(s, l, extra_closing_parenthesis);
		}

		// Purpose: 
		//   Returns a reference to the genome in element Nth of the stack. 
		//
		//   This is a stack operation.  Each element of the stack is a genome.  This function will return 
		//   the genome located at element N on the stack where 0 is the top level of the stack.
		//
		//   If N is less than or equal to 0, then a reference to the genome at the top of the stack is 
		//   returned.  If N is greater than the number of elements in the stack, then a reference to the
		//   genome at the bottom of the stack is returned. 
		//
		// Parameters:
		//   n - Stack element to return (zero-based).
		// 
		// Return value:
		//   Reference to genome in requested stack element
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  
		//
		// Remarks:
		//
		inline Genome_section<T> operator [] (unsigned int item_position)
		{
			int s = 0;
			int l = 0;
			unsigned int extra_closing_parenthesis;

			// Find index to top of item after target item
			for (int n = 0; n <= item_position; n++)
			{
				s += l;
				l = number_of_atoms(extra_closing_parenthesis, n);
			}

			return Genome_section<T>(s, l, extra_closing_parenthesis);
		}

		// Purpose: 
		//   Checks if the contents of lhs and rhs are equal, that is, they have the same number of atom 
		//   and each atom in lhs compares equal with the atom in rhs at the same position.
		//
		// Parameters:
		//   lhs, rhs - genomes whose contents to compare
		// 
		// Return value:
		//   true if the contents of the genomes are equal, false otherwise
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  
		//
		// Remarks:
		//
		inline bool operator==(Genome<T> &other_genome) const
		{
			return comp(other_genome);
		}

		// Purpose: 
		//   Compare genome with provided genome and return True if they match
		//
		// Parameters:
		//   other_genome	- Reference to the genome to compare
		// 
		// Return value:
		//   True if the genomes match
		//   False if they do not match
		//
		// Side Effects:
		//   None.
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		inline bool comp(Genome_section<T> genome_a, Genome_section<T> genome_b)
		{
			if (genome_a.size != genome_b.size)
				return false;

			for (int n = 0; n < genome_a.size; n++)
			{
				unsigned int pos_a = genome_a.ending_position - n;
				unsigned int pos_b = genome_b.ending_position - n;

				T atom_a = Utilities::FixedSizeStack<T>::get_atom(pos_a);
				T atom_b = Utilities::FixedSizeStack<T>::get_atom(pos_b);

				if (atom_a.like(atom_b) == false)
					return false;
			}

			return true;
		}

		// Purpose: 
		//   Checks if the contents of lhs and rhs are not equal, that is, they don't have the same 
		//   number of atom or any atom in lhs is not equal with the atom in rhs at the same position.
		//
		// Parameters:
		//   lhs, rhs - genomes whose contents to compare
		// 
		// Return value:
		//   true if the contents of the genomes are not equal, false otherwise
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  
		//
		// Remarks:
		//
		inline bool operator!=(Genome<T> &other_genome) const
		{
			return !comp(other_genome);
		}

		// Purpose: 
		//   Returns the genome program as a string
		//
		// Parameters:
		//   None
		// 
		// Return value:
		//   Genome program as a std::string object
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  
		//
		// Remarks:
		//
		inline const std::string to_string()
		{
			if (genome_string_.empty())
				convert_genome_to_string();

			return genome_string_;
		}

		// Purpose: 
		//   Returns the number of blocks on the top of the genome stack. 
		//
		//   The genome is processed as a list object, i.e., an open parenthesis is assumed to exist 
		//   before the first item on the stack.  This function returns the number of items in the top 
		//   level of the list; that is, nested lists contribute only 1 to this count, no matter what 
		//   they contain.  Closing parenthesis can either be interpreted as close instructions (to satisfy 
		//   a block requirement in a nested list) or as close - open instructions (for the top level list).
		//   Nested levels begin when an instruction requiring blocks is encountered in the list and end 
		//   when all required blocks are found.
		//
		//   This is equivalent to the Push Stack level.  Each block on the FixedSize stack is a 
		//   level on the Push Stack.
		//
		// Parameters:
		//   None
		// 
		// Return value:
		//   Number of blocks in the top level of the list
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		unsigned int number_of_blocks()
		{
			unsigned int item_number = 0;
			unsigned int wanted_blocks = 1;	// Assume all genomes begin as a block.
			unsigned int extra_blocks = 0;
			std::stack<unsigned int> wanted_stack;
			int n = Utilities::FixedSizeStack<T>::size() - 1;

			while ((n >= 0) || (extra_blocks > 0))
			{
				Plush::Atom atom;

				if (extra_blocks == 0)
				{
					atom = Utilities::FixedSizeStack<T>::stack_[n];
					n--;
				}
				else
				{
					atom = Plush::Atom("{:instruction EXEC.NOOP :close 1}");
					extra_blocks--;
				}

				if ((n < 0) && (extra_blocks <= 0) && (atom.close_parenthesis == 0))
					atom.close_parenthesis = 1;

				int closing = atom.close_parenthesis - Func2BlockWantsMap[atom.instruction];

				if (closing < 0)
				{
					wanted_stack.push(wanted_blocks);
					wanted_blocks = 0 - closing;
				}

				extra_blocks = (closing > 1) ? (closing - 1) : (0);

				if (closing > 0)
				{
					if (wanted_blocks > 0)
						wanted_blocks--;
				}

				if (wanted_blocks == 0)
				{
					if (wanted_stack.size() == 0)
						item_number++;

					if (wanted_stack.size() > 0)
					{
						wanted_blocks = wanted_stack.top();
						wanted_stack.pop();
					}
				}
			}

			return item_number;
		}

		// Purpose: 
		//   Returns the number of items in the genome stack. 
		//
		//   The genome is processed as a list object, i.e., an open parenthesis is assumed to exist 
		//   before the first item on the stack.  This function returns the number of items in the top 
		//   level of the list; that is, nested lists contribute only 1 to this count, no matter what 
		//   they contain.  Closing parenthesis can either be interpreted as close instructions (to satisfy 
		//   a block requirement in a nested list) or as close - open instructions (for the top level list).
		//   Nested levels begin when an instruction requiring blocks is encountered in the list and end 
		//   when all required blocks are found.
		//
		// Parameters:
		//   starting_position	- Position of first atom in genome for sub-genome search.  0 = top 
		//                        of stack.  Do not provide for a top level search.
		// 
		// Return value:
		//   Number of items and blocks in the top level of the list
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		unsigned int number_of_items(int starting_position = -1)
		{
			unsigned int item_number = 0;
			unsigned int wanted_blocks = 0;
			unsigned int extra_blocks = 0;
			std::stack<unsigned int> wanted_stack;
			int n = Utilities::FixedSizeStack<T>::size() - 1;

			if (starting_position > -1)
			{
				wanted_blocks = 1;
				n -= starting_position;
			}

			while (n >= 0)
			{
				Plush::Atom atom;

				if (extra_blocks == 0)
				{
					atom = Utilities::FixedSizeStack<T>::stack_[n];
					n--;
				}
				else
					atom = Plush::Atom("{:instruction EXEC.NOOP :close 1}");

				int closing = atom.close_parenthesis - Func2BlockWantsMap[atom.instruction];

				if (n < 0)
					closing = wanted_blocks;

				if (closing < 0)
				{
					wanted_stack.push(wanted_blocks);
					wanted_blocks = 0 - closing;
				}

				extra_blocks = (closing > 1) ? (closing - 1) : (0);

				if (closing > 0)
				{
					if (wanted_blocks > 0)
						wanted_blocks--;

					else if ((wanted_blocks == 0) && (wanted_stack.size() == 0))
						break;
				}

				if (wanted_blocks == 0)
				{
					if (wanted_stack.size() > 0)
					{
						wanted_blocks = wanted_stack.top();
						wanted_stack.pop();
					}
					else
						wanted_blocks = 0;

					if (wanted_stack.size() == 0)
						item_number++;
				}
			}

			return item_number;
		}

		// Purpose: 
		//   Returns the number of items in the genome section. 
		//
		//   The genome is processed as a list object, i.e., an open parenthesis is assumed to exist 
		//   before the first item on the stack.  This function returns the number of items in the top 
		//   level of the list; that is, nested lists contribute only 1 to this count, no matter what 
		//   they contain.  Closing parenthesis can either be interpreted as close instructions (to satisfy 
		//   a block requirement in a nested list) or as close - open instructions (for the top level list).
		//   Nested levels begin when an instruction requiring blocks is encountered in the list and end 
		//   when all required blocks are found.
		//
		// Parameters:
		//   section - Genome section to search.
		// 
		// Return value:
		//   Number of items and blocks in the top level of the Genome section
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		unsigned int number_of_items(Genome_section<T> section)
		{
			unsigned int item_number = 0;
			unsigned int wanted_blocks = 0;
			unsigned int extra_blocks = 0;
			std::stack<unsigned int> wanted_stack;
			int n = Utilities::FixedSizeStack<T>::top_ - section.starting_position - 1;

			while (n >= 0)
			{
				Plush::Atom atom;

				if (extra_blocks == 0)
				{
					atom = Utilities::FixedSizeStack<T>::stack_[n];
					n--;
				}
				else
					atom = Plush::Atom("{:instruction EXEC.NOOP :close 1}");

				int closing = atom.close_parenthesis - Func2BlockWantsMap[atom.instruction];

				if (n < 0)
					closing = wanted_blocks;

				if (closing < 0)
				{
					wanted_stack.push(wanted_blocks);
					wanted_blocks = 0 - closing;
				}

				extra_blocks = (closing > 1) ? (closing - 1) : (0);

				if (closing > 0)
				{
					if (wanted_blocks > 0)
						wanted_blocks--;

					else if ((wanted_blocks == 0) && (wanted_stack.size() == 0))
					{
						item_number++;
						break;
					}
				}

				if (wanted_blocks == 0)
				{
					if (wanted_stack.size() > 0)
					{
						wanted_blocks = wanted_stack.top();
						wanted_stack.pop();
					}
					else
						wanted_blocks = 0;

					if (wanted_stack.size() == 0)
						item_number++;
				}
			}

			return item_number;
		}

		// Purpose: 
		//   Returns the number of atoms in the Nth top level block on the stack
		//
		//   For example, 
		//		if N = 0 and the top genome is "( A B )" then this returns 2. 
		//		If N = 0 and the top genome is "( ( A B ) C )" then this returns 3.
		//		If N = 1 and the genome stack is  "( A )( ( A B ) C )" then this returns 3.
		//		If N = 2 and the genome stack is  "( A B )( ( A B ) C )" then this returns 3.
		//		If N = 1 and the genome stack is  "( A )()( ( A B ) C )" then this returns 0.
		//
		// Parameters:
		//   extra_blocks		- Where to put the number of extra closing parenthesis at the end of the search
		//   item_number		- Top block number to count the number of atoms in.  0 is the first block on 
		//                        the stack.
		//   starting_position	- Position of first atom in genome for sub-genome search.  0 = top of 
		//                        stack.  Do not provide for a top level search.
		// 
		// Return value:
		//   Number of atoms in the top genome
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//   All callers of this method always use it for a top level search.  I.e, no callers ever 
		//   provide the starting_position parameter.  I am keeping this parameter available in 
		//   case I ever have a need for it.
		//
		inline unsigned int number_of_atoms(unsigned int& extra_blocks_returned, int item_number = 0, int starting_position = -1)
		{
			unsigned int wanted_blocks = 0;
			unsigned int extra_blocks = 0;
			std::stack<unsigned int> wanted_stack;
			int atom_count = 0;
			int block_starting_index = Utilities::FixedSizeStack<T>::size() - 1;

			if (starting_position > -1)
			{
				wanted_blocks = 0;
				block_starting_index -= starting_position;
			}

			for (int item = 0; item <= item_number; item++)
			{
				atom_count = 0;

				if (extra_blocks > 0)
					extra_blocks--;

				else
				{
					int block_ending_index = 0;

					for (int i = block_starting_index; i >= 0; i--)
					{
						block_ending_index = i;

						T atom;

						if (extra_blocks == 0)
						{
							atom = Utilities::FixedSizeStack<T>::container()[i];
							atom_count++;
						}
						else
						{
							atom = T("{:instruction EXEC.NOOP :close 1}");
							extra_blocks--;
							i++;
							block_ending_index++;
						}

						int closing = atom.close_parenthesis - Func2BlockWantsMap[atom.instruction];

						if (closing < 0)
						{
							wanted_stack.push(wanted_blocks);
							wanted_blocks = 0 - closing;
						}

						extra_blocks += (closing > 1) ? (closing - 1) : (0);

						if (closing > 0)
						{
							if (wanted_blocks > 0)
								wanted_blocks--;

							else if ((wanted_blocks == 0) && (wanted_stack.size() == 0))
								break;
						}

						if (wanted_blocks == 0)
						{
							if (wanted_stack.size() > 0)
							{
								wanted_blocks = wanted_stack.top();
								wanted_stack.pop();
							}
						}
					}

					block_starting_index = block_ending_index - 1;
				}
			}

			extra_blocks_returned = extra_blocks;

			return atom_count;
		};

		// Purpose: 
		//   Returns the number of atoms in the block which begins at the position in provided parameter 
		//
		// Parameters:
		//   extra_blocks		- Where to put the number of extra closing parenthesis at the end of the search
		//   starting_position	- Position of first atom in genome for sub-genome search.  0 = top 
		//                        of stack.  Do not provide for a top level search.
		// 
		// Return value:
		//   Number of atoms in the genome that starts at N
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		unsigned int number_of_atoms_in_Nth_block(unsigned int& extra_blocks_returned, int starting_position = -1, bool top_block = false)
		{
			int atom_count = 0;
			unsigned int item_number = 0;
			unsigned int wanted_blocks = 0;
			unsigned int extra_blocks = 0;
			Utilities::FixedSizeStack<CodeAtom> temp;
			std::stack<unsigned int> wanted_stack;

			int block_starting_index = Utilities::FixedSizeStack<T>::size() - 1;

			if (starting_position > -1)
			{
				wanted_blocks = 1;
				block_starting_index -= starting_position;
			}

			for (int i = block_starting_index; i >= 0; i--)
			{
				T atom;

				if (extra_blocks == 0)
				{
					atom = Utilities::FixedSizeStack<T>::container()[i];
					atom_count++;
				}
				else
				{
					atom = T("{:instruction EXEC.NOOP :close 1}");
					extra_blocks--;
					i++;
				}

				int closing = atom.close_parenthesis - Func2BlockWantsMap[atom.instruction];

				if (closing < 0)
				{
					wanted_stack.push(wanted_blocks);
					wanted_blocks = 0 - closing;
				}

				extra_blocks += (closing > 1) ? (closing - 1) : (0);

				if (closing > 0)
				{
					if (wanted_blocks > 0)
						wanted_blocks--;

					if ((wanted_blocks == 0) && (wanted_stack.size() == (top_block ? 0 : 1)))
						break;
				}

				if (wanted_blocks == 0)
				{
					if (wanted_stack.size() == 0)
						break;

					if (wanted_stack.size() > 0)
					{
						wanted_blocks = wanted_stack.top();
						wanted_stack.pop();
					}
				}
			}

			extra_blocks_returned = extra_blocks;

			return atom_count;
		};

		// Purpose: 
		//   Returns Nth item in top genome
		//
		// Parameters:
		//   item_number - Item number to return.   Use N = 0 for entire top genome.  Use N = 1 for first item.
		//
		// Return value:
		//    Genome section of Nth item
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		Genome_section<T> get_subitem(int item_number)
		{
			Genome_section<T> subsection;
			int atom_count = 0;
			unsigned int extra_blocks = 0;
			Utilities::FixedSizeStack<CodeAtom> temp;

			int item_ending_position = 0;
			int item_length = 0;

			int search_starting_index = Utilities::FixedSizeStack<T>::size() - 1;
			int search_ending_index = search_starting_index;

			for (int n = 0; n < item_number; n++)
			{
				unsigned int wanted_blocks = 0;
				std::stack<unsigned int> wanted_stack;

				for (int i = search_starting_index; i >= 0; i--)
				{
					search_ending_index = i;

					T atom;

					if (extra_blocks == 0)
					{
						atom = Utilities::FixedSizeStack<T>::container()[i];
						atom_count++;
					}
					else
					{
						atom = T("{:instruction EXEC.NOOP :close 1}");
						extra_blocks--;
						i++;
					}

					int closing = atom.close_parenthesis - Func2BlockWantsMap[atom.instruction];

					if (closing < 0)
					{
						wanted_stack.push(wanted_blocks);
						wanted_blocks = 0 - closing;
					}

					extra_blocks += (closing > 1) ? (closing - 1) : (0);

					if (closing > 0)
					{
						if (wanted_blocks > 0)
							wanted_blocks--;

						if ((wanted_blocks == 0) && (wanted_stack.size() == 1))
							break;
					}

					if (wanted_blocks == 0)
					{
						if (wanted_stack.size() == 0)
							break;

						if (wanted_stack.size() > 0)
						{
							wanted_blocks = wanted_stack.top();
							wanted_stack.pop();
						}
					}
				}

				item_length = search_starting_index - search_ending_index + 1;
				item_ending_position += item_length;

				search_starting_index -= item_length;
			}

			subsection.set(item_ending_position - item_length, item_length, extra_blocks);
			return subsection;
		};

		// Purpose: 
		//   Returns the item which begins at the specified position 
		//
		// Parameters:
		//   search_starting_position	- Position of first atom in item.  0 = top of stack.
		// 
		// Return value:
		//   genome_section of found item
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		Genome_section<T> get_item_that_starts_at_position(int search_starting_position)
		{
			Genome_section<T> subsection;
			int atom_count = 0;
			unsigned int extra_blocks = 0;

			int item_ending_position = 0;
			int item_length = 0;

			int search_starting_index = Utilities::FixedSizeStack<T>::size() - search_starting_position - 1;
			int search_ending_index = search_starting_index;

			unsigned int wanted_blocks = 0;
			std::stack<unsigned int> wanted_stack;

			for (int i = search_starting_index; i >= 0; i--)
			{
				T atom;

				if (extra_blocks == 0)
				{
					atom = Utilities::FixedSizeStack<T>::container()[i];
					atom_count++;
				}
				else
				{
					atom = T("{:instruction EXEC.NOOP :close 1}");
					extra_blocks--;
					i++;
				}

				search_ending_index = i;

				int closing = atom.close_parenthesis - Func2BlockWantsMap[atom.instruction];

				if (closing < 0)
				{
					wanted_stack.push(wanted_blocks);
					wanted_blocks = 0 - closing;
				}

				extra_blocks += (closing > 1) ? (closing - 1) : (0);

				if (closing > 0)
				{
					if (wanted_blocks > 0)
						wanted_blocks--;

					if ((wanted_blocks == 0) && (wanted_stack.size() == 1))
						break;
				}

				if (wanted_blocks == 0)
				{
					if (wanted_stack.size() == 0)
						break;

					if (wanted_stack.size() > 0)
					{
						wanted_blocks = wanted_stack.top();
						wanted_stack.pop();
					}
				}
			}

			item_length = search_starting_index - search_ending_index + 1;
			item_ending_position += item_length;

			search_starting_index -= item_length;

			subsection.set(search_starting_position, item_length, extra_blocks);
			return subsection;
		};

		// Purpose: 
		//   Push an atom on the stack
		//
		// Parameters:
		//   atom	- Reference to genome to push
		// 
		// Return value:
		//   None
		//
		// Side Effects:
		//   Stack updated with provided atom at the top of the stack.
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		inline void push(long value)
		{
			Utilities::FixedSizeStack<T>::push(value);
		}

		inline void push(double value)
		{
			Utilities::FixedSizeStack<T>::push(value);
		}

		inline void push(bool value)
		{
			Utilities::FixedSizeStack<T>::push(value);
		}

		inline void push(CodeAtom atom)
		{
			if (Utilities::FixedSizeStack<T>::top_ > 0)
			{
				T top_atom = Utilities::FixedSizeStack<T>::get_top();

				if ((top_atom.instruction == "EXEC.NOOP")
					&& (top_atom.type == Atom::AtomType::ins)
					&& (Utilities::FixedSizeStack<T>::top_ > 0))
				{
					atom.close_parenthesis += top_atom.close_parenthesis;
					Utilities::FixedSizeStack<T>::pop();
				}

				Utilities::FixedSizeStack<T>::push(atom);
			}

			else
				Utilities::FixedSizeStack<T>::push(atom);
		}

		inline void push(ExecAtom atom)
		{
			if (Utilities::FixedSizeStack<T>::top_ > 0)
			{
				T top_atom = Utilities::FixedSizeStack<T>::get_top();

				if ((top_atom.instruction == "EXEC.NOOP")
					&& (top_atom.type == Atom::AtomType::ins)
					&& (Utilities::FixedSizeStack<T>::top_ > 0))
				{
					atom.close_parenthesis += top_atom.close_parenthesis;
					Utilities::FixedSizeStack<T>::pop();
				}

				Utilities::FixedSizeStack<T>::push(atom);
			}

			else
				Utilities::FixedSizeStack<T>::push(atom);
		}

		// This function uses absolute index instead of relative index and therefore will not work.
		//inline void push_genome(Genome_section<T> section)
		//{
		//	for (int n = 0; n < section.size; n++)
		//		push(container()[section.ending_position + n]);
		//}

		// Purpose: 
		//   Pop an atom from the stack
		//
		// Parameters:
		//   None
		// 
		// Return value:
		//   CodeAtom or ExecAtom poped from the stack
		//
		// Side Effects:
		//   The top atom is poped from the stack.
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		inline T pop()
		{
			T atom = Utilities::FixedSizeStack<T>::get_top();
			Utilities::FixedSizeStack<T>::pop();
			return atom;
		}

		// Purpose: 
		//   Pop a genome from the stack and returns a reference to the poped genome.  Warning, the 
		//   returned reference is only valid until the next stack manipulation operation.
		//
		// Parameters:
		//   None
		// 
		// Return value:
		//   Genome_section - Reference to poped genome.
		//
		// Side Effects:
		//   The top genome is poped from the stack.
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		inline Genome_section<T> pop_genome()
		{
			Genome_section<T> section = (*this)[0];
			
			Utilities::FixedSizeStack<T>::top_ -= section.size;

			if (section.extra_parenthesis > 0)
			{
				T atom = T("{:instruction EXEC.NOOP :close 0}");
				atom.close_parenthesis = section.extra_parenthesis;
				push(atom);
			}

			return section;
		};

		// Purpose: 
		//   Peeks a genome from the stack and returns a reference to the genome.  Warning, the 
		//   returned reference is only valid until the next stack manipulation operation.
		//
		// Parameters:
		//   None
		// 
		// Return value:
		//   Genome_section - Reference to top genome.
		//
		// Side Effects:
		//   The top genome is poped from the stack.
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		inline Genome_section<T> peek_genome()
		{
			Genome_section<T> block = (*this)[0];

			return block;
		};

		// Purpose: 
		//   Returns the top atom on the stack
		//
		// Parameters:
		//   None
		// 
		// Return value:
		//   CodeAtom or ExecAtom from the stack
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		inline T get_top()
		{
			T atom = Utilities::FixedSizeStack<T>::get_top();
			return atom;
		}

		inline T& get_top_ref()
		{
			return Utilities::FixedSizeStack<T>::get_top();
		}

		// Purpose: 
		//   Returns a reference to the bottom atom on the specified item
		//
		// Parameters:
		//   position - Index of item.  0 refers to the top item on the stack.
		// 
		// Return value:
		//   Reference to CodeAtom or ExecAtom from the bottom of the item
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		inline T& get_bottom_atom(int position)
		{
			int s = 0;
			int l = 0;
			unsigned int extra_blocks;

			// Find index to top of item after target item
			for (int n = 0; n <= position + 1; n++)
			{
				s += l;
				l = number_of_atoms(extra_blocks, n);
			}

			return Utilities::FixedSizeStack<T>::get_atom(s - 1);
		}

		// Purpose: 
		//   Returns stack size of item
		//
		// Parameters:
		//   item_index - Index of item.  0 refers to the top item on the stack.
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
		inline unsigned int item_size(int item_index)
		{
			int l = 0;
			unsigned int extra_blocks;

			l = number_of_atoms(extra_blocks, item_index);

			return l;
		}

		// Purpose: 
		//   Returns starting position of item in the stack
		//
		// Parameters:
		//   item_index - Index of item.  0 refers to the top item on the stack.
		// 
		// Return value:
		//   Starting position
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		inline unsigned int item_starting_position(int item_index)
		{
			int s = 0;
			int l = 0;
			unsigned int extra_blocks;

			// Find index to top of item after target item
			for (int n = 0; n <= item_index; n++)
			{
				s += l;
				l = number_of_atoms(extra_blocks, n);
			}

			return s;
		}

		// Purpose: 
		//   Returns starting position of Nth item in the top level block of stack
		//
		// Parameters:
		//   position - Index of item.  0 refers to the first item of the top block on the stack.
		// 
		// Return value:
		//   Starting position
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		//inline unsigned int subitem_starting_position(int position)
		//{
		//	int s = 0;
		//	int l = 0;
		//	unsigned int extra_blocks;

		//	// Find index to item after target item
		//	for (int n = 0; n <= position; n++)
		//	{
		//		l = number_of_atoms_in_Nth_block(extra_blocks, n);
		//		s += l;
		//	}

		//	return s - l;
		//}

		// Purpose: 
		//   Returns starting position of Nth item in the top level block of stack
		//
		// Parameters:
		//   position - Index of item.  0 refers to the first item of the top block on the stack.
		// 
		// Return value:
		//   Starting position
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		//inline Genome_section<T> get_subitem(int n)
		//{
		//	int s = 0;
		//	int l = 0;
		//	unsigned int extra_blocks;

		//	//for (int n = 0; n <= position; n++)
		//	//{
		//	//	l = number_of_atoms_in_Nth_item(extra_blocks, n);
		//	//	s += l;
		//	//}

		//	//return Genome_section<T>(s - l, l);

		//	Genome_section<T> subsection = get_subitem(extra_blocks, n);

		//	return subsection;
		//}

		// Purpose: 
		//   Returns position of last item of Nth item in the top level block of stack
		//
		// Parameters:
		//   position - Index of item.  0 refers to the first item of the top block on the stack.
		// 
		// Return value:
		//   Starting position
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		//inline unsigned int subitem_ending_position(int position)
		//{
		//	int s = 0;
		//	int l = 0;
		//	unsigned int extra_blocks;

		//	// Find index to top of item after target item
		//	for (int n = 0; n <= position; n++)
		//	{
		//		l = number_of_atoms_in_Nth_block(extra_blocks, n);
		//		s += l;
		//	}

		//	return s - 1;
		//}

		// Purpose: 
		//   Removes a top level item from the stack
		//
		// Parameters:
		//   item_position - Index of item to remove.  0 refers to the top item on the stack.
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
		inline void remove_item_at_position(int item_position)
		{
			int s = 0;
			int l = 0;
			unsigned int extra_blocks;

			for (int n = 0; n <= item_position; n++)
			{
				s += l;
				l = number_of_atoms(extra_blocks, n);
			}

			if ((l == 0) && (s > 0))
			{
				T& atom = Utilities::FixedSizeStack<T>::get_atom(s - 1);

				if (atom.close_parenthesis > 0)
					atom.close_parenthesis--;
			}

			else if (extra_blocks == 0)
				Utilities::FixedSizeStack<T>::remove_items(s, l);

			else
			{
				std::string atom_str = "{:instruction EXEC.NOOP :close ";
				atom_str += std::to_string(extra_blocks);
				atom_str += "}";
				T atom(atom_str);

				Utilities::FixedSizeStack<T>::replace(atom, s + l - 1);

				if (l > 1)
					Utilities::FixedSizeStack<T>::remove_items(s, l - 1);
			}
		}

		// Purpose: 
		//   Removes the genome at element N of the stack.
		//
		//   This is a stack operation.  Each element of the stack contains a genome.  This function will remove
		//   the genome in element N where N=0 is the top of the stack.
		//
		// Parameters:
		//   element_pos - Stack element to remove (zero-based).
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
		inline void remove_stack_element(int element_pos)
		{
			Genome_section<T> section = (*this)[element_pos];
			Utilities::FixedSizeStack<T>::remove_items(section.starting_position, section.size);

			if (section.extra_parenthesis > 0)
			{
				T& atom = Utilities::FixedSizeStack<T>::get_atom(section.starting_position - 1);
				int new_close_parenthesis = atom.close_parenthesis + section.extra_parenthesis;
				atom.close_parenthesis = new_close_parenthesis;
			}

			else if (section.size == 0)
			{
				T& atom = Utilities::FixedSizeStack<T>::get_atom(section.starting_position - 1);
				int new_close_parenthesis = atom.close_parenthesis - 1;

				if (atom.close_parenthesis > section.extra_parenthesis)
					atom.close_parenthesis = new_close_parenthesis;
			}
		}

		// Purpose: 
		//   Pushes a copy of item N on top of the stack.
		//
		// Parameters:
		//   item_position - Index of item to copy.  0 refers to the top item on the stack.
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
		inline void yankdup_item(int item_position)
		{
			int s = 0;
			int l = 0;
			unsigned int extra_blocks = 0;

			for (int n = 0; n <= item_position; n++)
			{
				s += l;
				l = number_of_atoms(extra_blocks, n);
			}

			if (l > 0)
				Utilities::FixedSizeStack<T>::yankdup_item(s, l);
			
			else 
				push(T("{:instruction EXEC.NOOP :close 1}"));

			if (Utilities::FixedSizeStack<T>::stack_[Utilities::FixedSizeStack<T>::size() - l].close_parenthesis > extra_blocks)
				Utilities::FixedSizeStack<T>::stack_[Utilities::FixedSizeStack<T>::size() - l].close_parenthesis -= extra_blocks;
		}

		// Purpose: 
		//   Pushes a copy of the genome at stack level N on top of the stack.
		//
		//   This is a stack operation.  Each element of the stack is a genome.  This function will copy 
		//   the genome located a stack level N where 0 is the top level of the stack.
		//
		// Parameters:
		//   element_pos = Position in stack of block to copy.  
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
		inline void yankdup_stack_element(int element_pos)
		{
			int s = 0;
			int l = 0;
			unsigned int extra_blocks = 0;

			Genome_section<T> section = (*this)[element_pos];

			if (section.size > 0)
				Utilities::FixedSizeStack<T>::yankdup_item(section.starting_position, section.size);

			else
				push(T("{:instruction EXEC.NOOP :close 1}"));

			int sz = Utilities::FixedSizeStack<T>::size();
			T& atom = Utilities::FixedSizeStack<T>::get_stack_element_ref(sz - 1);
			int new_close_parenthesis = atom.close_parenthesis - section.extra_parenthesis;

			if (atom.close_parenthesis > section.extra_parenthesis)
				atom.close_parenthesis = new_close_parenthesis;
		}

		inline void yankdup_stack_element(Genome_section<T> section)
		{
			if (section.size > 0)
				Utilities::FixedSizeStack<T>::yankdup_item(section.starting_position, section.size);

			else
				push(T("{:instruction EXEC.NOOP :close 1}"));
		}

		// Purpose: 
		//   Pushes a copy of the genome at stack level N on top of the stack.
		//
		//   This is a stack operation.  Each element of the stack is a genome.  This function will copy 
		//   the genome refernced by the genome section to the top of the stack.
		//
		// Parameters:
		//   section = Reference to section of stack to push.  
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
		inline void push(Genome_section<T> section)
		{
			if (section.size > 0)
				Utilities::FixedSizeStack<T>::yankdup_item(section.starting_position, section.size);
		}


		// Purpose: 
		//   Moves item N to the top of the stack.
		//
		// Parameters:
		//   item_position - Index of item to move.  0 refers to the top item on the stack.
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
		inline void yank_item(int item_position)
		{
			if (item_position >= 0)
			{
				yankdup_item(item_position);
				remove_item_at_position(item_position + 1);
			}
		}

		// Purpose: 
		//   Moves genome at stack level N to the top of the stack.
		//
		//   This is a stack operation.  Each element of the stack is a genome.  This function will yank 
		//   the genome located a stack level N where 0 is the top level of the stack.
		//
		// Parameters:
		//   element_pos = Position in stack of block to yank.  
		// 
		// Return value:
		//   None
		//
		// Side Effects:
		//   The genome at stack level N is moved to the top of the stack.
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		inline void yank_stack_element(int element_pos)
		{
			if (element_pos > 0)
			{
				yankdup_stack_element(element_pos);
				remove_stack_element(element_pos + 1);
			}
		}

		// Purpose: 
		//   Moves item N to the top of the stack.
		//
		// Parameters:
		//   item_position - Index of item to move.  0 refers to the top item on the stack.
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
		inline void yank_stack_element(Genome_section<T> section)
		{
			if (section.size > 0)
			{
				yankdup_stack_element(section);
				Utilities::FixedSizeStack<T>::remove_items(section.starting_position, section.size);
			}
		}

		// Purpose: 
		//   Insert genome section deep in the stack
		//
		// Parameters:
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
		inline void shove(int destination_position, Genome_section<T> source_section)
		{
			T atom;
			std::array<T, N>& stack = Utilities::FixedSizeStack<T>::stack_;

			if ((Utilities::FixedSizeStack<T>::top_ + source_section.size) > N)
			{
				std::stringstream error_message;
				error_message << "Utilities::FixedSizeStack::shove() - Stack overflow.";

				throw std::overflow_error(error_message.str());
			}

			destination_position = (destination_position < 0) ? 0 : destination_position;
			destination_position = (destination_position > Utilities::FixedSizeStack<T>::top_) 
				? Utilities::FixedSizeStack<T>::top_ : destination_position;

			int source_index = Utilities::FixedSizeStack<T>::top_ - source_section.starting_position - 1;
			int destination_index = Utilities::FixedSizeStack<T>::top_ - destination_position - 1;

			// Make space in this stack for the other items
			for (int i = 0, j = Utilities::FixedSizeStack<T>::top_ - 1, k = Utilities::FixedSizeStack<T>::top_;
				i < destination_position; 
				i++, j--, k--)
			{
				stack[k] = stack[j];
			}

			// Insert open parenthesis to begin inserted block
			stack[Utilities::FixedSizeStack<T>::top_ - destination_position] = T("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}");

			// Fix closing parenthesis
			int extra_parenthesis = (source_section.size == 0) ? (1) : (source_section.extra_parenthesis);

			atom = stack[source_index - source_section.size + 1];
			atom.close_parenthesis -= extra_parenthesis;
			stack[source_index - source_section.size + 1] = atom;

			atom = stack[destination_index - source_section.size + 1];
			atom.close_parenthesis += extra_parenthesis;
			stack[destination_index - source_section.size + 1] = atom;

			// Swap source and destinaton atoms
			for (int i = 0, j = source_index, k = Utilities::FixedSizeStack<T>::top_ - 1 - destination_position;
				i < source_section.size;
				i++, j--, k--)
			{
				atom = stack[k];
				stack[k] = stack[j];
				stack[j] = atom;
			}

			// Update stack top pointer
			Utilities::FixedSizeStack<T>::top_++;
		}

		// Purpose: 
		//   Inserts the the top stack element in element N of the stack.
		//
		//   This is a stack operation.  Each element of the stack is a genome.  This function will insert
		//   the genome located a the top of the stack to position N on the stack
		//
		// Parameters:
		//   element_pos = Position in stack to insert into. 
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
		inline void shove_to_stack_element(int element_pos)
		{
			Genome_section<T> target = (*this)[element_pos];
			Genome_section<T> pre_target = (*this)[element_pos - 1];
			Genome_section<T> first = (*this)[0];

			int target_position = (target.size == 0) ? (target.ending_position - 1) : (target.ending_position);
			int target_extra_parenthesis = (target.size == 0) ? (pre_target.extra_parenthesis) : (target.extra_parenthesis);

		 	Utilities::FixedSizeStack<T>::shove_it(target_position, 0, first.size);

			if (target_extra_parenthesis)
			{
				first = (*this)[element_pos - 1];
				T& atom1 = Utilities::FixedSizeStack<T>::get_atom(first.ending_position);

				if (first.extra_parenthesis > 0)
					atom1.close_parenthesis -= first.extra_parenthesis;

				if (first.extra_parenthesis > 0)
				{
					T& atom2 = Utilities::FixedSizeStack<T>::get_atom(target.ending_position);
					int new_close_parenthesis = atom2.close_parenthesis + target_extra_parenthesis;
					atom2.close_parenthesis = new_close_parenthesis;
				}
			}
		}

		// Purpose: 
		//   Substitute the first item with the second item 
		//
		// Parameters:
		//   first - Reference to item to replace
		//   second	- Reference to item to replace with
		// 
		// Return value:
		//   None
		//
		// Side Effects:
		//   The genome is modified if the first item is found
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		inline void subst(Genome_section<T> main_section, Genome_section<T> first_section, Genome_section<T> second_section)
		{
			if (Utilities::FixedSizeStack<T>::free() - second_section.size > 0)
			{
				int new_top = Utilities::FixedSizeStack<T>::size();

				for (int main_section_position = main_section.starting_position; 
					main_section_position <= main_section.ending_position; 
					main_section_position++)
				{
					Genome_section<T> item = get_item_that_starts_at_position(main_section_position);

					if (comp(item, first_section))
					{
						for (int second_section_position = second_section.starting_position;
							second_section_position <= second_section.ending_position;
							second_section_position++)
						{
							T atom = Utilities::FixedSizeStack<T>::get_atom(second_section_position);

							if (second_section_position == second_section.ending_position)
								atom.close_parenthesis = Utilities::FixedSizeStack<T>::get_atom(item.ending_position).close_parenthesis;

							Utilities::FixedSizeStack<T>::container()[new_top] = atom;
							new_top++;

							if (Utilities::FixedSizeStack<T>::check_set_top(new_top) == false)
								return;
						}
					}

					else
					{
						T atom = Utilities::FixedSizeStack<T>::get_atom(item.starting_position);

						Utilities::FixedSizeStack<T>::container()[new_top] = atom;
						new_top++;

						if (Utilities::FixedSizeStack<T>::check_set_top(new_top) == false)
							return;
					}
				}

				int i = Utilities::FixedSizeStack<T>::size();
				int j = new_top - 1;

				while (i < j)
				{
					T atom = Utilities::FixedSizeStack<T>::container()[i];
					Utilities::FixedSizeStack<T>::container()[i] = Utilities::FixedSizeStack<T>::container()[j];
					Utilities::FixedSizeStack<T>::container()[j] = atom;

					i++;
					j--;
				};

				Utilities::FixedSizeStack<T>::set_top(new_top);

				Genome_section<T> item = (*this)[0];

				if (item.extra_parenthesis > 0)
				{
					T& atom = Utilities::FixedSizeStack<T>::get_atom(item.ending_position);
					atom.close_parenthesis -= item.extra_parenthesis;
				}
			}
		}

		// Purpose: 
		//   Returns the position of Block B in Block A or -1 if not found.
		//
		// Parameters:
		//   block_a - Reference to Block A on the stack
		//   block_b - Reference to Block B on the stack
		// 
		// Return value:
		//   Position of the item or -1 if not found
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		inline Genome_section<T> contains(Genome_section<T> block_b, Genome_section<T> block_a)
		{
			unsigned int extra_blocks = 0;

			// If length of Block A is less than Block B + 1, then it is too small to contain Block B
			if (block_a.size > block_b.size)
			{
				for (int block_a_section_position = block_a.starting_position;
					block_a_section_position <= block_a.ending_position;
					block_a_section_position++)
				{
					Genome_section<T> item = get_item_that_starts_at_position(block_a_section_position);

					if (comp(item, block_b))
						return item;
				}
			};

			return Genome_section<T>(0, 0);
		}

		// Purpose: 
		//   Returns the container of the provided item within the genome.
		//
		//   If the genome contains the provided item anywhere (i.e. in any nested list) then the 
		//   container is the smallest sub-list that contains but is not equal to the provided item. For 
		//   example, if the genome is "( B ( C ( A ) ) ( D ( A ) ) )" and the provided item is "( A )" 
		//   then this returns "( C ( A ) )" or returns an empty list if there is no such container.
		//
		// Parameters:
		//   other_genome	- Genome to search for
		//   container		- The container
		//
		// Return value:
		//   True if found
		//	 False if not found
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		//inline bool container_of(Genome_section<T> block_A, Genome_section<T> block_B, Genome_section<T>& container_block)
		//{
		//	bool found = false;
		//	//unsigned int extra_blocks = 0;

		//	//// If length of Block A is less than Block B + 1, then it is too small to contain Block B
		//	//while (block_A.size > block_B.size)
		//	//{
		//	//	T& atom_a = Utilities::FixedSizeStack<T>::stack_[block_A.starting_position];

		//	//	// If the top atom of block A is the start of another block
		//	//	if (Func2BlockWantsMap[atom_a.instruction] > 0)
		//	//	{
		//	//		// And the next item is block B
		//	//		Genome_ref sub_block = block_A + 1;

		//	//		// Then we are done.  Return the container of Block B
		//	//		if (comp(sub_block, block_B))
		//	//		{
		//	//			container_block = block_A;
		//	//			found = true;
		//	//			break;
		//	//		}
		//	//	}

		//	//	// Keep looking...
		//	//	//block_A++;
		//	//};

		//	return found;
		//}

		inline Genome_section<T> container_of(Genome_section<T> block_a, Genome_section<T> block_b)
		{
			bool found = false;
			unsigned int extra_blocks = 0;
			Genome_section<T> container_block(0, 0);

			// If length of Block A is less than Block B + 1, then it is too small to contain Block B
			if (block_a.size > block_b.size)
			{
				//for (unsigned int n = block_a.starting_position; n <= block_a.ending_position; n++)
				//{
				//	unsigned int l = number_of_atoms_in_Nth_block(extra_blocks, n);

				//	Genome_section<T> sub_block(n, l);

				//	if (comp(sub_block, block_b))
				//	{
				//		found = true;
				//		break;
				//	}

				//	container_block.set(n, l);
				//}



				for (int block_a_section_position = block_a.starting_position;
					block_a_section_position <= block_a.ending_position;
					block_a_section_position++)
				{
					Genome_section<T> item = get_item_that_starts_at_position(block_a_section_position);

					if (comp(item, block_b))
					{
						found = true;
						break;
					}

					container_block = item;
				}


			};

			if (!found)
				container_block.clear();

			return container_block;
		}
	};

	template<class T, size_t N>
	inline void Genome<T, N>::convert_genome_to_string()
	{
		genome_string_.clear();

		for (int n = 0; n < Utilities::FixedSizeStack<T>::size(); n++)
		{
			genome_string_ += "{";
			genome_string_ += ":instruction ";
			genome_string_ += Utilities::FixedSizeStack<T>::stack_[n].instruction;
			genome_string_ += " :close  ";
			genome_string_ += std::to_string(Utilities::FixedSizeStack<T>::stack_[n].close_parenthesis);
			genome_string_ += "}";
		}
	}

	template<class T, size_t N>
	inline void Genome<T, N>::ingest_plush_genome(std::string _genome_str)
	{
		std::string gene;
		Utilities::FixedSizeStack<Atom> code_stack;

		while (_genome_str.length() > 0)
		{
			gene = first_atom(_genome_str);
			_genome_str = rest_atom(_genome_str);
			Utilities::trim(_genome_str);

			CodeAtom atom(gene);
			push(atom);
		}
	}

	template<class T, size_t N>
	inline void Genome<T, N>::set(std::string _genome_string)
	{
		std::string genome_string = Utilities::trim_copy(_genome_string);

		if (genome_string.length() == 0)
		{
			std::stringstream error_message;
			error_message << "Genome::set() - Parameter empty.";

			throw std::invalid_argument(error_message.str());
		}

		ingest_plush_genome(_genome_string);
		convert_genome_to_string();
	};
	template <>
	class Genome <long> : public Utilities::FixedSizeStack<long>
	{
	};
	template <>
	class Genome <double> : public Utilities::FixedSizeStack<double>
	{
	};
	template <>
	class Genome <bool> : public Utilities::FixedSizeStack<bool>
	{
	};
}
