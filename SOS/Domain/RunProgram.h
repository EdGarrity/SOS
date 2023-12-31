#pragma once

#include <latch>

#include "..\Utilities\Task.hpp"
#include "..\Utilities\Threadpool.hpp"

#include "Develop Strategy/RunProgram_WorkOrder_Form.h"

namespace domain
{
	class RunProgram
	{
	private:
		Utilities::Threadpool& m_pool;

	public:
		RunProgram(Utilities::Threadpool& pool) : m_pool(pool) {};
		Utilities::Task run(develop_strategy::RunProgram_WorkOrder_Form const workorder_form, std::latch &work_done) const;
		void wait_for_all_threads_to_complete();
	};
}