#pragma once
#include <string>
#include <vector>
#include <stack>
#include <map>
#include "..\Utilities\FixedSizeStack.h"
#include "Atom.h"

// Favor blocks over atoms algorithm:
// - An atoms is considered a single littoral or instruction.
// - A block is zero or more atoms surrounded by parenthesis.
// - An item is an atom or a block
// - A genome is a list of one or more items
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

	template <class T, size_t N = domain::argmap::maximum_stack_size>
//	template <class T>
	class Genome : public Utilities::FixedSizeStack<T>
	{
	private:
//		Utilities::FixedSizeStack<Atom> *stack_;

		// Plush genome
		//std::vector<struct Atom> genome_atoms_;
		//std::string genome_string_;

		// Push program
		//std::string program_;
		//unsigned int points_;

		// Help functions
//		void ingest_plush_genome(std::string _genome_str);
////		void translate_plush_genome_to_push_program();
//		unsigned int count_points();
//		void convert_genome_to_string();

	public:
		Genome()
		{
			Utilities::FixedSizeStack<T>::top_ = 0;
		}

		inline Genome(Genome *other)
		{
			Utilities::FixedSizeStack<T>::top_ = other->top_;

			for (int n = 0; n < Utilities::FixedSizeStack<T>::top_; n++)
				Utilities::FixedSizeStack<T>::stack_[n] = other->stack_[n];
		}

		////Genome(Utilities::FixedSizeStack<Atom> *stack);
		////Genome(const Genome &other_genome);
		////Genome(std::string _genome_string);
		//Genome(Utilities::FixedSizeStack<Atom> &atoms);

		//void set(std::string _genome_string);
		//void set(std::vector<struct Atom> & _genome_atoms);

		//void clear()
		//{
		//	genome_atoms_.clear();
		//	genome_string_.clear();
		//	program_.clear();
		//	points_ = 0;
		//}

		//const std::string to_string()
		//{
		//	if (genome_string_.empty())
		//		convert_genome_to_string();

		//	return genome_string_;
		//}

		//const unsigned int size()
		//{
		//	return genome_atoms_.size();
		//}

		//const std::vector<struct Atom> get_atoms()
		//{
		//	return genome_atoms_;
		//}

		//const std::string get_program()
		//{
		//	if (program_.empty())
		//		translate_plush_genome_to_push_program();

		//	return program_;
		//}

		//const unsigned int get_points()
		//{
		//	return points_;
		//}

//		unsigned int length();

		typedef typename std::array<T, N>::value_type value_type;
		typedef typename std::array<T, N>::reference reference;
		typedef typename std::array<T, N>::const_reference const_reference;
		typedef typename std::array<T, N>::size_type size_type;

		inline const_reference operator [] (int index) const
		{
			return Utilities::FixedSizeStack<T>::stack_[index];
		}

		inline reference operator [] (int index)
		{
			return Utilities::FixedSizeStack<T>::stack_[index];
		}

		inline bool operator==(Genome<Atom> &other_genome) const
		{
			return comp(other_genome);
		}

		inline bool operator!=(Genome<Atom> &other_genome) const
		{
			return !comp(other_genome);
		}

		// Purpose: 
		//   Returns the length of the genome stack. 
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
		//   Number if items and blocks in the top level of the list
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

			while (n >= 0)
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
					//					extra_blocks--;
				}

				int closing = atom.close_parentheses - Func2BlockWantsMap[atom.instruction];

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
					if (wanted_stack.size() == 0)
						item_number++;

					if (wanted_stack.size() > 0)
					{
						wanted_blocks = wanted_stack.top();
						wanted_stack.pop();
					}
					else
						wanted_blocks = 1; // Assume closing parenthesis on top level block always start another block.
				}
			}

			return item_number;
		}

		// Purpose: 
		//   Returns the length of the genome stack. 
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
		//   Number if items and blocks in the top level of the list
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		unsigned int number_of_items()
		{
			unsigned int item_number = 0;
			unsigned int wanted_blocks = 0;
			unsigned int extra_blocks = 0;
			std::stack<unsigned int> wanted_stack;
			int n = Utilities::FixedSizeStack<T>::size() - 1;

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

				int closing = atom.close_parentheses - Func2BlockWantsMap[atom.instruction];

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
		//   items from the beginning of the genome to locate the split point; nested lists contribute 
		//   only 1 to this count, no matter what they contain.  Closing parenthesis can either be 
		//   interpreted as close instructions (to satisfy a block requirement in a nested list) or as 
		//   close - open instructions (for the top level list).  Nested levels begin when an instruction 
		//   requiring blocks is encountered in the list and end when all required blocks are found.
		//
		// Parameters:
		//   left_half		- Reference to buffer to write genome items located before the split point
		//   right_half		- Reference to buffer to write genome items located after the split point
		//   split_position - Zero-based index of the split point in the genome list of items
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
		unsigned int split(Genome<T> &left_half, Genome<T> &right_half, unsigned int split_position)
		{
			unsigned int item_number = 0;
			unsigned int wanted_blocks = 0;
			Utilities::FixedSizeStack<T> temp_left;
			Utilities::FixedSizeStack<T> temp_right;
			std::stack<unsigned int> wanted_stack;
			int simulated_closing_parenthesis = 0;
			Plush::Atom atom;

			// Itierate through the stack in the order the would be poped
			for (int n = Utilities::FixedSizeStack<T>::size() - 1; n >= 0; n--)
			{
				if (simulated_closing_parenthesis == 0)
					atom = Utilities::FixedSizeStack<T>::stack_[n];

				if (simulated_closing_parenthesis > 0)
				{
					atom = Plush::Atom("{:instruction EXEC.NOOP :close 1}");
					simulated_closing_parenthesis--;
					n++;
				}

				int closing = atom.close_parentheses - Func2BlockWantsMap[atom.instruction];

				if (closing > 1)
				{
					simulated_closing_parenthesis = closing - 1;
					atom.close_parentheses--;
				}

				if (item_number < split_position)
					temp_left.push(atom);
				else
					temp_right.push(atom);

				if (closing < 0)
				{
					wanted_stack.push(wanted_blocks);
					wanted_blocks = 0 - closing;
				}

				if (closing > 0)
					wanted_blocks > 0 ? --wanted_blocks : 0;

				if (wanted_blocks == 0)
				{
					if (wanted_stack.size() > 0)
					{
						wanted_blocks = wanted_stack.top();
						wanted_stack.pop();
					}

					item_number++;
				}
			};

			while (temp_left.size() > 0)
			{
				Plush::Atom atom = temp_left.top();
				temp_left.pop();
				left_half.push(atom);
			}

			while (temp_right.size() > 0)
			{
				Plush::Atom atom = temp_right.top();
				temp_right.pop();
				right_half.push(atom);
			}

			if (simulated_closing_parenthesis > 0)
				right_half[0].close_parentheses += simulated_closing_parenthesis;

			return item_number;
		}

		// Purpose: 
		//   Push a genome on the stack
		//
		// Parameters:
		//   genome	- Reference to genome to push
		// 
		// Return value:
		//   None
		//
		// Side Effects:
		//   Stack updated with provided genome at the top of the stack.
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		inline void push(Genome<Atom> &genome)
		{
			unsigned int item_number = 0;
			unsigned int wanted_blocks = 0;
			unsigned int extra_blocks;
			Utilities::FixedSizeStack<T> temp;
			std::stack<unsigned int> wanted_stack;

			for (int n = 0; n < genome.size(); n++)
			{
				Atom atom = genome[n];

				//if ((atom.instruction == "EXEC.NOOP")
				//	&& (atom.type == Atom::AtomType::ins)
				//	&& (Utilities::FixedSizeStack<T>::top_ > 0))
				//{
				//	Utilities::FixedSizeStack<T>::stack_[Utilities::FixedSizeStack<T>::top_ - 1].close_parentheses += atom.close_parentheses;
				//}

				//else

				Atom top_atom = Utilities::FixedSizeStack<T>::top();
				
				if ((top_atom.instruction == "EXEC.NOOP")
					&& (top_atom.type == Atom::AtomType::ins)
					&& (Utilities::FixedSizeStack<T>::top_ > 0))
				{
					atom.close_parentheses += top_atom.close_parentheses;
					Utilities::FixedSizeStack<T>::pop();
				}

				Utilities::FixedSizeStack<T>::push(atom);
			}
		}

		// Purpose: 
		//   Push a genome on the stack
		//
		// Parameters:
		//   genome	- Reference to genome to push
		// 
		// Return value:
		//   None
		//
		// Side Effects:
		//   Stack updated with provided genome at the top of the stack.
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		inline void push_back(Genome<Atom> &genome)
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
		inline void push(const value_type& atom)
		{
			//if ((atom.instruction == "EXEC.NOOP")
			//	&& (atom.type == Atom::AtomType::ins)
			//	&& (Utilities::FixedSizeStack<T>::top_ > 0))
			//{
			//	Utilities::FixedSizeStack<T>::stack_[Utilities::FixedSizeStack<T>::top_ - 1].close_parentheses += atom.close_parentheses;
			//}

			//else

			Atom top_atom = Utilities::FixedSizeStack<T>::top();
			Atom atom_copy(atom);

			if ((top_atom.instruction == "EXEC.NOOP")
				&& (top_atom.type == Atom::AtomType::ins)
				&& (Utilities::FixedSizeStack<T>::top_ > 0))
			{
				atom_copy.close_parentheses += top_atom.close_parentheses;
				Utilities::FixedSizeStack<T>::pop();
			}
		
			Utilities::FixedSizeStack<T>::push(atom_copy);
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
			Plush::Atom atom = Utilities::FixedSizeStack<T>::top();
			Utilities::FixedSizeStack<T>::pop();
			return atom;
		}

		// Purpose: 
		//   Pop a genome from the stack
		//
		//   For example, if the top genome "( A B )" then this returns "( A B )" (after popping the 
		//   argument from the genome). If the top genome is "( ( A B ) C )" then this returns "( A B )".
		//
		//   Will push a NOOP on the stack with the approprieate number of close parenthesis if needed to 
		//   compensate for extra closing parenthesis in the poped genome.
		//
		// Parameters:
		//   genome	- Reference to buffer to copy poped genome into
		// 
		// Return value:
		//   None
		//
		// Side Effects:
		//   The top genome is poped from the stack and copied to the provided buffer.  The provided 
		//   genome buffer is cleared first.
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		unsigned int pop_genome(Genome<Atom> &poped_item)
		{
			unsigned int item_number = 0;
			unsigned int wanted_blocks = 0;
			unsigned int extra_blocks = 0;
			Utilities::FixedSizeStack<CodeAtom> temp;
			std::stack<unsigned int> wanted_stack;

			poped_item.clear();

			while (Utilities::FixedSizeStack<T>::empty() == false)
			{
				Plush::Atom atom;

				if (extra_blocks == 0)
				{
					atom = Utilities::FixedSizeStack<T>::top();
					Utilities::FixedSizeStack<T>::pop();
					temp.push(atom);
				}
				else
				{
					atom = Plush::Atom("{:instruction EXEC.NOOP :close 1}");
					extra_blocks--;
				}

				int closing = atom.close_parentheses - Func2BlockWantsMap[atom.instruction];

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
					if (wanted_stack.size() == 0)
						item_number++;

					if (wanted_stack.size() > 0)
					{
						wanted_blocks = wanted_stack.top();
						wanted_stack.pop();
					}
				}
			}

			Plush::Atom first = temp.top();
			temp.pop();
			first.close_parentheses -= extra_blocks;
			temp.push(first);

			while (temp.size() > 0)
			{
				Plush::Atom atom = temp.top();
				temp.pop();
				poped_item.push(atom);
			}

			if (extra_blocks > 0)
			{
				std::string noop = "{:instruction EXEC.NOOP :close " + std::to_string(extra_blocks) + "}";

				Utilities::FixedSizeStack<T>::push(noop);
			}

			return 0;
		};

		// Purpose: 
		//   Pop first item from the genome
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
		unsigned int pop_item(Genome<Atom> &poped_item)
		{
			unsigned int item_number = 0;
			unsigned int wanted_blocks = 0;
			unsigned int extra_blocks = 0;
			Utilities::FixedSizeStack<CodeAtom> temp;
			std::stack<unsigned int> wanted_stack;

			poped_item.clear();

			while ((Utilities::FixedSizeStack<T>::empty() == false) && (item_number == 0))
			{
				Plush::Atom atom;

				if (extra_blocks == 0)
				{
					atom = Utilities::FixedSizeStack<T>::top();
					Utilities::FixedSizeStack<T>::pop();
					temp.push(atom);
				}
				else
				{
					atom = Plush::Atom("{:instruction EXEC.NOOP :close 1}");
					extra_blocks--;
				}

				int closing = atom.close_parentheses - Func2BlockWantsMap[atom.instruction];

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
					{
						//extra_blocks = (closing > 1) ? (closing - 1) : (0);
						break;
					}
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

			while (temp.size() > 0)
			{
				Plush::Atom atom = temp.top();
				temp.pop();
				poped_item.push(atom);
			}

			return extra_blocks;
		};

		// Purpose: 
		//   Returns the top atom on the stack
		//
		// Parameters:
		//   None
		// 
		// Return value:
		//   CodeAtom or ExecAtom poped from the stack
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		inline T top()
		{
			Plush::Atom atom = Utilities::FixedSizeStack<T>::top();
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
		unsigned int top(Genome<Atom> &poped_item)
		{
			unsigned int item_number = 0;
			unsigned int wanted_blocks = 0;
			unsigned int extra_blocks;
			Utilities::FixedSizeStack<CodeAtom> temp;
			std::stack<unsigned int> wanted_stack;

			poped_item.clear();

			while (Utilities::FixedSizeStack<T>::empty() == false)
			{
				Plush::Atom atom = Utilities::FixedSizeStack<T>::top();
				Utilities::FixedSizeStack<T>::pop();

				temp.push(atom);

				int closing = atom.close_parentheses - Func2BlockWantsMap[atom.instruction];

				if (closing < 0)
				{
					wanted_stack.push(wanted_blocks);
					wanted_blocks = 0 - closing;
				}

				if (closing > 0)
				{
					if (wanted_blocks > 0)
						wanted_blocks--;

					else if (wanted_blocks == 0)
					{
						extra_blocks = (closing > 1) ? (closing - 1) : (0);
						break;
					}
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

			while (temp.size() > 0)
			{
				Plush::Atom atom = temp.top();
				temp.pop();
				poped_item.push(atom);
				Utilities::FixedSizeStack<T>::push(atom);
			}

			return extra_blocks;
		};

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
		bool comp(Genome<Atom> &other_genome) const
		{
			int size_A = Utilities::FixedSizeStack<T>::size();
			int size_B = other_genome.size();

			if (size_A != size_B)
				return false;

			for (int n = 0; n < size_A; n++)
			{
				Atom atom_A = Utilities::FixedSizeStack<T>::stack_[n];
				Atom atom_B = other_genome[n];

				if (atom_A != atom_B)
					return false;
			}
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
		bool subst(Genome<Atom> &first_genome, Genome<Atom> &second_genome)
		{
			bool found = false;
			Genome<Atom> modified_block;
			Genome<Atom> original_genome(this);

			if ((first_genome.size() ==1) &&(second_genome.size() == 1))
			{
				while (Utilities::FixedSizeStack<T>::empty() == false)
				{
					Genome<Atom> temp_block;

					pop_item(temp_block);

					if (temp_block == second_genome)
					{
						Genome<Atom> modified_block_copy(modified_block);

						modified_block.clear();

						modified_block.push(first_genome);
						modified_block.push(modified_block_copy);

						found = true;
					}
					else
					{
						if (temp_block.size() > 1)
						{
							Atom atom = temp_block.pop();

							if (atom.like(second_genome[0]))
								atom.instruction = first_genome[0].instruction;

							if (temp_block.subst(first_genome, second_genome))
								found = true;

							Genome<Atom> modified_block_copy(modified_block);

							modified_block.clear();
							modified_block.push(temp_block);
							modified_block.push(atom);
							modified_block.push(modified_block_copy);
						}

						else if (temp_block[0].like(second_genome[0]))
						{
							temp_block[0].instruction = first_genome[0].instruction;
							modified_block.push(temp_block);
							found = true;
						}

						else
							modified_block.push(temp_block);
					}
				}
			}

			else
			{
				while (Utilities::FixedSizeStack<T>::empty() == false)
				{
					Genome<Atom> temp_block;
					Genome<Atom> temp_first_genome = first_genome;

					pop_item(temp_block);

					// Normalliize to one block
					int extra_blocks = 0;

					if (temp_block[0].close_parentheses > second_genome[0].close_parentheses)
					{
						extra_blocks = temp_block[0].close_parentheses - second_genome[0].close_parentheses;
						temp_block[0].close_parentheses = second_genome[0].close_parentheses;
						temp_first_genome[0].close_parentheses += extra_blocks;
					}

					if (temp_block == second_genome)
					{
						modified_block.push(temp_first_genome);
						found = true;
					}

					else
					{
						if (temp_block.size() > 1)
						{
							Atom atom = temp_block.pop();

							if (atom == second_genome[0])
								atom.instruction = temp_first_genome[0].instruction;

							if (temp_block.subst(temp_first_genome, second_genome))
								found = true;

							Genome<Atom> modified_block_copy(modified_block);

							modified_block.clear();
							modified_block.push(temp_block);
							modified_block.push(atom);
							modified_block.push(modified_block_copy);
						}

						else if (temp_block[0].like(second_genome[0]))
						{
							temp_block[0].instruction = temp_first_genome[0].instruction;
							modified_block.push(temp_block);
							found = true;
						}

						else
							modified_block.push(temp_block);
					}
				}
			}

			if (found)
				push(modified_block);

			else
				push(original_genome);

			return found;
		}

		// Purpose: 
		//   Returns the position of the provided item in the genome or -1 if not found.
		//
		// Parameters:
		//   other_genome	- Genome to search for
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
		int contains(Genome<Atom> &other_genome)
		{
			bool found = false;
			int position = -1;
			Genome<Atom> original_genome(this);

			if (Utilities::FixedSizeStack<T>::size() < other_genome.size())
				return -1;

			// If searching for a single item
			if (other_genome.size() == 1)
			{
				while (Utilities::FixedSizeStack<T>::empty() == false)
				{
					Genome<Atom> temp_block;

					pop_item(temp_block);	// May need to push back empty blocks if there were extra blocks returned by pop().

					position++;

					if (temp_block == other_genome)
					{
						found = true;
						break;
					}

					else
					{
						if (temp_block.size() > 1)
						{
							Atom atom = temp_block.pop();

							if (temp_block.contains(other_genome) >= 0)
							{
								found = true;
								break;
							}

							temp_block.push(atom);
						}

						else if (temp_block[0].like(other_genome[0]))
						{
							found = true;
							break;
						}
					}
				}
			}

			// If searching for a block
			else
			{
				while (Utilities::FixedSizeStack<T>::empty() == false)
				{
					Genome<Atom> temp_block;

					pop_item(temp_block);

					position++;

					// Normalliize to one block
					int extra_blocks = 0;

					if (temp_block[0].close_parentheses > other_genome[0].close_parentheses)
					{
						extra_blocks = temp_block[0].close_parentheses - other_genome[0].close_parentheses;
						temp_block[0].close_parentheses = other_genome[0].close_parentheses;
					}

					if (temp_block == other_genome)
					{
						found = true;
						break;
					}

					else
					{
						if (temp_block.size() > 1)
						{
							Atom atom = temp_block.pop();

							if (temp_block.contains(other_genome) >= 0)
							{
								found = true;
								break;
							}

							temp_block.push(atom);
						}

						else if (temp_block[0].like(other_genome[0]))
						{
							found = true;
							break;
						}
					}
				}
			}

			Utilities::FixedSizeStack<T>::clear();
			push(original_genome);

			if (found)
				return position;

			else
				return -1;
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
		bool container(Genome<Atom> &other_genome, Genome<Atom> &container)
		{
			bool found = false;
			Genome<Atom> original_genome(this);
			Genome<Atom> container_block;
			Genome<Atom> container_block_canidate;
			Genome<Atom> other_block;

			//container.clear();
			//container.push(CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 1}"));

			if (Utilities::FixedSizeStack<T>::size() < other_genome.size())
				return false;

			// If searching for a single item
			if (other_genome.size() == 1)
			{
				while (Utilities::FixedSizeStack<T>::empty() == false)
				{
					container_block_canidate.clear();
					container_block_canidate.push(other_block);

					pop_item(other_block);	// May need to push back empty blocks if there were extra blocks returned by pop().

//					if ((other_block == other_genome) && (container_block_canidate.empty() == false))
					if (other_block == other_genome)
					{
						Atom atom = container.pop();
						container.clear();
						other_block[0].close_parentheses = 1;
						container.push(other_block);
						container.push(atom);

						found = true;
						break;
					}
					else
					{
						if (other_block.size() > 1)
						{
							Atom atom = other_block.pop();
							container.clear();
							container.push(atom);

							if (other_block.container(other_genome, container))
							{
								found = true;
								break;
							}

							other_block.push(atom);
						}

						else if (other_block[0].like(other_genome[0]))
						{
							Atom atom = container.pop();
							container.clear();
							other_block[0].close_parentheses = 1;
							container.push(other_block);
							container.push(atom);

							found = true;
							break;
						}
					}
				}
			}

			// If searching for a block
			else
			{
				while (Utilities::FixedSizeStack<T>::empty() == false)
				{
					container_block_canidate.clear();
					container_block_canidate.push(other_block);

					pop_item(other_block);

					// Normalliize to one block
					int extra_blocks = 0;

					if (other_block[0].close_parentheses > other_genome[0].close_parentheses)
					{
						extra_blocks = other_block[0].close_parentheses - other_genome[0].close_parentheses;
						other_block[0].close_parentheses = other_genome[0].close_parentheses;
					}

//					if ((other_block == other_genome) && (container_block_canidate.empty() == false))
					if (other_block == other_genome)
					{
						Atom atom = container.pop();
						container.clear();
						other_block[0].close_parentheses = other_genome[0].close_parentheses;
						container.push(other_block);
						container.push(atom);

						found = true;
						break;
					}

					else
					{
						if (other_block.size() > 1)
						{
							Atom atom = other_block.pop();
							container.clear();
							container.push(atom);

							if (other_block.container(other_genome, container))
							{
								found = true;
								break;
							}

							other_block.push(atom);
						}

						else if (other_block[0].like(other_genome[0]))
						{
							Atom atom = container.pop();
							container.clear();
							other_block[0].close_parentheses = other_genome[0].close_parentheses;
							container.push(other_block);
							container.push(atom);

							found = true;
							break;
						}
					}
				}
			}

			Utilities::FixedSizeStack<T>::clear();
			push(original_genome);

			return found;
		}
	};
}
