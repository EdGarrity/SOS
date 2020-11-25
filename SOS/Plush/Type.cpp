#define NOMINMAX
#include <algorithm>
#include "Type.h"
#include "Environment.h"

namespace Plush
{
	bool Type::can_pop_from(const Environment & env) const
	{
		for (unsigned i = _start; i < type.size(); ++i)
		{
			unsigned int s = env.get_stack_size(i);

			if ((type[i] > 0) && (s < static_cast<unsigned>(type[i])))
				return false;
		}

		return true;
	}

	bool Type::operator==(const Type & other) const
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

	void Type::abs()
	{
		for (unsigned i = 0; i < type.size(); ++i)
			type[i] = std::abs(type[i]);
	}
}