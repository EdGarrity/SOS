#pragma once

#include <ppl.h>
#include "Code.h"
#include "Type.h"
#include "TypeDef.h"
#include "CodeUtils.h" // for cdr

using namespace concurrency;

namespace Push
{
	// This needs to be initialize in Push Initialze and stored in Thread Local Storage
//	extern thread_local Env env;
//	extern combinable<Env> env;

//	typedef std::map<std::string, unsigned int> String2parenthesesMap;
//	extern const String2parenthesesMap &str2parentheses_map;

	struct Parameters
	{
		double max_random_float;				// = 1.0;
		double min_random_float;				// = -1.0;
		int max_random_integer;					// = 10
		int min_random_integer;					// = -10
		int dataload_limit;
		int evalpush_limit;						// = 1000
		double new_erc_name_probability;		// = 0.001
		int max_points_in_random_expression;	// = 50
		unsigned int max_points_in_program;				// = 100
		int random_seed;						// = time.time()
	};

	extern const Parameters global_parameters;

	extern std::vector<double> null_input;

	class Env_detail
	{
	public:
		/* instructions */
		Code function_set;
		Parameters parameters;

		// Pointer to input & output data
		std::vector<double> & input = null_input;
		std::vector<double> output;

		void push_code_to_exec_stack(const Code &code)
		{
			exec_stack.push_back(Exec(code));
		}

		/* Stacks */
		std::vector<Exec>	exec_stack;
		std::vector<int>	int_stack;
		std::vector<Code>	code_stack;
		std::vector<bool>   bool_stack;
		std::vector<double>	double_stack;

//		Env(unsigned _reserve = 1000) : function_set(instructions), parameters(global_parameters)
		Env_detail() : parameters(global_parameters)
		{
			reserve(1000);
			clear_stacks();
			null_input.clear();
			input = null_input;
			output.clear();
		}

		//Env(std::vector<double> & _input, unsigned _reserve = 1000) : parameters(global_parameters)
		//{
		//	reserve(_reserve);
		//	clear_stacks();
		//	input = _input;
		//	output.clear();
		//}

		virtual ~Env_detail()
		{ 
			clear_stacks(); 
		} // virtual ~Env(){ delete next_env; }  // EG: Added clear();

		virtual void initialize(std::vector<double> & _input, unsigned _reserve = 1000)
		{
			function_set = Code(instructions);
			reserve(_reserve);
			clear_stacks();
			input = _input;
			output.clear();
		}

		virtual void reserve(unsigned _reserve)
		{
			exec_stack.reserve(_reserve);
			int_stack.reserve(_reserve);
			code_stack.reserve(_reserve);
			bool_stack.reserve(_reserve);
			double_stack.reserve(_reserve);
		}

		virtual void clear_stacks()
		{
			exec_stack.clear();
			int_stack.clear();
			code_stack.clear();
			bool_stack.clear();
			double_stack.clear();

			exec_stack.shrink_to_fit();
			int_stack.shrink_to_fit();
			code_stack.shrink_to_fit();
			bool_stack.shrink_to_fit();
			double_stack.shrink_to_fit();
		}

		/* Needed for type checking of preconditions */
		virtual size_t get_stack_size(int which) const
		{
			switch (which)
			{
			case EXEC_STACK:
				return exec_stack.size();

			case INTEGER_STACK:
				return int_stack.size();

			case CODE_STACK:
				return code_stack.size();

			case BOOL_STACK:
				return bool_stack.size();

			case FLOAT_STACK:
				return double_stack.size();
			}

			return 0;
		}

		virtual Type make_type() const;

		/* Needed for type based packing */
		virtual Code pop_stack_from_id(int id);

		unsigned go(Env & _env, unsigned n = 50);
		unsigned go_trace(Env & _env, unsigned _max_effort, std::string & trace_line);
	};

	typedef combinable<Push::Env_detail> Env;

	//extern std::string print(Env & _env);
	//inline std::ostream &operator<<(std::ostream &os, Env & _env)
	//{
	//	os << print(_env);
	//	return os;
	//}

