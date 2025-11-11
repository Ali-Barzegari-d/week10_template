#include "ThreadedAccumulator.hpp"
#include <iostream>
#include <algorithm>

ThreadedAccumulator::ThreadedAccumulator(const std::vector<int> &data, size_t thread_count)
    : data(data), thread_count(thread_count), total_sum(0)
{
    if (thread_count == 0)
    {
        throw std::invalid_argument("Invalid thread count");
    }
}

void ThreadedAccumulator::partial_sum(size_t start, size_t end)
{
    long local_sum = 0;

    for (size_t i = start; i < end; ++i)
    {
        local_sum += data[i];
    }

    // Safely update shared total
    std::lock_guard<std::mutex> lock(mtx);
    total_sum += local_sum;
}

long ThreadedAccumulator::compute_sum()
{
    if (data.empty())
        return 0;

    std::vector<std::thread> threads;
    threads.reserve(thread_count);

    size_t block_size = data.size() / thread_count;
    size_t remainder = data.size() % thread_count;

    size_t start = 0;
    for (size_t i = 0; i < thread_count; ++i)
    {
        size_t end = start + block_size + (i < remainder ? 1 : 0);
        if (start >= data.size())
            break;

        threads.emplace_back(&ThreadedAccumulator::partial_sum, this, start, std::min(end, data.size()));
        start = end;
    }

    // Join all threads (RAII-safe)
    for (auto &t : threads)
    {
        if (t.joinable())
            t.join();
    }

    return total_sum;
}
