#ifndef TIMED_WORKER_POOL_HPP
#define TIMED_WORKER_POOL_HPP

#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <functional>
#include <chrono>
#include <vector>
#include <atomic>
#include <stdexcept>

// A worker pool where threads exit after being idle for a timeout period.
class TimedWorkerPool
{
public:
    TimedWorkerPool(size_t initial_workers, std::chrono::milliseconds idle_timeout);
    ~TimedWorkerPool();

    // Add a new task to the queue (throws if shutting down)
    void submit(std::function<void()> task);

    // Gracefully stop all workers
    void shutdown();

private:
    // Main loop for each worker
    void worker_loop(size_t id);

    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex mtx;
    std::condition_variable cv;

    std::atomic<bool> stopping;
    std::chrono::milliseconds timeout;
};

#endif
