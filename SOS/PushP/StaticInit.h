#pragma once

#include "Code.h"

namespace push
{
	int init_push();
	
	namespace detail
	{
		extern Code* nil_ptr;
		extern Code* instructions_ptr;
		extern String2CodeMap* str2code_map_ptr;
		extern String2ParenthesesMap* str2parentheses_map_ptr;
		extern CodeSet* erc_set_ptr;
		extern CodeSet* exec_set_ptr;
//		extern std::vector<ParseHook>* parse_extension_hooks_ptr;

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
					nil_ptr = new Code(new CodeList(CodeArray()));
					instructions_ptr = new Code(*nil_ptr);
					str2code_map_ptr = new String2CodeMap();
					str2parentheses_map_ptr = new String2ParenthesesMap();
					erc_set_ptr = new CodeSet();
//					exec_set_ptr = new CodeSet;
//					parse_extension_hooks_ptr = new std::vector<ParseHook>;
					//                    default_root_env_ptr = 0;
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
					delete erc_set_ptr;
//					delete exec_set_ptr;
//					delete parse_extension_hooks_ptr;
				}
			}

			/* Registers a function and makes it globally available through the 'instructions' Code */
			Code register_pushfunc(Code code);

			/* Adds a Hook function that can be used to parse extra-pushlike syntax */
//			int register_parse_hook(ParseHook);

			/* Set a root environment as a subclass of Env */
//			int set_root_env(Env* env);

		};



	}
} // namespace detail / push

// the object that does the initialization
static push::detail::StaticInit static_initializer;
