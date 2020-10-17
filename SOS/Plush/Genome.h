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

		// Default constructor
		Genome_section()
		{
			this->starting_position = 0;
			this->ending_position = 0;
			this->size = 0;
		}

		// Copy constructor
		Genome_section(const Genome_section<T>& other)
		{
			this->starting_position = other.starting_position;
			this->ending_position = other.ending_position;
			this->size = other.size;
		}

		// Purpose: 
		//   Constructs a reference to a section of the genome
		//
		// Parameters:
		//   starting_position	- 0-based position of the first atom in the genome section
		//   size				- Number of atoms in the genome section
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
		Genome_section(unsigned int starting_position, unsigned int size)
		{
			if (starting_position >= size)
			{
				this->starting_position = starting_position;
				this->ending_position = starting_position - size;
				this->size = size;
			}
			else
			{
				this->starting_position = 0;
				this->ending_position = 0;
				this->size = 0;
			}
		}

		// Purpose: 
		//   Sets a reference to a section of the genome
		//
		// Parameters:
		//   starting_position	- 0-based position of the first atom in the genome section
		//   size				- Number of atoms in the genome section
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
		inline void set(unsigned int starting_position, unsigned int size)
		{
			if (starting_position >= size)
			{
				this->starting_position = starting_position;
				this->ending_position = starting_position - size;
				this->size = size;
			}
			else
			{
				this->starting_position = 0;
				this->ending_position = 0;
				this->size = 0;
			}
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
			if (size >= n)
			{
				std::stringstream error_message;
				error_message << "const_reference Genome_ref::operator - Stack overflow.  n = " << n;

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
		Genome_section<T>& operator++ (int)
		{
			Genome_section<T> temp(*this);

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
		inline const std::array<T, N>& get_atoms()
		{
			return Utilities::FixedSizeStack<T>::stack_;
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
		inline const Genome_section<T> operator [] (unsigned int n) const
		{
			return (Genome_section<T>(item_starting_position(n), item_size(n)));
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
		inline Genome_section<T> operator [] (unsigned int n)
		{
			return (Genome_section<T>(item_starting_position(n), item_size(n)));
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
		inline bool comp(Genome<T> &other_genome) const
		{
			//int size_A = Utilities::FixedSizeStack<T>::size();
			//int size_B = other_genome.size();

			//if (size_A != size_B)
			//	return false;

			//for (int n = 0; n < size_A; n++)
			//{
			//	//Atom atom_A = Utilities::FixedSizeStack<T>::stack_[n];
			//	//Atom atom_B = other_genome[n];
			//	T atom_A = Utilities::FixedSizeStack<T>::stack_[n];
			//	T atom_B = other_genome[n];

			//	if (atom_A != atom_B)
			//		return false;
			//}

			return true;
		}

		inline bool comp(Genome_section<T> genome_a, Genome_section<T> genome_b)
		{
			if (genome_a.size != genome_b.size)
				return false;

			for (int n = 0; n < genome_a.size; n++)
			{
				unsigned int pos_a = genome_a.ending_position - n;
				unsigned int pos_b = genome_b.ending_position - n;

				T atom_a = Utilities::FixedSizeStack<T>::stack_[pos_a];
				T atom_b = Utilities::FixedSizeStack<T>::stack_[pos_b];

				if (atom_a != atom_b)
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
		unsigned int number_of_items(Genome_section<T> section)
		{
			unsigned int item_number = 0;
			unsigned int wanted_blocks = 0;
			unsigned int extra_blocks = 0;
			std::stack<unsigned int> wanted_stack;
			int n = section.starting_position;

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
						}

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
		unsigned int number_of_atoms_in_Nth_block(unsigned int& extra_blocks_returned, int starting_position = -1)
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
				}

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

			extra_blocks_returned = extra_blocks;

			return atom_count;
		};

		// Purpose: 
		//   Splits the genome in two
		//
		//   This function will split the genome into two parts at the split point provided by the caller.
		//   The split point is zero - based; that is, a split point less than or equal to 0 represents a
		//   point before the first item.  A split point greater than the number of items in the genome will 
		//   represent a point after the last item.  The caller is expected to provide the two genomes to 
		//   write the two haves to.  This function is non - destructive, that is, it will not destroy or 
		//   alter the genome to be split.
		//
		//   When determining the split point, the genome is processed as a list object, i.e., an open 
		//   parenthesis is assumed to exist before the first item on the stack.  This function counts 
		//   items or blocks (depending on the mode) from the beginning of the genome to locate the split 
		//   point; nested lists contribute only 1 to this count, no matter what they contain.  Closing 
		//   parenthesis can either be interpreted as close instructions (to satisfy a block requirement 
		//   in a nested list) or as close - open instructions (for the top level list).  Nested levels 
		//   begin when an instruction requiring blocks is encountered in the list and end when all 
		//   required blocks are found.
		//
		// Parameters:
		//   left_half		- Reference to buffer to write genome items located before the split point
		//   right_half		- Reference to buffer to write genome items located after the split point
		//   split_position - Zero-based index of the split point in the genome list of items
		//   mode			- Bolck or Item
		//
		// Return value:
		//   Number if items in the top level of the list
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		enum SPLIT_MODE
		{
			block,
			item
		};
		unsigned int split(Genome<T> &left_half, 
			Genome<T> &right_half, 
			unsigned int split_position, 
			SPLIT_MODE mode)
		{
			//Genome<T> genome(this);
			//Genome<T> temp_left;
			//Genome<T> temp_right;

			//while ((genome.empty() == false) && (split_position-- > 0))
			//{
			//	Genome<T> temp;

			//	if (mode == SPLIT_MODE::block)
			//		genome.pop_genome(temp);
			//	else
			//		genome.pop_item(temp);

			//	temp_left.push_genome(temp);
			//}

			//while (genome.empty() == false)
			//{
			//	Genome<T> temp;

			//	if (mode == SPLIT_MODE::block)
			//		genome.pop_genome(temp);
			//	else
			//		genome.pop_item(temp);

			//	temp_right.push_genome(temp);
			//}

			//while (temp_left.empty() == false)
			//{
			//	Genome<T> temp;

			//	if (mode == SPLIT_MODE::block)
			//		temp_left.pop_genome(temp);
			//	else
			//		temp_left.pop_item(temp);

			//	left_half.push_genome(temp);
			//}

			//while (temp_right.empty() == false)
			//{
			//	Genome<T> temp;

			//	if (mode == SPLIT_MODE::block)
			//		temp_right.pop_genome(temp);
			//	else
			//		temp_right.pop_item(temp);

			//	right_half.push_genome(temp);
			//}

			unsigned int item_number = 0;
			
			return item_number;
		}

		// Purpose: 
		//   Push a genome on the back of the stack
		//
		// Parameters:
		//   genome	- Reference to genome to push
		// 
		// Return value:
		//   None
		//
		// Side Effects:
		//   Stack updated with provided genome at the bottom of the stack.
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		inline void push_back(Genome<T> &genome)
		{
			Utilities::FixedSizeStack<T>::shove(genome, Utilities::FixedSizeStack<T>::size());
		}

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

		//// Purpose: 
		////   Pop a genome from the stack
		////
		////   For example, if the top genome is "( A B )" then this returns "( A B )" (after popping the 
		////   argument from the genome). If the top genome is "( ( A B ) C )" then this returns "( A B )".
		////
		////   Will push a NOOP on the stack with the approprieate number of close parenthesis if needed to 
		////   compensate for extra closing parenthesis in the poped genome.
		////
		//// Parameters:
		////   genome	- Reference to buffer to copy poped genome into
		//// 
		//// Return value:
		////   None
		////
		//// Side Effects:
		////   The top genome is poped from the stack and copied to the provided buffer.  The provided 
		////   genome buffer is cleared first.
		////
		//// Thread Safe:
		////   Yes.  As long as no other thread attemps to write to the child.
		////
		//// Remarks:
		////
		//inline unsigned int pop_genome(Genome<T> &poped_item)
		//{
		//	//unsigned int item_number = 0;
		//	//unsigned int wanted_blocks = 0;
		//	//unsigned int extra_blocks = 0;
		//	//Utilities::FixedSizeStack<T> temp;
		//	//std::stack<unsigned int> wanted_stack;

		//	//poped_item.clear();

		//	//while (Utilities::FixedSizeStack<T>::empty() == false)
		//	//{
		//	//	T atom;

		//	//	if (extra_blocks == 0)
		//	//	{
		//	//		atom = Utilities::FixedSizeStack<T>::get_top();
		//	//		Utilities::FixedSizeStack<T>::pop();
		//	//		temp.push(atom);
		//	//	}
		//	//	else
		//	//	{
		//	//		atom = T("{:instruction EXEC.NOOP :close 1}");
		//	//		extra_blocks--;
		//	//	}

		//	//	int closing = atom.close_parenthesis - Func2BlockWantsMap[atom.instruction];

		//	//	if (closing < 0)
		//	//	{
		//	//		wanted_stack.push(wanted_blocks);
		//	//		wanted_blocks = 0 - closing;
		//	//	}

		//	//	extra_blocks = (closing > 1) ? (closing - 1) : (0);

		//	//	if (closing > 0)
		//	//	{
		//	//		if (wanted_blocks > 0)
		//	//			wanted_blocks--;

		//	//		else if ((wanted_blocks == 0) && (wanted_stack.size() == 0))
		//	//			break;
		//	//	}

		//	//	if (wanted_blocks == 0)
		//	//	{
		//	//		if (wanted_stack.size() == 0)
		//	//			item_number++;

		//	//		if (wanted_stack.size() > 0)
		//	//		{
		//	//			wanted_blocks = wanted_stack.top();
		//	//			wanted_stack.pop();
		//	//		}
		//	//	}
		//	//}

		//	//T first = temp.get_top();
		//	//temp.pop();
		//	//first.close_parenthesis -= extra_blocks;
		//	//temp.push(first);

		//	//while (temp.size() > 0)
		//	//{
		//	//	T atom = temp.get_top();
		//	//	temp.pop();
		//	//	poped_item.push(atom);
		//	//}

		//	//if (extra_blocks > 0)
		//	//{
		//	//	std::string noop = "{:instruction EXEC.NOOP :close " + std::to_string(extra_blocks) + "}";

		//	//	Utilities::FixedSizeStack<T>::push(noop);
		//	//}

		//	return 0;
		//};

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
			Genome_section<T> block = (*this)[0];
			
			Utilities::FixedSizeStack<T>::top_ -= block.size;

			return block;
		};

		// Purpose: 
		//   Pop first top-level item from the genome
		//
		//   For example, if the top genome "( A B )" then this returns "A" (after popping the argument 
		//   from the genome). However, if the top genome is a block then that will be returned.  For 
		//	 example, if the top genome is "( ( A B ) C )" then this returns "( A B )".
		//
		// Parameters:
		//   first	- Reference to buffer to copy poped item into
		// 
		// Return value:
		//   None
		//
		// Side Effects:
		//   The top item is removed from the genome.
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		unsigned int pop_item(Genome<T> &poped_item)
		{
			//unsigned int item_number = 0;
			//unsigned int wanted_blocks = 0;
			unsigned int extra_blocks = 0;
			//Utilities::FixedSizeStack<CodeAtom> temp;
			//std::stack<unsigned int> wanted_stack;

			//poped_item.clear();

			//while ((Utilities::FixedSizeStack<T>::empty() == false) && (item_number == 0))
			//{
			//	T atom;

			//	if (extra_blocks == 0)
			//	{
			//		atom = Utilities::FixedSizeStack<T>::get_top();
			//		Utilities::FixedSizeStack<T>::pop();
			//		temp.push(atom);
			//	}
			//	else
			//	{
			//		atom = T("{:instruction EXEC.NOOP :close 1}");
			//		extra_blocks--;
			//	}

			//	int closing = atom.close_parenthesis - Func2BlockWantsMap[atom.instruction];

			//	if (closing < 0)
			//	{
			//		wanted_stack.push(wanted_blocks);
			//		wanted_blocks = 0 - closing;
			//	}

			//	extra_blocks = (closing > 1) ? (closing - 1) : (0);

			//	if (closing > 0)
			//	{
			//		if (wanted_blocks > 0)
			//			wanted_blocks--;

			//		if ((wanted_blocks == 0) && (wanted_stack.size() == 1))
			//			break;
			//	}

			//	if (wanted_blocks == 0)
			//	{
			//		if (wanted_stack.size() == 0)
			//			item_number++;

			//		if (wanted_stack.size() > 0)
			//		{
			//			wanted_blocks = wanted_stack.top();
			//			wanted_stack.pop();
			//		}
			//	}
			//}

			//while (temp.size() > 0)
			//{
			//	T atom = temp.get_top();
			//	temp.pop();
			//	poped_item.push(atom);
			//}

			return extra_blocks;
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

		// Purpose: 
		//   Returns the top genome on the stack
		//
		// Parameters:
		//   genome	- Reference to buffer to copy top genome into
		// 
		// Return value:
		//   None
		//
		// Side Effects:
		//   The provided genome buffer is cleared first.
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
//		};

		// Purpose: 
		//   Returns the top genome on the stack
		//
		// Parameters:
		//   genome	- Reference to buffer to copy top genome into
		// 
		// Return value:
		//   Top atom
		//
		// Side Effects:
		//   The provided genome buffer is cleared first.
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		inline T get_top(Genome_section<T> section)
		{
			return Utilities::FixedSizeStack::stack_[section.starting_position];
		};

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

			return Utilities::FixedSizeStack<T>::get_item(s - 1);
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
			for (int n = 0; n <= item_index + 1; n++)
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
		inline unsigned int subitem_starting_position(int position)
		{
			int s = 0;
			int l = 0;
			unsigned int extra_blocks;

			// Find index to item after target item
			for (int n = 0; n <= position; n++)
			{
				l = number_of_atoms_in_Nth_block(extra_blocks, n);
				s += l;
			}

			return s - l;
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
		inline Genome_section<T> get_subitem(int position)
		{
			int s = 0;
			int l = 0;
			unsigned int extra_blocks;

			// Find index to item after target item
			for (int n = 0; n <= position; n++)
			{
				l = number_of_atoms_in_Nth_block(extra_blocks, n);
				s += l;
			}

			return Genome_section<T>(s - l, l);
		}

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
		inline unsigned int subitem_ending_position(int position)
		{
			int s = 0;
			int l = 0;
			unsigned int extra_blocks;

			// Find index to top of item after target item
			for (int n = 0; n <= position; n++)
			{
				l = number_of_atoms_in_Nth_block(extra_blocks, n);
				s += l;
			}

			return s - 1;
		}

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
		inline void remove_item(int item_position)
		{
			int s = 0;
			int l = 0;
			unsigned int extra_blocks;

			for (int n = 0; n <= item_position; n++)
			{
				s += l;
				l = number_of_atoms(extra_blocks, n);
			}

			if (extra_blocks == 0)
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
		//inline void remove_block(Genome_ref& block)
		//{
		//	int s = 0;
		//	int l = 0;
		//	unsigned int extra_blocks;

		//	s = block.starting_position;
		//	l = number_of_atoms_in_Nth_block(extra_blocks, s);

		//	if (extra_blocks == 0)
		//		Utilities::FixedSizeStack<T>::remove_items(s, l);

		//	else
		//	{
		//		std::string atom_str = "{:instruction EXEC.NOOP :close ";
		//		atom_str += std::to_string(extra_blocks);
		//		atom_str += "}";
		//		T atom(atom_str);

		//		Utilities::FixedSizeStack<T>::replace(atom, s + l - 1);

		//		if (l > 1)
		//			Utilities::FixedSizeStack<T>::remove_items(s, l - 1);
		//	}
		//}

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

			s = element_pos;
			l = number_of_atoms_in_Nth_block(extra_blocks, s);

			if (l > 0)
				Utilities::FixedSizeStack<T>::yankdup_item(s, l);

			else
				push(T("{:instruction EXEC.NOOP :close 1}"));

			if (Utilities::FixedSizeStack<T>::stack_[Utilities::FixedSizeStack<T>::size() - l].close_parenthesis > extra_blocks)
				Utilities::FixedSizeStack<T>::stack_[Utilities::FixedSizeStack<T>::size() - l].close_parenthesis -= extra_blocks;
		}

		inline void yankdup_stack_element(Genome_section<T> section)
		{
			if (section.size > 0)
				Utilities::FixedSizeStack<T>::yankdup_item(section.starting_position, section.size);

			else
				push(T("{:instruction EXEC.NOOP :close 1}"));
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
			if (item_position > 0)
			{
				yankdup_item(item_position);
				remove_item(item_position + 1);
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
//			inline void shove(FixedSizeStack<T>& other, int insert_position, int offset, int length)

			Genome_section<T> section = (*this)[element_pos];

			shove(this, section.starting_position, 0, section.size);
		}

		// Purpose: 
		//   Substitute the second item for the first item in the genome
		//
		// Parameters:
		//   first	- Reference to item to replace with
		//   second - Reference to item to replace
		// 
		// Return value:
		//   None
		//
		// Side Effects:
		//   The genome is modified if the second item is found
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		//inline bool subst(Genome<T> &first_genome, Genome<T> &second_genome)
		//{
		//	bool found = false;

			//Genome<T> modified_block;
			//Genome<T> original_genome(this);

			//if ((first_genome.size() == 1) &&(second_genome.size() == 1))
			//{
			//	while (Utilities::FixedSizeStack<T>::empty() == false)
			//	{
			//		Genome<T> temp_block;

			//		pop_item(temp_block);

			//		if (temp_block == second_genome)
			//		{
			//			Genome<T> modified_block_copy(modified_block);

			//			modified_block.clear();

			//			modified_block.push_genome(first_genome);
			//			modified_block.push_genome(modified_block_copy);

			//			found = true;
			//		}
			//		else
			//		{
			//			if (temp_block.size() > 1)
			//			{
			//				T atom = temp_block.pop();

			//				if (atom.like(second_genome.bottom()))
			//					atom.instruction = first_genome.bottom().instruction;

			//				if (temp_block.subst(first_genome, second_genome))
			//					found = true;

			//				Genome<T> modified_block_copy(modified_block);

			//				modified_block.clear();
			//				modified_block.push_genome(temp_block);
			//				modified_block.push(atom);
			//				modified_block.push_genome(modified_block_copy);
			//			}

			//			else if (temp_block.bottom().like(second_genome.bottom()))
			//			{
			//				temp_block.bottom().instruction = first_genome.bottom().instruction;
			//				modified_block.push_genome(temp_block);
			//				found = true;
			//			}

			//			else
			//				modified_block.push_genome(temp_block);
			//		}
			//	}
			//}

			//else
			//{
			//	while (Utilities::FixedSizeStack<T>::empty() == false)
			//	{
			//		Genome<T> temp_block;
			//		Genome<T> temp_first_genome = first_genome;

			//		pop_item(temp_block);

			//		// Normalliize to one block
			//		int extra_blocks = 0;

			//		if (temp_block.bottom().close_parenthesis > second_genome.bottom().close_parenthesis)
			//		{
			//			extra_blocks = temp_block.bottom().close_parenthesis - second_genome.bottom().close_parenthesis;
			//			temp_block.bottom().close_parenthesis = second_genome.bottom().close_parenthesis;
			//			temp_first_genome.bottom().close_parenthesis += extra_blocks;
			//		}

			//		if (temp_block == second_genome)
			//		{
			//			modified_block.push_genome(temp_first_genome);
			//			found = true;
			//		}

			//		else
			//		{
			//			if (temp_block.size() > 1)
			//			{
			//				T atom = temp_block.pop();

			//				if (atom == second_genome.bottom())
			//					atom.instruction = temp_first_genome.bottom().instruction;

			//				if (temp_block.subst(temp_first_genome, second_genome))
			//					found = true;

			//				Genome<T> modified_block_copy(modified_block);

			//				modified_block.clear();
			//				modified_block.push_genome(temp_block);
			//				modified_block.push(atom);
			//				modified_block.push_genome(modified_block_copy);
			//			}

			//			else if (temp_block.bottom().like(second_genome.bottom()))
			//			{
			//				temp_block.bottom().instruction = temp_first_genome.bottom().instruction;
			//				modified_block.push_genome(temp_block);
			//				found = true;
			//			}

			//			else
			//				modified_block.push_genome(temp_block);
			//		}
			//	}
			//}

			//if (found)
			//	push_genome(modified_block);

			//else
			//	push_genome(original_genome);

			//return found;
		//}

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
			for (int i = main_section.ending_position; i < main_section.starting_position; i++)
			{
				bool found = false;
				int j = first_section.ending_position;
				int extra_parenthesis = 0;

				for (int n = 0; n = first_section.size; n++)
				{
					if (Utilities::FixedSizeStack::stack_[i + n].type != Utilities::FixedSizeStack::stack_[j + n].type)
						break;

					if (Utilities::FixedSizeStack::stack_[i + n].instruction != Utilities::FixedSizeStack::stack_[j + n].instruction)
						break;

					if (n > 0)
					{
						if (Utilities::FixedSizeStack::stack_[i + n].close_parenthesis != Utilities::FixedSizeStack::stack_[j + n].close_parenthesis)
							break;
					}
					else
					{
						if (Utilities::FixedSizeStack::stack_[i + n].close_parenthesis < Utilities::FixedSizeStack::stack_[j + n].close_parenthesis)
							break;

						extra_parenthesis = Utilities::FixedSizeStack::stack_[i + n].close_parenthesis < Utilities::FixedSizeStack::stack_[j + n].close_parenthesis;
					}

					found = true;
				}

				if (found)
				{
					for (int k = second_section.ending_position; k < second_section.starting_position; k++)
					{
						T atom = Utilities::FixedSizeStack::stack_[k];

						if (k == second_section.ending_position)
							atom.close_parenthesis += extra_parenthesis;

						Utilities::FixedSizeStack::push(atom);
					}

					i += first_section.size;
				}

				else
					Utilities::FixedSizeStack::push(Utilities::FixedSizeStack::stack_[i]);
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
		inline Genome_section<T> contains(Genome_section<T> block_a, Genome_section<T> block_b)
		{
			unsigned int extra_blocks = 0;

			// If length of Block A is less than Block B + 1, then it is too small to contain Block B
			if (block_a.size > block_b.size)
			{
				for (unsigned int n = block_a.starting_position; n <= block_a.ending_position; n++)
				{
					unsigned int l = number_of_atoms_in_Nth_block(extra_blocks, n);

					Genome_section<T> sub_block(n, l);

					if (comp(sub_block, block_b))
						return sub_block;
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
		inline bool container_of(Genome_section<T> block_A, Genome_section<T> block_B, Genome_section<T>& container_block)
		{
			bool found = false;
			//unsigned int extra_blocks = 0;

			//// If length of Block A is less than Block B + 1, then it is too small to contain Block B
			//while (block_A.size > block_B.size)
			//{
			//	T& atom_a = Utilities::FixedSizeStack<T>::stack_[block_A.starting_position];

			//	// If the top atom of block A is the start of another block
			//	if (Func2BlockWantsMap[atom_a.instruction] > 0)
			//	{
			//		// And the next item is block B
			//		Genome_ref sub_block = block_A + 1;

			//		// Then we are done.  Return the container of Block B
			//		if (comp(sub_block, block_B))
			//		{
			//			container_block = block_A;
			//			found = true;
			//			break;
			//		}
			//	}

			//	// Keep looking...
			//	//block_A++;
			//};

			return found;
		}

		inline Genome_section<T> container_of(Genome_section<T> block_a, Genome_section<T> block_b)
		{
			unsigned int extra_blocks = 0;
			Genome_section<T>& container_block(0, 0);

			// If length of Block A is less than Block B + 1, then it is too small to contain Block B
			if (block_a.size > block_b.size)
			{
				for (unsigned int n = block_a.starting_position; n <= block_a.ending_position; n++)
				{
					unsigned int l = number_of_atoms_in_Nth_block(extra_blocks, n);

					Genome_section<T> sub_block(n, l);

					if (comp(sub_block, block_b))
						return sub_block;

					container_block.set(n, l);
				}
			};

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
