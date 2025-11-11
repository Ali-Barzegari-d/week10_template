#include "TimedWorkerPool.hpp"
#include <iostream>

TimedWorkerPool::TimedWorkerPool(size_t initial_workers, std::chrono::milliseconds idle_timeout)
    : stopping(false), timeout(idle_timeout)
{
    if (initial_workers == 0)
    {
        throw std::invalid_argument("Worker count must be > 0");
    }

    workers.reserve(initial_workers);
    for (size_t i = 0; i < initial_workers; ++i)
    {
        workers.emplace_back(&TimedWorkerPool::worker_loop, this, i);
    }
}

TimedWorkerPool::~TimedWorkerPool()
{
    shutdown();
}

void TimedWorkerPool::submit(std::function<void()> task)
{
    {
        std::unique_lock<std::mutex> lock(mtx);
        if (stopping.load())
        {
            throw std::runtime_error("Pool is shutting down");
        }
        tasks.push(std::move(task));
    }
    cv.notify_one();
}

void TimedWorkerPool::shutdown()
{
    {
        std::unique_lock<std::mutex> lock(mtx);
        stopping.store(true);
    }
    cv.notify_all();

    for (auto &t : workers)
    {
        if (t.joinable())
            t.join();
    }

    workers.clear();
}

void TimedWorkerPool::worker_loop(size_t id)
{
    while (true)
    {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(mtx);

            bool has_task = cv.wait_for(lock, timeout, [this]()
                                        { return stopping.load() || !tasks.empty(); });

            if (!has_task && tasks.empty())
            {
                // Timed out waiting — worker exits gracefully
                std::cout << "[Worker " << id << "] idle timeout, exiting.\n";
                return;
            }

            if (stopping.load() && tasks.empty())
            {
                return;
            }

            if (!tasks.empty())
            {
                task = std::move(tasks.front());
                tasks.pop();
            }
        }

        try
        {
            if (task)
                task();
        }
        catch (const std::exception &e)
        {
            std::cerr << "[Worker " << id << "] Task exception: " << e.what() << "\n";
        }
        catch (...)
        {
            std::cerr << "[Worker " << id << "] Unknown task error.\n";
        }
    }
}
