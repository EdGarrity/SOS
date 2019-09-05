#pragma once

#include <functional>
#include "Code.h"

namespace Push
{
	int init_push();
	int init_push(std::function<void(void)> _init_push_application_specific_functions);

	namespace detail
	{
		typedef std::vector<std::string> Names;

		extern Code* nil_ptr;
		extern Code* instructions_ptr;
		extern String2CodeMap* str2code_map_ptr;
		extern String2ParenthesesMap* str2parentheses_map_ptr;
		extern CodeSet* erc_set_ptr;
		extern CodeSet* exec_set_ptr;
		extern Names function_names;

		//        extern Env* default_root_env_ptr;

		extern int init_count;

		extern void init_types();

		class StaticInit
		{
		public:

			StaticInit()
			{
				if (init_count++ == 0)
				{
					nil_ptr = new Code(new CodeList(CodeArray()));  // Potential Memory Leak  Look to free the memory allocated for the CodeList
					instructions_ptr = new Code(*nil_ptr);
					str2code_map_ptr = new String2CodeMap();
					str2parentheses_map_ptr = new String2ParenthesesMap();
					erc_set_ptr = new CodeSet();
					init_types();
				}
			}

			~StaticInit()
			{
				if (--init_count == 0)
				{
					delete nil_ptr;
					delete instructions_ptr;
					delete str2code_map_ptr;
					delete str2parentheses_map_ptr;
					delete erc_set_ptr;
				}
			}

			/* Registers a function and makes it globally available through the 'instructions' Code */
			Code register_pushfunc(Code code);
		};
	}
} // namespace detail / push

// the object that does the initialization
static Push::detail::StaticInit static_initializer;
