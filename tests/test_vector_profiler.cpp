#include "vector_profiler.hpp"
#include <cassert>
#include <fstream>
#include <iostream>

using namespace csi::perf;

int main()
{
    VectorProfiler vp("report.txt");

    vp.run_tests(50000);
    vp.save_report();

    std::ifstream in("report.txt");
    assert(in.good() && "Report file should exist");

    bool found = false;
    std::string line;
    while (std::getline(in, line))
    {
        if (line.find("move_only") != std::string::npos)
            found = true;
    }

    assert(found && "Report should contain move_only test result");
    std::cout << "✅ All tests passed.\n";
}
