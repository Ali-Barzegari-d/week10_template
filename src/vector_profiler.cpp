#include "vector_profiler.hpp"
#include <chrono>
#include <fstream>
#include <iostream>

namespace csi::perf
{

    namespace
    {
        struct CopyOnly
        {
            int x;
            CopyOnly() = default;
            CopyOnly(const CopyOnly &) = default;
            CopyOnly(CopyOnly &&) = delete;
        };

        struct MoveOnly
        {
            int x;
            MoveOnly() = default;
            MoveOnly(const MoveOnly &) = delete;
            MoveOnly(MoveOnly &&) noexcept = default;
        };
    } // namespace

    VectorProfiler::VectorProfiler(std::string report_file)
        : file_name(std::move(report_file)) {}

    VectorProfiler::~VectorProfiler() = default;

    void VectorProfiler::run_tests(size_t n)
    {
        results.clear();

        // TODO: call measure functions and store results
        // Example:
        // results.push_back({"no_reserve", measure_no_reserve(n), n});
        // results.push_back({"with_reserve", measure_with_reserve(n), n});
        // results.push_back({"copy_only", measure_copy_only(n), n});
        // results.push_back({"move_only", measure_move_only(n), n});
    }

    long long VectorProfiler::measure_no_reserve(size_t n)
    {
        // TODO:
        // - Create std::vector<int> v;
        // - Insert n elements using push_back, without reserve.
        // - Measure time using std::chrono::steady_clock.
        // - Return duration in microseconds.
        return 0;
    }

    long long VectorProfiler::measure_with_reserve(size_t n)
    {
        // TODO:
        // - Create std::vector<int> v;
        // - Call v.reserve(n);
        // - Insert n elements using push_back.
        // - Measure and return time in microseconds.
        return 0;
    }

    long long VectorProfiler::measure_copy_only(size_t n)
    {
        // TODO:
        // - Create std::vector<CopyOnly> v;
        // - Insert n elements using emplace_back().
        // - Measure and return time in microseconds.
        return 0;
    }

    long long VectorProfiler::measure_move_only(size_t n)
    {
        // TODO:
        // - Create std::vector<MoveOnly> v;
        // - Insert n elements using emplace_back(MoveOnly{});
        // - Measure and return time in microseconds.
        return 0;
    }

    void VectorProfiler::save_report() const
    {
        // TODO:
        // - Open file_name with std::ofstream (truncate mode)
        // - Write a header (Label | Time(us) | Size)
        // - Write all entries from results vector in aligned columns
        // - Handle file open failure with std::cerr message
    }

} // namespace csi::perf
