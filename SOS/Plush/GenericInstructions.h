#pragma once
#include "Environment.h"

namespace Plush
{
	/* generic functions */

	template <typename T>
	inline size_t dup(Environment & _env)
	{
		_env.push<T>(_env.get_top_atom<T>());

		return 1;
	}
	template <>
	inline size_t dup<CodeAtom>(Environment & _env)
	{
		_env.push<CodeAtom>(_env.peek_genome<CodeAtom>(0));

		return 1;
	}
	template <>
	inline size_t dup<ExecAtom>(Environment & _env)
	{
		_env.push<ExecAtom>(_env.peek_genome<ExecAtom>(0));

		return 1;
	}

	template <class T>
	inline size_t pop_safe_position(Environment & _env)
	{
		int index = _env.pop<long>();
		size_t stacksize = _env.get_stack<T>().size();

		index = (index < 0) ? 0 : index;
		size_t i = (index >= stacksize) ? stacksize - 1 : index;

		return i;
	}

	template <class T>
	inline size_t yank(Environment & _env)
	{
		size_t effort = 1;

		// Check for valid parameters
		size_t position = pop_safe_position<T>(_env);

		// Need to change to 0-based index
		// Get item from deep in stack
		T atom = _env.get_atom_at_position<T>(position);

		// Remove item from deep in stack
		size_t stack_size = _env.get_stack<T>().size();
		size_t delete_index = _env.position_to_index<T>(position);
		Genome<T>& stack = _env.get_stack<T>();

		if (stack_size > 1)
		{
			size_t stack_pointer = delete_index;

			while (stack_pointer < stack_size - 1)
			{
				stack[stack_pointer] = stack[stack_pointer + 1];
				stack_pointer++;
				effort++;
			}
		}

		// Push removed item to top of stack
		stack[stack_size - 1] = atom;

		return effort;
	}

	template<>
	inline size_t yank<CodeAtom>(Environment & _env)
	{
		return _env.get_stack<CodeAtom>().yank_item(pop_safe_position<CodeAtom>(_env));
	}

	template<>
	inline size_t yank<ExecAtom>(Environment & _env)
	{
		return _env.get_stack<ExecAtom>().yank_item(pop_safe_position<ExecAtom>(_env));
	}

	template <class T>
	inline size_t yankdup(Environment & _env)
	{
		return _env.push<T>(_env.get_atom_at_position<T>(pop_safe_position<T>(_env)));
	}

	template<>
	inline size_t yankdup<CodeAtom>(Environment & _env)
	{
		return _env.get_stack<CodeAtom>().yankdup_stack_element(pop_safe_position<CodeAtom>(_env));
	}

	template<>
	inline size_t yankdup<ExecAtom>(Environment & _env)
	{
		return _env.get_stack<CodeAtom>().yankdup_stack_element(pop_safe_position<ExecAtom>(_env));
	}

	template <typename T>
	inline size_t equals(Environment & _env)
	{
		_env.push<bool>(_env.pop<T>() == _env.pop<T>());

		return 1;
	}

	template<>
	inline size_t equals<CodeAtom>(Environment & _env)
	{
		Genome<CodeAtom>& genome = _env.get_stack<CodeAtom>();
		Genome_section<CodeAtom> block_a = genome[0];
		Genome_section<CodeAtom> block_b = genome[1];

		if (genome.comp(block_a, block_b))
			_env.push<bool>(true);
		else
			_env.push<bool>(false);

		_env.pop_genome<CodeAtom>();
		_env.pop_genome<CodeAtom>();

		return 1;
	}

	template<>
	inline size_t equals<ExecAtom>(Environment & _env)
	{
		Genome<ExecAtom>& genome = _env.get_stack<ExecAtom>();

		Genome_section<ExecAtom> block_a = genome[0];
		Genome_section<ExecAtom> block_b = genome[1];

		if (genome.comp(block_a, block_b))
			_env.push<bool>(true);
		else
			_env.push<bool>(false);

		_env.pop_genome<ExecAtom>();
		_env.pop_genome<ExecAtom>();

		return 1;
	}


