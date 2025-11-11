cmake_minimum_required(VERSION 3.15)
project(csi2372_week10 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
enable_testing()

# --- Task 1: ThreadedAccumulator ---
add_library(threaded_accumulator src/ThreadedAccumulator.cpp)
target_include_directories(threaded_accumulator PUBLIC include)

add_executable(threaded_accumulator_tests tests/test_ThreadedAccumulator.cpp)
target_link_libraries(threaded_accumulator_tests PRIVATE threaded_accumulator)
add_test(NAME threaded_accumulator_tests COMMAND $<TARGET_FILE:threaded_accumulator_tests>)

# --- Task 2: TimedWorkerPool ---
add_library(timed_worker_pool src/TimedWorkerPool.cpp)
target_include_directories(timed_worker_pool PUBLIC include)

add_executable(timed_worker_pool_tests tests/test_TimedWorkerPool.cpp)
target_link_libraries(timed_worker_pool_tests PRIVATE timed_worker_pool)
add_test(NAME timed_worker_pool_tests COMMAND $<TARGET_FILE:timed_worker_pool_tests>)

# --- Task 3: SafeFileBuffer ---
add_library(Safe_File src/SafeFileBuffer.cpp)
target_include_directories(Safe_File PUBLIC include)

add_executable(Safe_File_tests tests/test_SafeFileBuffer.cpp)
target_link_libraries(Safe_File_tests PRIVATE Safe_File)
add_test(NAME Safe_File_tests COMMAND $<TARGET_FILE:Safe_File_tests>)

# --- Task 4: vector_profiler ---
add_library(vector_profiler src/vector_profiler.cpp)
target_include_directories(vector_profiler PUBLIC include)

add_executable(vector_profiler_tests tests/test_vector_profiler.cpp)
target_link_libraries(vector_profiler_tests PRIVATE vector_profiler)
add_test(NAME vector_profiler_tests COMMAND $<TARGET_FILE:vector_profiler_tests>)
