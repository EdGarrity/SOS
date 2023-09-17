#pragma once

#include <coroutine>
#include <iostream>
#include <utility>

namespace Utilities
{
    struct task_promise;

    class [[nodiscard]] Task
    {
    public:
        using promise_type = task_promise;

        //explicit task(std::coroutine_handle<task_promise> handle)
        //    : m_handle(handle)
        //{
        //}

        explicit Task(std::coroutine_handle<task_promise> handle)
            : m_handle(handle)
        {
        }

        Task() {};

    private:
        std::coroutine_handle<task_promise> m_handle;
    };

    struct task_promise
    {
        Task get_return_object() noexcept {
            return Task{ std::coroutine_handle<task_promise>::from_promise(*this) };
        };

        std::suspend_never initial_suspend() const noexcept { return {}; }
        std::suspend_never final_suspend() const noexcept { return {}; }

        void return_void() noexcept {}

        void unhandled_exception() noexcept {
            std::cerr << "Unhandled exception caught...\n";
            exit(1);
        }
    };
}