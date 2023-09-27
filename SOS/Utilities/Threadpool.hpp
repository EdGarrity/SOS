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

namespace Utilities
{
    class Threadpool
    {
    public:
        explicit Threadpool(const std::size_t threadCount)
        {
            {
                std::ostringstream ss;
                ss << ",method=Threadpool.Threadpool()"
                    << ",threadCount=" << threadCount
                    << ",message=Constructor";
                Utilities::logline_threadsafe << ss.str();
            }

            for (std::size_t i = 0; i < threadCount; ++i)
            {
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
                {
                    std::ostringstream ss;
                    ss << ",method=Threadpool.wait_for_all_threads_to_complete"
                        << ",work_queue_size=" << m_coros.size()
                        << ",message=Waiting_for_work_queue_to_be_empty";
                    Utilities::logline_threadsafe << ss.str();
                }

                std::this_thread::sleep_for(std::chrono::seconds(10));
            }

            {
                std::ostringstream ss;
                ss << ",method=Threadpool.wait_for_all_threads_to_complete"
                    << ",work_queue_size=" << m_coros.size()
                    << ",message=Work_queue_empty";
                Utilities::logline_threadsafe << ss.str();
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            {
                std::ostringstream ss;
                ss << ",method=Threadpool.wait_for_all_threads_to_complete"
                    << ",thread_count=" << m_threads.size()
                    << ",message=Waiting_for_all_threads_to_complete";
                Utilities::logline_threadsafe << ss.str();
            }

            //for (std::thread& thread : m_threads)
            //{
            //    if (thread.joinable())
            //    {
            //        {
            //            std::ostringstream ss;
            //            ss << ",method=Threadpool.wait_for_all_threads_to_complete"
            //                << ",message=Joining_thread";
            //            Utilities::logline_threadsafe << ss.str();
            //        }

            //        thread.join();

            //        {
            //            std::ostringstream ss;
            //            ss << ",method=Threadpool.wait_for_all_threads_to_complete"
            //                << ",message=Thread_done";
            //            Utilities::logline_threadsafe << ss.str();
            //        }
            //    }
            //}

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            work_done.wait();

            {
                std::ostringstream ss;
                ss << ",method=Threadpool.wait_for_all_threads_to_complete"
                    << ",message=All_work_done";
                Utilities::logline_threadsafe << ss.str();
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
            {
                std::ostringstream ss;
                ss << ",method=Threadpool.thread_loop()"
                    << ",message=Started";
                Utilities::logline_threadsafe << ss.str();
            }
            
            while (!m_stop_thread)
            {
                {
                    std::ostringstream ss;
                    ss << ",method=Threadpool.thread_loop()"
                        << "m_coros.size()=" << m_coros.size()
                        << ",message=Entered_loop";
                    Utilities::logline_threadsafe << ss.str();
                }

                std::unique_lock<std::mutex> lock(m_mutex);

                while (!m_stop_thread && m_coros.size() == 0)
                {
                    m_cond.wait_for(lock, std::chrono::microseconds(100));
                }

                if (m_stop_thread)
                {
                    break;
                }

                {
                    std::ostringstream ss;
                    ss << ",method=Threadpool.thread_loop()"
                        << "m_coros.size()=" << m_coros.size()
                        << ",message=Found_work";
                    Utilities::logline_threadsafe << ss.str();
                }

                auto coro = m_coros.front();
                m_coros.pop();
                lock.unlock();

                {
                    std::ostringstream ss;
                    ss << ",method=Threadpool.thread_loop"
                        << "m_coros.size()=" << m_coros.size()
                        << ",message=coro.resuming";
                    Utilities::logline_threadsafe << ss.str();
                }

                coro.resume();

                {
                    std::ostringstream ss;
                    ss << ",method=Threadpool.thread_loop"
                        << "m_coros.size()=" << m_coros.size()
                        << ",message=coro.resumed";
                    Utilities::logline_threadsafe << ss.str();
                }
            }
        }

        void enqueue_task(std::coroutine_handle<> coro) noexcept 
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_coros.emplace(coro);
            m_cond.notify_one();
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
    };
}