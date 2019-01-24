function_pool.h

#pragma once
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <cassert>

class Function_pool
{

private:
    std::queue<std::function<void()>> m_function_queue;
    std::mutex m_lock;
    std::condition_variable m_data_condition;
    std::atomic<bool> m_accept_functions;

public:

    Function_pool();
    ~Function_pool();
    void push(std::function<void()> func);
    void done();
    void infinite_loop_func();
};
function_pool.cpp

#include "function_pool.h"

Function_pool::Function_pool() : m_function_queue(), m_lock(), m_data_condition(), m_accept_functions(true)
{
}

Function_pool::~Function_pool()
{
}

void Function_pool::push(std::function<void()> func)
{
    std::unique_lock<std::mutex> lock(m_lock);
    m_function_queue.push(func);
    // when we send the notification immediately, the consumer will try to get the lock , so unlock asap
    lock.unlock();
    m_data_condition.notify_one();
}

void Function_pool::done()
{
    std::unique_lock<std::mutex> lock(m_lock);
    m_accept_functions = false;
    lock.unlock();
    // when we send the notification immediately, the consumer will try to get the lock , so unlock asap
    m_data_condition.notify_all();
    //notify all waiting threads.
}

void Function_pool::infinite_loop_func()
{
    std::function<void()> func;
    while (true)
    {
        {
            std::unique_lock<std::mutex> lock(m_lock);
            m_data_condition.wait(lock, [this]() {return !m_function_queue.empty() || !m_accept_functions; });
            if (!m_accept_functions && m_function_queue.empty())
            {
                //lock will be release automatically.
                //finish the thread loop and let it join in the main thread.
                return;
            }
            func = m_function_queue.front();
            m_function_queue.pop();
            //release the lock
        }
        func();
    }
}
main.cpp

#include "function_pool.h"
#include <string>
#include <iostream>
#include <mutex>
#include <functional>
#include <thread>
#include <vector>

Function_pool func_pool;

class quit_worker_exception : public std::exception {};

void example_function()
{
    std::cout << "bla" << std::endl;
}

int main()
{
    std::cout << "stating operation" << std::endl;
    int num_threads = std::thread::hardware_concurrency();
    std::cout << "number of threads = " << num_threads << std::endl;
    std::vector<std::thread> thread_pool;
    for (int i = 0; i < num_threads; i++)
    {
        thread_pool.push_back(std::thread(&Function_pool::infinite_loop_func, &func_pool));
    }

    //here we should send our functions
    for (int i = 0; i < 50; i++)
    {
        func_pool.push(example_function);
    }
    func_pool.done();
    for (unsigned int i = 0; i < thread_pool.size(); i++)
    {
        thread_pool.at(i).join();
    }
}