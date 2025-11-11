#include "TimedWorkerPool.hpp"
#include <cassert>
#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>

int main()
{
    using namespace std::chrono_literals;

    std::atomic<int> counter{0};

    {
        TimedWorkerPool pool(3, 500ms);

        for (int i = 0; i < 10; ++i)
        {
            pool.submit([&counter, i]()
                        {
                std::this_thread::sleep_for(50ms);
                counter += 1; });
        }

        std::this_thread::sleep_for(1s);

        pool.shutdown();

        bool caught = false;
        try
        {
            pool.submit([]() {});
        }
        catch (const std::runtime_error &)
        {
            caught = true;
        }
        assert(caught);
    }

    assert(counter == 10);
    std::cout << "✅ TimedWorkerPool tests passed!" << std::endl;
}