	template <class T>
	inline size_t flush(Environment & _env)
	{
		_env.get_stack<T>().clear();
		return 1;
	}

	template <typename  T>
	inline size_t protected_pop(Environment & _env)
	{
		_env.pop<T>();

		return 1;
	}
	template <>
	inline size_t protected_pop<CodeAtom>(Environment & _env)
	{
		_env.pop_genome<CodeAtom>();

		return 1;
	}
	template <>
	inline size_t protected_pop<ExecAtom>(Environment & _env)
	{
		_env.pop_genome<ExecAtom>();

		return 1;
	}

	template <class T>
	inline size_t rot(Environment & _env)
	{
		T x = _env.pop<T>();
		T y = _env.pop<T>();
		T z = _env.pop<T>();
		_env.push<T>(y);
		_env.push<T>(x);
		_env.push<T>(z);

		return 1;
	}

	template<>
	inline size_t rot<CodeAtom>(Environment & _env)
	{
		_env.get_stack<CodeAtom>().yank_stack_element(2);
		
		return 1;
	}

	template<>
	inline size_t rot<ExecAtom>(Environment & _env)
	{
		_env.get_stack<ExecAtom>().yank_stack_element(2);

		return 1;
	}

	template <class T>
	inline size_t shove(Environment & _env)
	{
		size_t effort = 1;

		int insert_relative_position = pop_safe_position<T>(_env);
		int stack_size = _env.get_stack<T>().size();
		int insert_absolute_index = stack_size - insert_relative_position - 1;

		Genome<T>& stack = _env.get_stack<T>();

		if (stack_size > 1)
		{
			// Get copy of the top atom
			T top_atom = _env.get_top_atom<T>();

			// Allocate space in the stack for the inserted atom.
			_env.get_stack<T>().set_top(stack_size + 1);

			// Set pointer to top element of the stack
			int absolute_stack_pointer = stack_size;

			// Create a black space in the stack at the insertion point.
			while ((absolute_stack_pointer > 0) && (absolute_stack_pointer > insert_absolute_index))
			{
				stack[absolute_stack_pointer] = stack[absolute_stack_pointer - 1];
				absolute_stack_pointer--;
			}

			stack[insert_absolute_index] = top_atom;
		}

		effort = stack.size() - insert_absolute_index + 1;

		return effort;
	}

	template<>
	inline size_t shove<CodeAtom>(Environment & _env)
	{
		Genome<CodeAtom>& genome = _env.get_stack<CodeAtom>();

		Genome_section<CodeAtom> section = genome[0];

		if (section.size > 0)
			genome.shove_to_stack_element(pop_safe_position<CodeAtom>(_env));

		return 1;
	}

	template<>
	inline size_t shove<ExecAtom>(Environment & _env)
	{
		Genome<ExecAtom>& genome = _env.get_stack<ExecAtom>();

		Genome_section<ExecAtom> section = genome[0];

		if (section.size > 0)
			genome.shove_to_stack_element(pop_safe_position<ExecAtom>(_env));

		return 1;
	}

	template <class T>
	inline size_t stackdepth(Environment & _env)
	{
		_env.push<long>(_env.get_stack<T>().size());
		return 1;
	}

	template <typename T>
	inline size_t swap(Environment & _env)
	{
		Genome<T>& stack = _env.get_stack<T>();

		size_t stack_size = _env.get_stack<T>().size();

		T tmp = stack[stack_size - 1];
		stack[stack_size - 1] = stack[stack_size - 2];
		stack[stack_size - 2] = tmp;

		return 1;
	}

	template<>
	inline size_t swap<CodeAtom>(Environment & _env)
	{
		return _env.get_stack<CodeAtom>().yank_stack_element(1);
	}

	template<>
	inline size_t swap<ExecAtom>(Environment & _env)
	{
		return _env.get_stack<ExecAtom>().yank_stack_element(1);
	}
}