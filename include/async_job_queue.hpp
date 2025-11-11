#pragma once
#include <functional>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

class AsyncJobQueue
{
public:
    AsyncJobQueue();
    ~AsyncJobQueue();

    // Non-copyable but movable
    AsyncJobQueue(const AsyncJobQueue &) = delete;
    AsyncJobQueue &operator=(const AsyncJobQueue &) = delete;
    AsyncJobQueue(AsyncJobQueue &&other) noexcept;
    AsyncJobQueue &operator=(AsyncJobQueue &&other) noexcept;

    void enqueue(std::function<void()> job);
    void stop();

private:
    void worker_loop();

    std::queue<std::function<void()>> jobs_;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::atomic<bool> running_{true};
    std::thread worker_;
};
