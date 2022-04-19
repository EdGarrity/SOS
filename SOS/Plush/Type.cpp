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
		//for (unsigned int i = _start; i < type.size(); ++i)
		//{
		//	unsigned int s = env.get_stack_size(i);

		//	if ((type[i] > 0) && (s < type[i]))
		//		return false;
		//}

		for (unsigned int i = _start; i < type.size(); ++i)
		{
			if (type[i] > 0)
			{
				//unsigned int s = env.check_stack_size_at_least(i, type[i]);

				//if (s < type[i])
				//	return false;

				if (env.check_stack_size_at_least(i, type[i]) == false)
					return false;
			}
		}

		return true;
	}

	bool Type::can_push_to(/*const*/ Environment & env) // const
	{
		//for (unsigned int i = _start; i < type.size(); ++i)
		//{
		//	size_t f = env.get_stack_free(i);

		//	if ((type[i] > 0) && (f < type[i]))
		//		return false;
		//}

		for (unsigned int i = _start; i < type.size(); ++i)
		{
			if (type[i] > 0)
			{
				size_t f = env.get_stack_free(i);

				if (f < type[i])
					return false;
			}
		}

		return true;
	}

	bool Type::operator==(/*const*/ Type & other) // const
	{
		unsigned long mins = (unsigned long)std::min(type.size(), other.type.size());

		for (unsigned long i = 0; i < mins; ++i)
		{
			if (type[i] != other.type[i]) return false;
		}

		for (unsigned long i = mins; i < type.size(); ++i)
		{
			if (type[i] != 0) return false;
		}

		for (unsigned long i = mins; i < other.type.size(); ++i)
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