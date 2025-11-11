#pragma once
#include <string>
#include <vector>

namespace csi::perf
{

    class VectorProfiler
    {
    public:
        explicit VectorProfiler(std::string report_file);
        ~VectorProfiler();

        // Runs all profiling tests for n insertions
        void run_tests(size_t n);

        // Saves profiling results to the report file
        void save_report() const;

    private:
        struct Result
        {
            std::string label;
            long long time_micro; // duration in microseconds
            size_t final_size;
        };

        std::vector<Result> results;
        std::string file_name;

        long long measure_no_reserve(size_t n);
        long long measure_with_reserve(size_t n);
        long long measure_move_only(size_t n);
        long long measure_copy_only(size_t n);
    };

} // namespace csi::perf
