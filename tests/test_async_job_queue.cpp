#include "async_job_queue.hpp"
#include <cassert>
#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>

using namespace std::chrono_literals;

void test_basic_enqueue()
{
    std::cout << "[TEST] Basic enqueue and execution...\n";

    AsyncJobQueue queue;
    std::atomic<int> counter = 0;

    for (int i = 0; i < 5; ++i)
    {
        queue.enqueue([&]
                      {
            std::this_thread::sleep_for(10ms);
            ++counter; });
    }

    std::this_thread::sleep_for(200ms);
    assert(counter == 5);

    std::cout << "✅ Basic enqueue passed.\n";
}

void test_move_semantics()
{
    std::cout << "[TEST] Move semantics...\n";

    AsyncJobQueue q1;
    std::atomic<int> count = 0;

    q1.enqueue([&]
               { ++count; });
    std::this_thread::sleep_for(20ms);

    AsyncJobQueue q2 = std::move(q1); // move constructor

    q2.enqueue([&]
               { ++count; });
    std::this_thread::sleep_for(100ms);

    assert(count >= 2); // both jobs should have executed
    std::cout << "✅ Move semantics passed.\n";
}

void test_safe_shutdown()
{
    std::cout << "[TEST] Safe shutdown and stop...\n";

    {
        AsyncJobQueue q;
        for (int i = 0; i < 3; ++i)
            q.enqueue([]
                      { std::this_thread::sleep_for(5ms); });
        q.stop();
    } // destructor should join safely

    std::cout << "✅ Safe shutdown passed.\n";
}

int main()
{
    std::cout << "\n=== AsyncJobQueue Test Suite ===\n";

    test_basic_enqueue();
    test_move_semantics();
    test_safe_shutdown();

    std::cout << "\n🎉 All AsyncJobQueue tests passed successfully!\n";
    return 0;
}
