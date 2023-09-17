#include <iostream>
#include <thread>

#include "RunProgram.h"

namespace domain
{
    Utilities::task RunProgram::operator()(Utilities::threadpool& pool, develop_strategy::RunStratergyWorkOrder const&) const
    {
        Utilities::co_await pool.schedule();
    }
}
