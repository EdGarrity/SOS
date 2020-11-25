#pragma once

#include <vector>
#include "Processor.h"
#include "Plush.StaticInit.h"
#include "TypedAtom.h"

namespace Plush
{
	class Instruction : public TypedAtom
	{

	};


	inline void push_make_instruction(Operator op, std::string type, std::string name, Type in)
	{
		static_initializer.register_pushfunc(op, type, name);
	}























	void initGenerics();

	/* generic functions */

	template <typename T>
	inline unsigned dup(Environment & _env)
	{
		// Check for valid parameters
		if (_env.has_elements<T>(1))
			_env.push<T>(_env.get_top<T>());

		return 1;
	}
	template <>
	inline unsigned dup<CodeAtom>(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(1))
		{
			Genome_section<CodeAtom> genome = _env.peek_genome<CodeAtom>(0);
			_env.push<CodeAtom>(genome);
		}

		return 1;
	}
	template <>
	inline unsigned dup<ExecAtom>(Environment & _env)
	{
		if (_env.has_elements<ExecAtom>(1))
		{
			Genome_section<ExecAtom> genome = _env.peek_genome<ExecAtom>(0);
			_env.push<ExecAtom>(genome);
		}

		return 1;
	}

	template <class T>
	inline unsigned pop_safe_index(Environment & _env)
	{
		int index = _env.pop<long>();
		int stacksize = _env.get_stack<T>().size();

		index = (index >= stacksize) ? stacksize - 1 : index;
		index = (index < 0) ? 0 : index;

		return index;
	}

	template <class T>
	inline unsigned yank(Environment & _env)
	{
		unsigned int effort = 1;

		// Check for valid parameters
		if ((_env.has_elements<long>(1)) && (_env.has_elements<T>(1)))
		{
			unsigned int index = pop_safe_index<T>(_env);

			// Need to change to 0-based index
			if (index > 0)
			{
				// Get item from deep in stack
				T v = _env.get_atom<T>(index);

				// Remove item from deep in stack
				unsigned int stack_size = _env.get_stack<T>().size();
				unsigned int delete_position = stack_size - index - 1;
				Genome<T>& stack = _env.get_stack<T>();

				if ((stack_size < domain::argmap::maximum_stack_size - 1) && (stack_size > 1))
				{
					unsigned int stack_pointer = delete_position;

					while (stack_pointer < stack_size - 1)
					{
						stack[stack_pointer] = stack[stack_pointer + 1];
						stack_pointer++;
						effort++;
					}
				}

				// Push removed item to top of stack
				stack[stack_size - 1] = v;
			}
		}

		return effort;
	}

	template<>
	inline unsigned yank<CodeAtom>(Environment & _env)
	{
		if ((_env.has_elements<long>(1)) && (_env.has_elements<CodeAtom>(1)))
		{
			unsigned int index = pop_safe_index<CodeAtom>(_env);
			Genome<CodeAtom>& genome = _env.get_stack<CodeAtom>();
			genome.yank_item(index);
		}

		return 1;
	}

	template<>
	inline unsigned yank<ExecAtom>(Environment & _env)
	{
		if ((_env.has_elements<long>(1)) && (_env.has_elements<ExecAtom>(1)))
		{
			unsigned int index = pop_safe_index<ExecAtom>(_env);
			Genome<ExecAtom>& genome = _env.get_stack<ExecAtom>();
			genome.yank_stack_element(index);
		}

		return 1;
	}

	template <class T>
	inline unsigned yankdup(Environment & _env)
	{
		// Check for valid parameters
		if ((_env.has_elements<long>(1)) && (_env.has_elements<T>(1)))
		{
			int index = pop_safe_index<T>(_env);

			T v = _env.get_atom<T>(index);
			_env.push<T>(v);
		}

		return 1;
	}

	template<>
	inline unsigned yankdup<CodeAtom>(Environment & _env)
	{
		if ((_env.has_elements<long>(1)) && (_env.has_elements<CodeAtom>(1)))
		{
			unsigned int index = pop_safe_index<CodeAtom>(_env);
			Genome<CodeAtom>& genome = _env.get_stack<CodeAtom>();
			genome.yankdup_stack_element(index);
		}

		return 1;
	}

	template<>
	inline unsigned yankdup<ExecAtom>(Environment & _env)
	{
		if ((_env.has_elements<long>(1)) && (_env.has_elements<ExecAtom>(1)))
		{
			unsigned int index = pop_safe_index<ExecAtom>(_env);
			Genome<CodeAtom>& genome = _env.get_stack<CodeAtom>();
			genome.yankdup_stack_element(index);
		}

		return 1;
	}

	// Need spcial cases for EXEC and CODE
	template <typename T> 
	inline unsigned equals(Environment & _env)
	{
		// Check for valid parameters
		if (_env.has_elements<T>(2))
			_env.push<bool>(_env.pop<T>() == _env.pop<T>());

		return 1;
	}

	template<>
	inline unsigned equals<CodeAtom>(Environment & _env)
	{
		// Check for valid parameters
		if (_env.has_elements<CodeAtom>(2))
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
		}

		return 1;
	}
		
	template<>
	inline unsigned equals<ExecAtom>(Environment & _env)
	{
		// Check for valid parameters
		if (_env.has_elements<ExecAtom>(2))
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
		}

		return 1;
	}


	template <class T>
	inline unsigned flush(Environment & _env)
	{
		_env.get_stack<T>().clear();
		return 1;
	}

	template <typename  T>
	inline unsigned protected_pop(Environment & _env)
	{
		if (_env.has_elements<T>(1))
			_env.pop<T>();

		return 1;
	}
	template <>
	inline unsigned protected_pop<CodeAtom>(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(1))
			_env.pop_genome<CodeAtom>();

		return 1;
	}
	template <>
	inline unsigned protected_pop<ExecAtom>(Environment & _env)
	{
		if (_env.has_elements<ExecAtom>(1))
			_env.pop_genome<ExecAtom>();

		return 1;
	}

	template <class T>
	unsigned rot(Environment & _env)
	{
		if (_env.has_elements<T>(3))
		{
			T x = _env.pop<T>();
			T y = _env.pop<T>();
			T z = _env.pop<T>();
			_env.push<T>(y);
			_env.push<T>(x);
			_env.push<T>(z);
		}

		return 1;
	}

	template<>
	inline unsigned rot<CodeAtom>(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(3))
		{
			Genome<CodeAtom>& genome = _env.get_stack<CodeAtom>();
			genome.yank_stack_element(2);
		}

		return 1;
	}

	template<>
	inline unsigned rot<ExecAtom>(Environment & _env)
	{
		if (_env.has_elements<ExecAtom>(3))
		{
			Genome<ExecAtom>& genome = _env.get_stack<ExecAtom>();
			genome.yank_stack_element(2);
		}

		return 1;
	}

	template <class T>
	inline unsigned shove(Environment & _env)
	{
		unsigned int effort = 1;

		if ((_env.has_elements<long>(1)) && (_env.has_elements<T>(1)))
		{
			int index = pop_safe_index<T>(_env);
			int stack_size = _env.get_stack<T>().size();

			int insert_position = stack_size - index - 1;

			Genome<T>& stack = _env.get_stack<T>();

			if ((stack_size < domain::argmap::maximum_stack_size - 1) && (stack_size > 1))
			{
				T v = _env.get_top<T>();

				int stack_pointer = stack_size - 1;

				while ((stack_pointer >= 0) && (stack_pointer >= insert_position))
				{
					stack[stack_pointer + 1] = stack[stack_pointer];
					stack_pointer--;
				}

				stack[stack_pointer + 1] = v;
				_env.push(stack[stack_size]);
			}

			effort = stack.size() - insert_position + 1;
		}

		return effort;
	}

	template<>
	inline unsigned shove<CodeAtom>(Environment & _env)
	{
		if ((_env.has_elements<long>(1)) && (_env.has_elements<CodeAtom>(1)))
		{
			unsigned int index = pop_safe_index<CodeAtom>(_env);

			//if (index > 0)
			//{
				Genome<CodeAtom>& genome = _env.get_stack<CodeAtom>();

				Genome_section<CodeAtom> section = genome[0];

				if (section.size > 0)
					genome.shove_to_stack_element(index);
			//}
		}

		return 1;
	}

	template<>
	inline unsigned shove<ExecAtom>(Environment & _env)
	{
		if ((_env.has_elements<long>(1)) && (_env.has_elements<ExecAtom>(1)))
		{
			unsigned int index = pop_safe_index<ExecAtom>(_env);

			//if (index > 0)
			//{
				Genome<ExecAtom>& genome = _env.get_stack<ExecAtom>();

				Genome_section<ExecAtom> section = genome[0];

				if (section.size > 0)
					genome.shove_to_stack_element(index);
			//}
		}

		return 1;
	}

	template <class T>
	inline unsigned stackdepth(Environment & _env)
	{
		_env.push<long>(_env.get_stack<T>().size());
		return 1;
	}

	template <typename T>
	inline unsigned swap(Environment & _env)
	{
		if (_env.has_elements<T>(2))
		{
			Genome<T>& stack = _env.get_stack<T>();

			unsigned int stack_size = _env.get_stack<T>().size();

			T tmp = stack[stack_size - 1];
			stack[stack_size - 1] = stack[stack_size - 2];
			stack[stack_size - 2] = tmp;
		}

		return 1;
	}

	template<>
	inline unsigned swap<CodeAtom>(Environment & _env)
	{
		if (_env.has_elements<CodeAtom>(2))
		{
			Genome<CodeAtom>& genome = _env.get_stack<CodeAtom>();
			genome.yank_stack_element(1);
		}

		return 1;
	}

	template<>
	inline unsigned swap<ExecAtom>(Environment & _env)
	{
		if (_env.has_elements<ExecAtom>(2))
		{
			Genome<ExecAtom>& genome = _env.get_stack<ExecAtom>();
			genome.yank_stack_element(1);
		}

		return 1;
	}
}