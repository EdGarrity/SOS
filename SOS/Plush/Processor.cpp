#include "Processor.h"
#include "Plush.StaticInit.h"
#include "..\Domain\Arguments.h"
#include "..\Utilities\String.h"

namespace Plush
{
	typedef unsigned(*Operator)(Environment &env);
	typedef std::map<std::string, Operator> Func2CodeMapType;

	extern 	Func2CodeMapType Func2CodeMap;

	//template <class T> 
	//std::pair<size_t, size_t> get_block_index(Environment & _env, unsigned int _block_number, unsigned int _added_opens)
	//{
	//	enum block_states
	//	{
	//		close = 0,
	//		close_and_open
	//	};

	//	Utilities::FixedSizeStack<block_states> block_stack;

	//	int blocks_needed = _added_opens;
	//	int added_opens = _added_opens;
	//	int prev_added_opens = 0;
	//	int hidden_blocks = 0;
	//	int block_level = 0;
	//	int prev_block_level = 0;
	//	bool top_level_block_start = false;
	//	int blocks_closed = 0;
	//	bool previously_poped_close_open = false;
	//	bool top_level_block_end = false;
	//	int start_index = SIZE_MAX;
	//	int end_index = SIZE_MAX;
	//	bool found = false;

	//	if (_added_opens > 0)
	//	{
	//		block_stack.push(block_states::close);
	//		block_level++;
	//	}

	//	// Zero based index of stack block to yank
	//	unsigned int index_of_block_we_are_looking_for = _block_number;


	//	std::array<T, domain::argmap::maximum_stack_size>& plush_stack = _env.get_stack<T>().container();

	//	unsigned int exec_stack_index = _env.get_stack<T>().size();

	//	while (exec_stack_index > 0)
	//	{
	//		bool poped_close_open = false;
	//		bool popped_open_close = false;

	//		exec_stack_index--;

	//		T atom = plush_stack[exec_stack_index];

	//		blocks_needed = Func2BlockWantsMap[atom.instruction];
	//		added_opens = blocks_needed - atom.close_parentheses;

	//		for (int n = 0; n < added_opens; n++)
	//		{
	//			if (n == 0)
	//			{
	//				block_stack.push(block_states::close);
	//				block_level++;
	//			}

	//			else
	//				block_stack.push(block_states::close_and_open);
	//		}

	//		bool popped = false;

	//		for (int n = 0; n < atom.close_parentheses; n++)
	//		{
	//			if (block_stack.empty())
	//				break;

	//			if (block_stack.top() == block_states::close)
	//			{
	//				block_level--;
	//				popped_open_close = true;
	//			}

	//			else if (block_stack.top() == block_states::close_and_open)
	//				poped_close_open = true;

	//			block_stack.pop();
	//			popped = true;
	//		}

	//		hidden_blocks = ((blocks_needed > 0) && (blocks_needed > added_opens)) ? (blocks_needed - added_opens) : 0;

	//		if (start_index == SIZE_MAX)
	//		{
	//			if ((prev_block_level == 0) && (block_level == 1))
	//				top_level_block_start = true;

	//			else if ((prev_block_level == 0) && (popped_open_close))
	//				top_level_block_start = true;

	//			else if ((block_level <= 1) && (previously_poped_close_open))
	//				top_level_block_start = true;

	//			else if ((prev_added_opens > 0) && (block_level == 1))
	//				top_level_block_start = true;

	//			else
	//				top_level_block_start = false;
	//		}

	//		else
	//			top_level_block_start = false;

	//		if ((popped) && (block_level == 0))
	//			top_level_block_end = true;

	//		else
	//			top_level_block_end = false;


	//		if ((index_of_block_we_are_looking_for > 0) && ((hidden_blocks > 0) || (top_level_block_start)))
	//			index_of_block_we_are_looking_for--;

	//		// Check if this is the block we are looking for
	//		if (index_of_block_we_are_looking_for == 0)
	//		{
	//			// found start of block
	//			if (start_index == SIZE_MAX)
	//				start_index = exec_stack_index;

	//			if ((block_level == 0) || (top_level_block_end))
	//			{
	//				// found end of block
	//				end_index = exec_stack_index;
	//				found = true;
	//				break;
	//			}
	//		}

	//		previously_poped_close_open = poped_close_open;
	//		prev_added_opens = added_opens;
	//		prev_block_level = block_level;
	//	}

	//	return std::make_pair(end_index, start_index);
	//}

	void run(Environment& env, std::string _program)
	{
		std::string gene;
		Utilities::FixedSizeStack<Atom> code_stack;

		while (_program.length() > 0)
		{
			gene = first_atom(_program);
			_program = rest_atom(_program);
			Utilities::trim(_program);

			Atom atom(gene);

			code_stack.push(atom);
		}

		while (!code_stack.empty())
		{
			env.get_stack<CodeAtom>().push(CodeAtom(code_stack.top()));
			env.get_stack<ExecAtom>().push(ExecAtom(code_stack.top()));
			code_stack.pop();
		}

		// The basic pop-exec cycle
		unsigned effort = 0;

		while ((!env.is_empty<ExecAtom>()) && (effort < domain::argmap::max_point_evaluations))
		{
			unsigned unit = 0;

			ExecAtom atom = env.pop<ExecAtom>();

			switch (atom.type)
			{
			case Atom::AtomType::integer:
				env.push<long>(std::stol(atom.instruction));
				unit = 1;
				break;
			case Atom::AtomType::floating_point:
				env.push<double>(std::stod(atom.instruction));
				unit = 1;
				break;
			case Atom::AtomType::boolean:
				env.push<bool>(atom.instruction == "TRUE");
				unit = 1;
				break;
			case Atom::AtomType::ins:
				auto search = Func2CodeMap.find(atom.instruction);
				
				if (search != Func2CodeMap.end())
				{
					Operator op = Func2CodeMap[atom.instruction];
					op(env);
				}

				break;
			}

			effort += (1u) > (unit) ? (1u) : (unit);
		}
	}
}