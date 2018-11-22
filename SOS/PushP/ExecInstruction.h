#pragma once

#include "Instruction.h"
#include "Env.h"
#include "CodeOps.h"

namespace Push
{
	class DoRangeClass : public CodeList
	{
	public:
		DoRangeClass(const CodeArray & vec) : CodeList(vec)
		{
			//			assert(vec.size() == 4);
		}

		unsigned operator()() const
		{
			CodeArray vec = get_stack();
			int i = static_cast<Literal<int>*>(vec[3].get())->get();
			int n = static_cast<Literal<int>*>(vec[2].get())->get();
			int direction = 1;

			if (i > n) direction = -1;

			push(i);
			Exec code = Exec(vec[0]);

			if (i != n)
			{
				vec[3] = Code(intLiteralFactory->createLiteral(i + direction));
				Code ranger = Code(codeListFactory->createCodeList(vec));  // new CodeList(vec));  //CodeList::adopt(vec);
				env.push_code_to_exec_stack(ranger);
			}

			push(code);
			return 1;
		}
	};

	//
	// DoRangeClass memory manager
	//
	class DoRangeClassRegisterNode
	{
		DoRangeClass* _p;
		DoRangeClassRegisterNode* _next;

		friend class DoRangeClassRegister;

	public:
		DoRangeClassRegisterNode(DoRangeClass *p_, DoRangeClassRegisterNode* next_)
		{
			_p = p_;
			_next = next_;
		}
	};

	class DoRangeClassRegister
	{
		DoRangeClassRegisterNode* _head;

	public:
		DoRangeClassRegister() : _head(nullptr) {}

		~DoRangeClassRegister()
		{
			clean_up();
		}

		void record(DoRangeClass* p)
		{
			DoRangeClassRegisterNode* node = new DoRangeClassRegisterNode(p, _head);
			_head = node;
		}

		void clean_up()
		{
			DoRangeClassRegisterNode* current_node = _head;

			while (current_node != nullptr)
			{
				DoRangeClassRegisterNode* next_node = current_node->_next;

				delete current_node->_p;
				current_node->_p = nullptr;

				delete current_node;
				current_node = nullptr;

				current_node = next_node;
			}

			_head = nullptr;
		}
	};

	class DoRangeClassFactory
	{
		DoRangeClassRegister doRangeClassRegister;

	public:
		DoRangeClass* createDoRangeClass(const CodeArray &stack);

		void clean_up()
		{
			doRangeClassRegister.clean_up();
		}
	};

	inline DoRangeClass* DoRangeClassFactory::createDoRangeClass(const CodeArray &stack)
	{
		DoRangeClass* lp = new DoRangeClass(stack);
		doRangeClassRegister.record(lp);
		return lp;
	}

	extern thread_local DoRangeClassFactory *doRangeClassFactory;
}
