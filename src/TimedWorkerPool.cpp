#include "TimedWorkerPool.hpp"
#include <iostream>

TimedWorkerPool::TimedWorkerPool(size_t initial_workers, std::chrono::milliseconds idle_timeout)
    : stopping(false), timeout(idle_timeout)
{
    // TODO: throw std::invalid_argument if initial_workers == 0
    // TODO: spawn initial_workers threads that run worker_loop()
}

TimedWorkerPool::~TimedWorkerPool()
{
    // TODO: Ensure shutdown() is called to join all threads safely
}

void TimedWorkerPool::submit(std::function<void()> task)
{
    // TODO:
    // 1. Lock mutex
    // 2. If stopping == true, throw std::runtime_error("Pool is shutting down")
    // 3. Push task into queue
    // 4. Notify one waiting worker
}

void TimedWorkerPool::shutdown()
{
    // TODO:
    // 1. Acquire lock, set stopping = true
    // 2. Notify all workers
    // 3. Join all threads
}

void TimedWorkerPool::worker_loop(size_t id)
{
    // TODO:
    // Each worker repeatedly waits for a task:
    // - Wait with cv.wait_for(lock, timeout, predicate)
    // - If timeout occurs and queue empty, exit gracefully
    // - If stopping == true and queue empty, exit
    // - Otherwise, pop a task and execute it
    // - Catch and ignore any exceptions from task execution
}
