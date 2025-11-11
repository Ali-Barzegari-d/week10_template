#include "SafeFileBuffer.hpp"
#include <iostream>

SafeFileBuffer::SafeFileBuffer(const std::string &path, std::ios::openmode mode)
    : path_(path), mode_(mode)
{
    // TODO: Open the file using fstream with the given mode
    // TODO: If file fails to open, throw std::runtime_error("Cannot open file")
}

SafeFileBuffer::~SafeFileBuffer()
{
    // TODO: Ensure file is properly closed
}

SafeFileBuffer::SafeFileBuffer(SafeFileBuffer &&other) noexcept
    : file_(std::move(other.file_)), path_(std::move(other.path_)), mode_(other.mode_)
{
    // TODO: reset other's state
}

SafeFileBuffer &SafeFileBuffer::operator=(SafeFileBuffer &&other) noexcept
{
    if (this != &other)
    {
        // TODO: close current file (if open)
        // TODO: move other's resources
    }
    return *this;
}

void SafeFileBuffer::write_bytes(const std::vector<char> &data)
{
    // TODO:
    // - Verify file is open and in output mode
    // - Write all bytes
    // - If write fails, throw std::runtime_error("Write failed")
}

std::vector<char> SafeFileBuffer::read_bytes()
{
    // TODO:
    // - Verify file is open and in input mode
    // - Seek to end, get size, read all bytes
    // - Throw std::runtime_error on read error
    // - Return vector<char> containing file contents
}

bool SafeFileBuffer::is_open() const noexcept
{
    // TODO: Return true if file is open
    return false; // placeholder
}
