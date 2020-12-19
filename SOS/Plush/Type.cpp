#define NOMINMAX
#include <algorithm>
#include "Type.h"
#include "Environment.h"

namespace Plush
{
	Type Type::make_equal_length(const Type &org)
	{
		if (type.size() == org.type.size()) return org;

		size_t len = std::max(type.size(), org.type.size());
		type.resize(len, 0);

		if (org.type.size() != len)
		{
			Type org2 = org;
			org2.type.resize(len, 0);
			return org2;
		}

		return org;
	}

	bool Type::can_pop_from(/*const*/ Environment & env) // const
	{
		for (unsigned int i = _start; i < type.size(); ++i)
		{
			unsigned int s = env.get_stack_size(i);

			if ((type[i] > 0) && (s < type[i]))
				return false;
		}

		return true;
	}

	bool Type::can_push_to(/*const*/ Environment & env) // const
	{
		for (unsigned int i = _start; i < type.size(); ++i)
		{
			size_t f = env.get_stack_free(i);

			if ((type[i] > 0) && (f < type[i]))
				return false;
		}

		return true;
	}

	bool Type::operator==(/*const*/ Type & other) // const
	{
		unsigned mins = std::min(type.size(), other.type.size());

		for (unsigned i = 0; i < mins; ++i)
		{
			if (type[i] != other.type[i]) return false;
		}

		for (unsigned i = mins; i < type.size(); ++i)
		{
			if (type[i] != 0) return false;
		}

		for (unsigned i = mins; i < other.type.size(); ++i)
		{
			if (other.type[i] != 0) return false;
		}

		return true;
	}

	//void Type::abs()
	//{
	//	for (unsigned i = 0; i < type.size(); ++i)
	//		type[i] = std::abs(type[i]);
	//}
}