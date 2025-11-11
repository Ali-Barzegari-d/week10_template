#ifndef SAFE_FILE_BUFFER_HPP
#define SAFE_FILE_BUFFER_HPP

#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>

class SafeFileBuffer
{
public:
    // Opens file with given mode (e.g., std::ios::in | std::ios::binary)
    explicit SafeFileBuffer(const std::string &path, std::ios::openmode mode);
    ~SafeFileBuffer();

    // Non-copyable
    SafeFileBuffer(const SafeFileBuffer &) = delete;
    SafeFileBuffer &operator=(const SafeFileBuffer &) = delete;

    // Movable
    SafeFileBuffer(SafeFileBuffer &&other) noexcept;
    SafeFileBuffer &operator=(SafeFileBuffer &&other) noexcept;

    // Write binary data
    void write_bytes(const std::vector<char> &data);

    // Read all bytes from file
    std::vector<char> read_bytes();

    // Check if file is open
    bool is_open() const noexcept;

private:
    std::fstream file_;
    std::string path_;
    std::ios::openmode mode_;
};

#endif
