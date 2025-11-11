#ifndef THREADED_ACCUMULATOR_HPP
#define THREADED_ACCUMULATOR_HPP

#include <vector>
#include <thread>
#include <mutex>
#include <stdexcept>

class ThreadedAccumulator
{
public:
    explicit ThreadedAccumulator(const std::vector<int> &data, size_t thread_count);

    // Compute total sum using multiple threads
    long compute_sum();

private:
    const std::vector<int> &data;
    size_t thread_count;
    long total_sum;
    std::mutex mtx;

    // Helper for thread computation
    void partial_sum(size_t start, size_t end);
};

#endif
