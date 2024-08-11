#pragma once

#include <map>
#include <mutex>
#include <thread>
#include "..\PushGP\Globals.h"
#include "Environment.h"

namespace Plush
{
	class EnvironmentPool
	{
	private:
		Plush::Environment m_env_pool[domain::argmap::max_threads];
		std::map<std::thread::id, size_t> m_pool;  //std::this_thread::get_id()
		size_t m_env_count = 0;
		size_t m_max_env_count = 8;
		std::mutex m_env_mutex;

		// Purpose: 
		//   Register a new thread with the ppol
		//
		// Parameters:
		//   None
		//
		// Return value:
		//   None
		//
		// Side Effects:
		//   If the calling thread is not already registered, it is added to the pool.
		//
		// Thread Safe:
		//   Yes
		//
		// Remarks:
		//   This method is called by the threadpool when a new thread is created.  The calling 
		//	 thread is assigned the next available environment and added to the pool.  The 
		//	 m_env_count is incremented in a thread safe manner.
		void register_thread();

	public:
		EnvironmentPool();
		// Purpose: 
		//   Constructs an environment pool with the size specified
		//
		// Parameters:
		//   None
		//
		// Return value:
		//   None
		//
		// Side Effects:
		//   Creates a pool of environments
		//
		// Thread Safe:
		//   No
		//
		// Remarks:
		//   Creates a pool of environments to be used by the threadpool.  The pool is created with the size specified.
		//EnvironmentPool();

		// Purpose: 
		//   Get reference to the envirent for the calling thread
		//
		// Parameters:
		//   None
		//
		// Return value:
		//   Reference to an environment from the pool
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes
		//
		// Remarks:
		//   This method by a function to get a reference to the environment assigned to the caller's thread.
		Plush::Environment& get_env();
	};
	
	extern EnvironmentPool environment_pool;
}
