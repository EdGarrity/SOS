#include "..\Utilities\Task.hpp"
#include "..\Utilities\Threadpool.hpp"

#include "Develop Strategy/RunStratergy.WorkOrder.h"

namespace domain
{
	class RunProgram
	{
	private:

	public:
		Utilities::task operator()(Utilities::threadpool& pool, develop_strategy::RunStratergyWorkOrder const&) const;
	};
}