#include <iostream>
#include <thread>

#include "RunProgram.h"

namespace domain
{
    Utilities::Task RunProgram::operator()(Utilities::Threadpool& pool, develop_strategy::RunProgram_WorkOrder_Form const&) const
    {
        // Schedule the program to run in its own thread and wait to be activated.  co_wait() will return control to the caller.
        co_await pool.schedule();

        // Run the program in seperate thread.
    }
}
