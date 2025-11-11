#include "ThreadedAccumulator.hpp"
#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

int main()
{
    std::vector<int> data(1000);
    std::iota(data.begin(), data.end(), 1); // 1..1000

    ThreadedAccumulator acc(data, 4);
    long result = acc.compute_sum();

    long expected = std::accumulate(data.begin(), data.end(), 0L);
    assert(result == expected);

    bool caught = false;
    try
    {
        ThreadedAccumulator invalid(data, 0);
    }
    catch (const std::invalid_argument &)
    {
        caught = true;
    }
    assert(caught);

    std::cout << "✅ ThreadedAccumulator tests passed!" << std::endl;
}
