#include "EnvironmentPool.h"
namespace Plush
{
	void Plush::EnvironmentPool::register_thread()
    {
		std::lock_guard<std::mutex> lock(m_mutex);

		if (m_env_count >= m_max_env_count)
			throw std::runtime_error("EnvironmentPool::register_thread() - Too many threads");

		std::thread::id this_id = std::this_thread::get_id();

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
		std::lock_guard<std::mutex> lock(m_mutex);

		std::thread::id this_id = std::this_thread::get_id();

		if (m_pool.find(this_id) == m_pool.end())
			return m_env_pool[m_pool[this_id]];
		else
			throw std::runtime_error("EnvironmentPool::get_env() - Thread not registered");
	}

	EnvironmentPool environment_pool;
}
