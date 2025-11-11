#include "async_job_queue.hpp"
#include <iostream>

AsyncJobQueue::AsyncJobQueue()
{
    // TODO: start worker thread that runs worker_loop()
}

AsyncJobQueue::~AsyncJobQueue()
{
    // TODO: stop the worker safely (use stop() and join the thread)
}

AsyncJobQueue::AsyncJobQueue(AsyncJobQueue &&other) noexcept
{
    // TODO: move ownership of thread and queue safely from other
}

AsyncJobQueue &AsyncJobQueue::operator=(AsyncJobQueue &&other) noexcept
{
    if (this != &other)
    {
        // TODO: stop current worker, then move resources from other
    }
    return *this;
}

void AsyncJobQueue::enqueue(std::function<void()> job)
{
    // TODO:
    // 1. lock mutex
    // 2. push job into queue
    // 3. notify worker thread
}

void AsyncJobQueue::stop()
{
    // TODO:
    // 1. set running_ = false
    // 2. notify the worker thread
}

void AsyncJobQueue::worker_loop()
{
    try
    {
        while (true)
        {
            std::function<void()> job;
            {
                std::unique_lock<std::mutex> lock(mtx_);
                cv_.wait(lock, [&]
                         { return !jobs_.empty() || !running_; });
                if (!running_ && jobs_.empty())
                    break;

                job = std::move(jobs_.front());
                jobs_.pop();
            }

            try
            {
                // TODO: execute the job safely
                job();
            }
            catch (const std::exception &e)
            {
                std::cerr << "[AsyncJobQueue] Job threw exception: " << e.what() << std::endl;
            }
            catch (...)
            {
                std::cerr << "[AsyncJobQueue] Unknown exception in job.\n";
            }
        }
    }
    catch (...)
    {
        std::cerr << "[AsyncJobQueue] Worker crashed unexpectedly.\n";
    }
}
