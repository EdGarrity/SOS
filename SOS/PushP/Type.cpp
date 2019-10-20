#include <iostream>
#include "Type.h"
#include "Env.h"

namespace Push
{
	// This needs to be initialize in Push Initialze and stored in Thread Local Storage
//	class Env;

	class Env_detail;
	typedef combinable<Push::Env_detail> Env;

	std::ostream &operator<<(std::ostream &os, const Type &tp)
	{
		if (tp.get().size() == 0) os << 0 << ' ';

		for (unsigned i = 0; i < tp.get().size(); ++i)
			os << tp.get()[i] << ' ';

		return os;
	}

	bool Type::can_pop_from(Env & _env) const
	{
		for (unsigned i = start_; i < type.size(); ++i)
		{
			if ((type[i] > 0) && (_env.local().get_stack_size(i) < static_cast<unsigned>(type[i])))
			{
				//std::cerr << "Fail on " << i << ' ' << env.get_stack_size(i) << std::endl;
				return false;
			}
		}

		return true;
	}

	bool Type::operator==(const Type &other) const
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