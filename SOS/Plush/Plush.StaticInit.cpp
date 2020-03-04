#include "Plush.StaticInit.h"

namespace Plush
{
	String2CodeMapType String2CodeMap;
	StaticInit static_initializer;

	void initGenerics();

	StaticInit::StaticInit()
	{
		initGenerics();
	};

	void StaticInit::register_pushfunc(Operator op, std::string type, std::string name)
	{
		std::string func_name = type + "." + name;
		String2CodeMap[func_name] = op;
	};
}