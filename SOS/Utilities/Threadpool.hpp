#pragma once

#include <iostream>
#include <condition_variable>
#include <coroutine>
#include <cstdint>
#include <latch>
#include <list>
#include <mutex>
#include <queue>
#include <thread>
#include <sstream>

#include "..\Utilities\Debug.h"
#include "..\Plush\EnvironmentPool.h"

namespace Utilities
{
    extern 	Plush::EnvironmentPool env_pool_1;

    class Threadpool
    {
    public:
        explicit Threadpool(const std::size_t threadCount)
        {
            if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
            {
                std::ostringstream ss;
                ss << ",method=Threadpool.Threadpool"
                    << ",diagnostic_level=9"
                    << ",threadCount=" << threadCount
                    << ",message=Constructor";
                Utilities::logline_threadsafe << ss.str();
            }

            for (std::size_t i = 0; i < threadCount; ++i)
            {
                if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
                {
                    std::ostringstream ss;
                    ss << ",method=Threadpool.Threadpool"
                        << ",diagnostic_level=9"
                        << ",i=" << i
                        << ",message=Creating_Thread";
                    Utilities::logline_threadsafe << ss.str();
                }

                std::thread worker_thread([this]() {
                    this->thread_loop();
                    });
                m_threads.push_back(std::move(worker_thread));
            }
        }

        ~Threadpool()
        {
            shutdown();
        }

        auto schedule()
        {
            struct awaiter
            {
                Threadpool* m_threadpool;

                constexpr bool await_ready() const noexcept { return false; }
                constexpr void await_resume() const noexcept { }
                void await_suspend(std::coroutine_handle<> coro) const noexcept {
                    m_threadpool->enqueue_task(coro);
                }
            };
            return awaiter{ this };
        }

        void wait_for_all_threads_to_complete(std::latch const& work_done)
        {
            while (m_coros.size() != 0)
            {
                if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
                {
                    std::ostringstream ss;
                    ss << ",method=Threadpool.wait_for_all_threads_to_complete"
                        << ",diagnostic_level=9"
                        << ",work_queue_size=" << m_coros.size()
                        << ",message=Waiting_for_work_queue_to_be_empty";
                    Utilities::logline_threadsafe << ss.str();
                }

                std::this_thread::sleep_for(std::chrono::seconds(10));
            }

            if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
            {
                std::ostringstream ss;
                ss << ",method=Threadpool.wait_for_all_threads_to_complete"
                    << ",diagnostic_level=9"
                    << ",work_queue_size=" << m_coros.size()
                    << ",message=Work_queue_empty";
                Utilities::logline_threadsafe << ss.str();
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
            {
                std::ostringstream ss;
                ss << ",method=Threadpool.wait_for_all_threads_to_complete"
                    << ",diagnostic_level=9"
                    << ",thread_count=" << m_threads.size()
                    << ",message=Waiting_for_all_threads_to_complete";
                Utilities::logline_threadsafe << ss.str();
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            work_done.wait();

            if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
            {
                std::ostringstream ss;
                ss << ",method=Threadpool.wait_for_all_threads_to_complete"
                    << ",diagnostic_level=9"
                    << ",message=All_work_done";
                Utilities::logline_threadsafe << ss.str();
            }
        }

        void shutdown()
        {
            m_stop_thread = true;
            while (m_threads.size() > 0)
            {
                std::thread& thread = m_threads.back();
                if (thread.joinable())
                {
                    thread.join();
                }
                m_threads.pop_back();
            }
        }

    private:
        std::list<std::thread> m_threads;

        std::mutex m_mutex;
        std::condition_variable m_cond;
        std::queue<std::coroutine_handle<>> m_coros;

        bool m_stop_thread = false;

        void thread_loop()
        {
            if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
            {
                std::ostringstream ss;
                ss << ",method=Threadpool.thread_loop"
                    << ",diagnostic_level=9"
                    << ",message=Started";
                Utilities::logline_threadsafe << ss.str();
            }

            while (!m_stop_thread)
            {
                if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
                {
                    std::ostringstream ss;
                    ss << ",method=Threadpool.thread_loop"
                        << ",diagnostic_level=9"
                        << ",m_coros.size=" << m_coros.size()
                        << ",message=Entered_lock";
                    Utilities::logline_threadsafe << ss.str();
                }

                std::unique_lock<std::mutex> lock(m_mutex);

                if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
                {
                    std::ostringstream ss;
                    ss << ",method=Threadpool.thread_loop"
                        << ",diagnostic_level=9"
                        << ",m_coros.size=" << m_coros.size()
                        << ",message=Entered_loop";
                    Utilities::logline_threadsafe << ss.str();
                }

                while (!m_stop_thread && m_coros.size() == 0)
                {
                    m_cond.wait_for(lock, std::chrono::microseconds(100));
                }

                if (m_stop_thread)
                {
                    break;
                }

                if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
                {
                    std::ostringstream ss;
                    ss << ",method=Threadpool.thread_loop"
                        << ",diagnostic_level=9"
                        << ",m_coros.size=" << m_coros.size()
                        << ",message=Found_work";
                    Utilities::logline_threadsafe << ss.str();
                }

                auto coro = m_coros.front();
                m_coros.pop();
                lock.unlock();

                if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
                {
                    std::ostringstream ss;
                    ss << ",method=Threadpool.thread_loop"
                        << ",diagnostic_level=9"
                        << ",m_coros.size=" << m_coros.size()
                        << ",message=coro.resuming";
                    Utilities::logline_threadsafe << ss.str();
                }

                coro.resume();

                if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
                {
                    std::ostringstream ss;
                    ss << ",method=Threadpool.thread_loop"
                        << ",diagnostic_level=9"
                        << ",m_coros.size=" << m_coros.size()
                        << ",message=coro.resumed";
                    Utilities::logline_threadsafe << ss.str();
                }
            }

            if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
            {
                std::ostringstream ss;
                ss << ",method=Threadpool.thread_loop"
                    << ",diagnostic_level=9"
                    << ",message=Done";
                Utilities::logline_threadsafe << ss.str();
            }
        }

        void enqueue_task(std::coroutine_handle<> coro) noexcept 
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_coros.emplace(coro);
            m_cond.notify_one();
        }
    };
}