#include "ThreadSafeArray.h"

namespace Utilities
{
	std::mutex array_access_;
}