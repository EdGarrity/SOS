#pragma once

//#include "Processor.h"
#include "Atom.h"
#include <vector>
#include "..\Utilities\FixedSizeStack.h"

namespace Plush
{
	typedef std::map<std::string, unsigned int> Func2BlockWantsMapType;
	extern Func2BlockWantsMapType Func2BlockWantsMap;





	class Environment
	{
	private:
		Utilities::FixedSizeStack<class ExecAtom> exec_stack_;
		Utilities::FixedSizeStack<class CodeAtom> code_stack_;
		Utilities::FixedSizeStack<long> int_stack_;
		Utilities::FixedSizeStack<double> double_stack_;
		Utilities::FixedSizeStack<bool> bool_stack_;
		
	public:
		virtual void clear_stacks()
		{
			exec_stack_.clear();
			int_stack_.clear();
			code_stack_.clear();
			bool_stack_.clear();
			double_stack_.clear();
		}






//		template <class T>
////		inline std::pair<size_t, size_t> get_block_index(Environment & _env, unsigned int _block_number, unsigned int _added_opens)
//		inline std::pair<size_t, size_t> get_block_index(unsigned int _block_number, unsigned int _added_opens)
//		{
//			enum block_states
//			{
//				close = 0,
//				close_and_open
//			};
//
//			Utilities::FixedSizeStack<block_states> block_stack;
//
//			int blocks_needed = _added_opens;
//			int added_opens = _added_opens;
//			int prev_added_opens = 0;
//			int hidden_blocks = 0;
//			int block_level = 0;
//			int prev_block_level = 0;
//			bool top_level_block_start = false;
//			int blocks_closed = 0;
//			bool previously_poped_close_open = false;
//			bool top_level_block_end = false;
//			int start_index = SIZE_MAX;
//			int end_index = SIZE_MAX;
//			bool found = false;
//
//			if (_added_opens > 0)
//			{
//				block_stack.push(block_states::close);
//				block_level++;
//				_block_number--;
//			}
//
//			// Zero based index of stack block to yank
//			unsigned int index_of_block_we_are_looking_for = _block_number + 1;
//			unsigned int index_of_current_top_block = 0;
//
//
//			std::array<T, domain::argmap::maximum_stack_size>& plush_stack = get_stack<T>().container();
//
//			unsigned int exec_stack_index = get_stack<T>().size();
//
//			while (exec_stack_index > 0)
//			{
//				bool poped_close_open = false;
//				bool popped_open_close = false;
//
//				exec_stack_index--;
//
//				T atom = plush_stack[exec_stack_index];
//
//				blocks_needed = Func2BlockWantsMap[atom.instruction];
//				added_opens = blocks_needed - atom.close_parentheses;
//
//				for (int n = 0; n < added_opens; n++)
//				{
//					if (n == 0)
//					{
//						block_stack.push(block_states::close);
//						block_level++;
//					}
//
//					else
//						block_stack.push(block_states::close_and_open);
//				}
//
//				bool popped = false;
//
//				for (int n = 0; n < atom.close_parentheses; n++)
//				{
//					if (block_stack.empty())
//						break;
//
//					if (block_stack.top() == block_states::close)
//					{
//						block_level--;
//						popped_open_close = true;
//					}
//
//					else if (block_stack.top() == block_states::close_and_open)
//						poped_close_open = true;
//
//					block_stack.pop();
//					popped = true;
//				}
//
//				hidden_blocks = ((blocks_needed > 0) && (blocks_needed > added_opens)) ? (blocks_needed - added_opens) : 0;
//
//				if (start_index == SIZE_MAX)
//				{
//					if ((prev_block_level == 0) && (block_level == 1))
//						top_level_block_start = true;
//
//					else if ((prev_block_level == 0) && (popped_open_close))
//						top_level_block_start = true;
//
//					else if ((block_level <= 1) && (previously_poped_close_open))
//						top_level_block_start = true;
//
//					else if ((prev_added_opens > 0) && (block_level == 1))
//						top_level_block_start = true;
//
//					else
//						top_level_block_start = false;
//				}
//
//				else
//					top_level_block_start = false;
//
//				if ((popped) && (block_level == 0))
//					top_level_block_end = true;
//
//				else
//					top_level_block_end = false;
//
//				if ((blocks_needed == 0) && (block_level == 0) && (popped_open_close == false))
//					index_of_current_top_block++;
//
//				else if ((hidden_blocks > 0) || (top_level_block_start))
//					index_of_current_top_block++;
//
//				if (index_of_current_top_block == index_of_block_we_are_looking_for)
//				{
//					// found start of block
//					if (start_index == SIZE_MAX)
//						start_index = exec_stack_index;
//
//					if ((block_level == 0) || (top_level_block_end))
//					{
//						// found end of block
//						end_index = exec_stack_index;
//						found = true;
//						break;
//					}
//				}
//
//				previously_poped_close_open = poped_close_open;
//				prev_added_opens = added_opens;
//				prev_block_level = block_level;
//			}
//
//			return std::make_pair(end_index, start_index);
//		};






