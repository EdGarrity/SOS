#include "Debug.h"

namespace Utilities
{
	std::mutex work_order_print_;
	unsigned long line_number = 0;

#if TRACE_LEVEL>0
	std::queue<std::string> trace_queue;
	int generation = 0;
#endif

}