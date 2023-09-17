#include "..\Utilities\Task.hpp"
#include "..\Utilities\Threadpool.hpp"

#include "Develop Strategy/RunProgram_WorkOrder_Form.h"

namespace domain
{
	class RunProgram
	{
	private:

	public:
		Utilities::Task operator()(Utilities::Threadpool& pool, develop_strategy::RunProgram_WorkOrder_Form const&) const;
	};
}