#include "async_job_queue.hpp"
#include <iostream>

AsyncJobQueue::AsyncJobQueue()
{
    worker_ = std::thread(&AsyncJobQueue::worker_loop, this);
}

AsyncJobQueue::~AsyncJobQueue()
{
    stop();
    if (worker_.joinable())
        worker_.join();
}

AsyncJobQueue::AsyncJobQueue(AsyncJobQueue &&other) noexcept
{
    // Transfer state
    {
        std::lock_guard<std::mutex> lock(other.mtx_);
        jobs_ = std::move(other.jobs_);
        running_ = other.running_.load();
    }

    // Start a new worker bound to *this* (not other's pointer!)
    if (running_)
    {
        worker_ = std::thread(&AsyncJobQueue::worker_loop, this);
    }

    // Stop the old one safely
    other.stop();
    if (other.worker_.joinable())
        other.worker_.join();
}

AsyncJobQueue &AsyncJobQueue::operator=(AsyncJobQueue &&other) noexcept
{
    if (this != &other)
    {
        stop();
        if (worker_.joinable())
            worker_.join();

        {
            std::lock_guard<std::mutex> lock1(mtx_);
            std::lock_guard<std::mutex> lock2(other.mtx_);
            jobs_ = std::move(other.jobs_);
            running_ = other.running_.load();
        }

        if (running_)
        {
            worker_ = std::thread(&AsyncJobQueue::worker_loop, this);
        }

        other.stop();
        if (other.worker_.joinable())
            other.worker_.join();
    }
    return *this;
}

void AsyncJobQueue::enqueue(std::function<void()> job)
{
    {
        std::lock_guard<std::mutex> lock(mtx_);
        jobs_.push(std::move(job));
    }
    cv_.notify_one();
}

void AsyncJobQueue::stop()
{
    {
        std::lock_guard<std::mutex> lock(mtx_);
        running_ = false;
    }
    cv_.notify_all();
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
