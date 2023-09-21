#pragma once

#include <iostream>
#include <condition_variable>
#include <coroutine>
#include <cstdint>
#include <list>
#include <mutex>
#include <queue>
#include <thread>

namespace Utilities
{
    class Threadpool
    {
    public:
        explicit Threadpool(const std::size_t threadCount)
        {
            //std::cout << "threadpool(" << threadCount << ") from thread : " << std::this_thread::get_id() << "\n";
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

        void wait_for_all_threads_to_complete()
        {
            while (m_coros.size() != 0)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            for (std::thread& thread : m_threads)
            {
                if (thread.joinable())
                {
                    thread.join();
                }
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
            while (!m_stop_thread)
            {
                std::unique_lock<std::mutex> lock(m_mutex);

                while (!m_stop_thread && m_coros.size() == 0)
                {
                    m_cond.wait_for(lock, std::chrono::microseconds(100));
                }

                if (m_stop_thread)
                {
                    break;
                }

                auto coro = m_coros.front();
                m_coros.pop();
                lock.unlock();
                coro.resume();
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