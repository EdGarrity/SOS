#pragma once

#include "Code.h"
#include "Type.h"
#include "TypeDef.h"
#include "CodeUtils.h" // for cdr
#include "../Domain/Stock Forecaster/Broker.h"

namespace Push
{
	// This needs to be initialize in Push Initialze and stored in Thread Local Storage
	extern thread_local Env env;

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
		domain::stock_forecaster::Broker* pBroker;				// Give Push access to the Broker object.
	};

	extern const Parameters global_parameters;

	class Env
	{
	public:
		/* instructions */
		Code function_set;
		Parameters parameters;

		// data record pointer
		int data_record_index;

		void push_code_to_exec_stack(const Code &code)
		{
//			guard.push_back(code);
			exec_stack.push_back(Exec(code));
		}

		/* Stacks */
		std::vector<Exec>	exec_stack;
		std::vector<int>	int_stack;
		std::vector<Code>	code_stack;
		std::vector<bool>   bool_stack;
		std::vector<double>	double_stack;

//		Env(unsigned _reserve = 1000) : function_set(instructions), parameters(global_parameters)
		Env(unsigned _reserve = 1000) : parameters(global_parameters)
		{
			reserve(_reserve);
			clear_stacks();
			data_record_index = 0;
		}

		virtual ~Env() 
		{ 
			clear_stacks(); 
		} // virtual ~Env(){ delete next_env; }  // EG: Added clear();

		//virtual Env* clone() const
		//{
		//	Env* newenv = new Env(*this);
		//	return newenv;
		//}

		virtual void initialize(unsigned _reserve = 1000) 
		{
			function_set = Code(instructions);
			reserve(_reserve);
			clear_stacks();
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

		int go(long n = 50);
	};

	extern std::string print(const Env &env);
	inline std::ostream &operator<<(std::ostream &os, const Env &env)
	{
		os << print(env);
		return os;
	}

	/* Operations */
	template <typename T> inline std::vector<T> &get_stack() { }
	template <> inline std::vector<Exec>   &get_stack()
	{
		return env.exec_stack;
	}
	template <> inline std::vector<int> &get_stack()
	{
		return env.int_stack;
	}
	template <> inline std::vector<double> &get_stack()
	{
		return env.double_stack;
	}
	template <> inline std::vector<bool>   &get_stack()
	{
		return env.bool_stack;
	}
	template <> inline std::vector<Code>   &get_stack()
	{
		return env.code_stack;
	}

	template <typename T> inline bool is_empty()
	{
		return (get_stack<T>().empty());
	}

	/* pushing and popping */
	template <typename T> inline void push(T value)
	{
		get_stack<T>().push_back(value);
	}
	template <typename T> inline T pop(Env &env)
	{
		T val = get_stack<T>().back();
		get_stack<T>().pop_back();
		return val;
	}

	template <typename T> inline T &top()
	{
		return get_stack<T>().back();
	}
	template <class T> inline    T &first()
	{
		return top<T>();
	}
	template <class T> inline    T &second()
	{
		return get_stack<T>()[get_stack<T>().size() - 2];
	}

	/* Push calling convention */
	inline void push_call(Code code)
	{
		env.push_code_to_exec_stack(code);
		push(code);
	}

	template <typename T>
	inline bool has_elements(unsigned sz)
	{
		if (get_stack<T>().size() < sz)
		{
//			env.error();
			return false;
		}

		return true;
	}

	/* generic functions */
	template <typename T>
	inline unsigned dup()
	{
		//if (not has_elements<T>(env, 1)) return 1;
		push<T>(get_stack<T>().back());
		return 1;
	}

	template <typename T>
	inline unsigned swap()
	{
		//if (not has_elements<T>(env,2)) return 1;
		std::vector<T> &stack = get_stack<T>();
		T tmp = stack.back();
		stack.back() = stack[stack.size() - 2];
		stack[stack.size() - 2] = tmp;
		return 1;
	}

	template <typename T>
	inline unsigned equals()
	{
		push(pop<T>(env) == pop<T>(env));
		return 1;
	}

	template <>
	inline unsigned equals<Code>()
	{
		int effort = 0;

		push(equal_to(pop<Code>(env), pop<Code>(env), effort));
		return effort;
	}

	template <>
	inline unsigned equals<Exec>()
	{
		int effort = 0;

		push(equal_to(pop<Exec>(env).to_CodeBase(), pop<Exec>(env).to_CodeBase(), effort));
		return effort;
	}

	//template <class T> inline bool safe_stack(Env& env) { return get_stack<T>(env).size() > 0; }
	//template <> inline bool safe_stack<int>(Env& env) { return get_stack<int>(env).size() > 1; } // as we need to pop from this

	template <class T>
	inline int safe_index()
	{
		//if (not safe_stack<T>(env)) return -1;
		//if (get_stack<int>(env).size() == 0) return -1;
		int index = pop<int>(env);
		int stacksize = get_stack<T>().size();
		index = (index < 0) ? 0 : (index >= stacksize ? stacksize - 1 : index);
		return stacksize - 1 - index;
	}

	template <typename T>
	inline unsigned yankdup()
	{
		int index = safe_index<T>();
		//if (index == -1) return 1; // -1 signals cannot obtain index for whatever reason
		std::vector<T> &stack = get_stack<T>();
		stack.push_back(stack[index]);
		return 1;
	}

	template <class T>
	inline unsigned protected_pop()
	{
		pop<T>(env);
		return 1;
	}

	template <class T>
	inline unsigned stackdepth()
	{
		push<int>(get_stack<T>().size());
		return 1;
	}

	template <class T>
	inline unsigned shove()
	{
		int index = safe_index<T>();
		//if (index == -1) return 1;
		std::vector<T> &stack = get_stack<T>();
		stack.insert(stack.begin() + index, pop<T>(env));
		return stack.size() - index + 1;
	}

	template <class T>
	inline unsigned yank()
	{
		int index = safe_index<T>();
		//if (index == -1) return 1;
		std::vector<T> &stack = get_stack<T>();
//		assert(index < (int)stack.size());
//		assert(index >= 0);
		T value = stack[index];
		stack.erase(stack.begin() + index);
		stack.push_back(value);
		return stack.size() - index + 1;
	}


	template <class T>
	unsigned rot()
	{
		//if (!has_elements<T>(env,3)) return 1;
		T x = pop<T>(env);
		T y = pop<T>(env);
		T z = pop<T>(env);
		push<T>(y);
		push<T>(x);
		push<T>(z);
		return 1;
	}






}