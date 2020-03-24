#include "Plush.StaticInit.h"

namespace Plush
{
	Func2CodeMapType Func2CodeMap;
	Func2BlockWantsMapType Func2BlockWantsMap;
	StaticInit static_initializer;

	void initGenerics();
	void initBool();
	void initExec();

	StaticInit::StaticInit()
	{
		initGenerics();
		initBool();
		initExec();
	};

	void StaticInit::register_pushfunc(Operator op, std::string type, std::string name, unsigned int block_wants)
	{
		std::string func_name = type + "." + name;
		Func2CodeMap[func_name] = op;
		Func2BlockWantsMap[func_name] = block_wants;
	};
}