	/* Operations */
	template <typename T> inline std::vector<T> &get_stack(Env & _env) { }
	template <> inline std::vector<Exec>   &get_stack(Env & _env)
	{
		return _env.local().exec_stack;
	}
	template <> inline std::vector<int> &get_stack(Env & _env)
	{
		return _env.local().int_stack;
	}
	template <> inline std::vector<double> &get_stack(Env & _env)
	{
		return _env.local().double_stack;
	}
	template <> inline std::vector<bool>   &get_stack(Env & _env)
	{
		return _env.local().bool_stack;
	}
	template <> inline std::vector<Code>   &get_stack(Env & _env)
	{
		return _env.local().code_stack;
	}

	template <typename T> inline bool is_empty(Env & _env)
	{
		return (get_stack<T>(_env).empty());
	}

	/* pushing and popping */
	template <typename T> inline void push(Env & _env, T value)
	{
		get_stack<T>(_env).push_back(value);
	}

	template <typename T> inline T pop(Env & _env)
	{
		T val = get_stack<T>(_env).back();
		get_stack<T>(_env).pop_back();
		return val;
	}

	template <typename T> inline T &top(Env & _env)
	{
		return get_stack<T>(_env).back();
	}

	template <class T> inline    T &first(Env & _env)
	{
		return top<T>(_env);
	}

	template <class T> inline    T &second(Env & _env)
	{
		return get_stack<T>(_env)[get_stack<T>(_env).size() - 2];
	}

	/* Push calling convention */
	inline void push_call(Env & _env, Code code)
	{
		_env.local().push_code_to_exec_stack(code);
		push(_env, code);
	}

	template <typename T>
	inline bool has_elements(Env & _env, unsigned sz)
	{
		if (get_stack<T>(_env).size() < sz)
			return false;
		else
			return true;
	}

	/* generic functions */
	template <typename T>
	inline unsigned dup(Env & _env)
	{
		push<T>(_env, get_stack<T>(_env).back());
		return 1;
	}

	template <typename T>
	inline unsigned swap(Env & _env)
	{
		std::vector<T> &stack = get_stack<T>(_env);
		T tmp = stack.back();
		stack.back() = stack[stack.size() - 2];
		stack[stack.size() - 2] = tmp;

		return 1;
	}

	template <typename T>
	inline unsigned equals(Env & _env)
	{
		push(_env, pop<T>(_env) == pop<T>(_env));
		return 1;
	}

	template <>
	inline unsigned equals<Code>(Env & _env)
	{
		int effort = 0;

		push(_env, equal_to(pop<Code>(_env), pop<Code>(_env), effort));
		return effort;
	}

	template <>
	inline unsigned equals<Exec>(Env & _env)
	{
		int effort = 0;

		push(_env, equal_to(pop<Exec>(_env).to_CodeBase(), pop<Exec>(_env).to_CodeBase(), effort));
		return effort;
	}

	template <class T>
	inline int safe_index(Env & _env)
	{
		int index = pop<int>(_env);
		int stacksize = get_stack<T>(_env).size();
		index = (index < 0) ? 0 : (index >= stacksize ? stacksize - 1 : index);
		return stacksize - 1 - index;
	}

	template <typename T>
	inline unsigned yankdup(Env & _env)
	{
		int index = safe_index<T>(_env);
		std::vector<T> &stack = get_stack<T>(_env);
		stack.push_back(stack[index]);

		return 1;
	}

	template <class T>
	inline unsigned protected_pop(Env & _env)
	{
		pop<T>(_env);
		return 1;
	}

	template <class T>
	inline unsigned stackdepth(Env & _env)
	{
		push<int>(_env, get_stack<T>(_env).size());
		return 1;
	}

	template <class T>
	inline unsigned shove(Env & _env)
	{
		int index = safe_index<T>(_env);

		std::vector<T> &stack = get_stack<T>(_env);
		stack.insert(stack.begin() + index, pop<T>(_env));

		return stack.size() - index + 1;
	}

	template <class T>
	inline unsigned yank(Env & _env)
	{
		int index = safe_index<T>(_env);
		std::vector<T> &stack = get_stack<T>(_env);
		T value = stack[index];
		stack.erase(stack.begin() + index);
		stack.push_back(value);

		return stack.size() - index + 1;
	}

	template <class T>
	unsigned rot(Env & _env)
	{
		T x = pop<T>(_env);
		T y = pop<T>(_env);
		T z = pop<T>(_env);
		push<T>(_env, y);
		push<T>(_env, x);
		push<T>(_env, z);

		return 1;
	}
}