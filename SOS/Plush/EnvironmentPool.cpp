#include "EnvironmentPool.h"

namespace Plush
{
	EnvironmentPool::EnvironmentPool()
	{
		m_env_count = 0;
		m_max_env_count = 8;
	}

	void Plush::EnvironmentPool::register_thread()
    {
		std::thread::id this_id = std::this_thread::get_id();

		//std::unique_lock<std::mutex> lock1(m_env_mutex);

		if (m_env_count >= m_max_env_count)
			throw std::runtime_error("EnvironmentPool::register_thread() - Too many threads");

		if (m_pool.find(this_id) == m_pool.end())
		{
			m_pool[this_id] = m_env_count;
			m_env_count++;
		}
		else
			throw std::runtime_error("EnvironmentPool::register_thread() - Thread already registered");
	}

	Plush::Environment& EnvironmentPool::get_env()
	{
		std::unique_lock<std::mutex> lock1(m_env_mutex);

		std::thread::id this_id = std::this_thread::get_id();

		if (m_pool.find(this_id) == m_pool.end())
			register_thread();

		return m_env_pool[m_pool[this_id]];
	}

	EnvironmentPool environment_pool;
}
