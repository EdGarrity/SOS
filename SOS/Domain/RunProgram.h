#pragma once

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
		//Utilities::Task operator()(Utilities::Threadpool& pool, develop_strategy::RunProgram_WorkOrder_Form const&) const;

//		Utilities::Task run(Utilities::Threadpool& pool, develop_strategy::RunProgram_WorkOrder_Form const&) const;

		RunProgram(Utilities::Threadpool& pool) : m_pool(pool) {};
		Utilities::Task run(develop_strategy::RunProgram_WorkOrder_Form const&) const;
		void wait_for_all_threads_to_complete();
	};
}