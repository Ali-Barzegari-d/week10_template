#include "ThreadedAccumulator.hpp"
#include <iostream>
#include <algorithm>

ThreadedAccumulator::ThreadedAccumulator(const std::vector<int> &data, size_t thread_count)
    : data(data), thread_count(thread_count), total_sum(0)
{
    // TODO: Throw std::invalid_argument if thread_count == 0
}

void ThreadedAccumulator::partial_sum(size_t start, size_t end)
{
    // TODO: Compute sum of data[start:end)
    // Use lock_guard<mutex> to safely add to total_sum
}

long ThreadedAccumulator::compute_sum()
{
    // TODO:
    // 1. Divide data range among threads
    // 2. Launch threads
    // 3. Join all threads
    // 4. Return total_sum
    return 0;
}
