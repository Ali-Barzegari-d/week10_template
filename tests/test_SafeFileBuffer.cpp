#include "SafeFileBuffer.hpp"
#include <cassert>
#include <iostream>
#include <filesystem>

int main()
{
    const std::string test_file = "test_buffer.bin";

    // Write test
    {
        SafeFileBuffer writer(test_file, std::ios::out | std::ios::binary);
        std::vector<char> data = {'H', 'e', 'l', 'l', 'o'};
        writer.write_bytes(data);
        assert(writer.is_open());
    }

    // Read test
    {
        SafeFileBuffer reader(test_file, std::ios::in | std::ios::binary);
        auto data = reader.read_bytes();
        assert(std::string(data.begin(), data.end()) == "Hello");
    }

    // Move semantics test
    {
        SafeFileBuffer temp(test_file, std::ios::in | std::ios::binary);
        SafeFileBuffer moved = std::move(temp);
        assert(moved.is_open());
    }

    std::filesystem::remove(test_file);
    std::cout << "✅ SafeFileBuffer tests passed!" << std::endl;
}