		/* Operations */
		template <typename T> inline Utilities::FixedSizeStack<T> &get_stack() { }
		template <> inline Utilities::FixedSizeStack<ExecAtom> &get_stack()
		{
			return exec_stack_;
		}
		template <> inline Utilities::FixedSizeStack<CodeAtom> &get_stack()
		{
			return code_stack_;
		}
		template <> inline Utilities::FixedSizeStack<long> &get_stack()
		{
			return int_stack_;
		}
		template <> inline Utilities::FixedSizeStack<double> &get_stack()
		{
			return double_stack_;
		}
		template <> inline Utilities::FixedSizeStack<bool> &get_stack()
		{
			return bool_stack_;
		}
		
		template <typename T> inline bool is_empty()
		{
			return (get_stack<T>().empty());
		}

		/* pushing and popping */

		// Need spcial cases for pushing an array of EXEC and CODE
		template <typename T>
		inline void push(T value)
		{
			get_stack<T>().push(value);
		}

		template <class T>
		inline void push(Utilities::FixedSizeStack<T> stack)
		{
			//for (int n = 0; n < stack.size(); n++)
			//	get_stack<T>().push(stack[n]);
			if (stack.size() > 0)
				for (int n = stack.size() - 1; n >= 0; n--)
					get_stack<T>().push(stack[n]);
		}

		// Need spcial cases for EXEC and CODE
		// Need to support receiving and returning stack of instructions
		// If number of parenthesis balance, then pop will retuirn first instruction
		// for impabance, then return balanced stack (from first instruction to instuction with matching close parenthesis inclusive)
		template <typename T>
		inline T pop()
		{
			T val = get_stack<T>().top();
			get_stack<T>().pop();
			return val;
		}

		template <typename T> inline int &pop(Utilities::FixedSizeStack<T> &stack) { }
		template <> inline int &pop(Utilities::FixedSizeStack<ExecAtom> &stack)
		{
			int blocks_open = 1;

			while ((blocks_open > 0) && (get_stack<ExecAtom>().size() > 0))
			{
				Atom atom = get_stack<ExecAtom>().top();
				get_stack<ExecAtom>().pop();
				stack.push(atom);

				blocks_open += Func2BlockWantsMap[atom.instruction];
				blocks_open -= atom.close_parentheses;
			};
			
			blocks_open *= -1;

			return blocks_open; // Return number of unmatched close parenthesis.
		}

		// Need spcial cases for EXEC and CODE
		template <typename T>
		inline T top()
		{
			T val = get_stack<T>().top();
			return val;
		}

		template <typename T>
		inline T top(T val)
		{
			get_stack<T>().top() = val;
			return val;
		}

		// Need spcial cases for EXEC and CODE
		template <typename T>
		inline bool has_elements(unsigned sz)
		{
			if (get_stack<T>().size() < sz)
				return false;
			else
				return true;
		}

		// Need spcial cases for EXEC and CODE
		template <typename T>
		inline T peek_index(unsigned index)
		{
			if (has_elements<T>(index + 1))
				return get_stack<T>().container()[get_stack<T>().size() - index - 1];

			else
				throw;
		}

		template <typename T> inline int &peek_index(unsigned index, unsigned open_blocks, Utilities::FixedSizeStack<T> &stack) { }
		template <> inline int &peek_index(unsigned index, unsigned open_blocks, Utilities::FixedSizeStack<CodeAtom> &stack)
		{
			int i = 0;
			int blocks_open = open_blocks;

			for (int n = 0; n < index + 1; n++)
			{
//				blocks_open = 1;

				if (blocks_open == 0)
				{
					CodeAtom atom = peek_index<CodeAtom>(n);

					if (n == index)
						stack.push(atom);

					blocks_open += Func2BlockWantsMap[atom.instruction];
					blocks_open -= atom.close_parentheses;

					i++;
				}

				else
				{
					while ((blocks_open > 0) && (i < get_stack<CodeAtom>().size()))
					{
						CodeAtom atom = peek_index<CodeAtom>(n);

						if (n == index)
							stack.push(atom);

						blocks_open += Func2BlockWantsMap[atom.instruction];
						blocks_open -= atom.close_parentheses;

						i++;
					};
				}
			}

			blocks_open *= -1;

			return blocks_open; // Return number of unmatched close parenthesis.
		}
		template <> inline int &peek_index(unsigned index, unsigned open_blocks, Utilities::FixedSizeStack<ExecAtom> &stack)
		{
			int i = 0;
			int blocks_open = open_blocks;

			for (int n = 0; n < index + 1; n++)
			{
				//				blocks_open = 1;

				if (blocks_open == 0)
				{
					ExecAtom atom = peek_index<ExecAtom>(n);

					if (n == index)
						stack.push(atom);

					blocks_open += Func2BlockWantsMap[atom.instruction];
					blocks_open -= atom.close_parentheses;

					i++;
				}

				else
				{
					while ((blocks_open > 0) && (i < get_stack<ExecAtom>().size()))
					{
						ExecAtom atom = peek_index<ExecAtom>(n);

						if (n == index)
							stack.push(atom);

						blocks_open += Func2BlockWantsMap[atom.instruction];
						blocks_open -= atom.close_parentheses;

						i++;
					};
				}
			}

			blocks_open *= -1;

			return blocks_open; // Return number of unmatched close parenthesis.
		}
	};
}
