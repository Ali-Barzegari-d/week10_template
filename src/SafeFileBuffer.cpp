// SafeFileBuffer.cpp
#include "SafeFileBuffer.hpp"
#include <iostream>

SafeFileBuffer::SafeFileBuffer(const std::string& path, std::ios::openmode mode)
    : path_(path), mode_(mode)
{
    file_.open(path_, mode_);
    if (!file_.is_open()) {
        throw std::runtime_error("Cannot open file");
    }
}

SafeFileBuffer::~SafeFileBuffer() {
    if (file_.is_open()) {
        // destructor must not throw; best-effort close
        try {
            file_.close();
        } catch (...) {
            // swallow any exception
        }
    }
}

// Move constructor
SafeFileBuffer::SafeFileBuffer(SafeFileBuffer&& other) noexcept
    : file_(std::move(other.file_)),
      path_(std::move(other.path_)),
      mode_(other.mode_)
{
    // leave other in a safe-to-destroy state
    other.mode_ = static_cast<std::ios::openmode>(0);
    other.path_.clear();
}

// Move assignment
SafeFileBuffer& SafeFileBuffer::operator=(SafeFileBuffer&& other) noexcept {
    if (this != &other) {
        // close current file if open
        if (file_.is_open()) {
            try { file_.close(); } catch (...) {}
        }

        // move resources
        file_ = std::move(other.file_);
        path_ = std::move(other.path_);
        mode_ = other.mode_;

        // reset other
        other.mode_ = static_cast<std::ios::openmode>(0);
        other.path_.clear();
    }
    return *this;
}

void SafeFileBuffer::write_bytes(const std::vector<char>& data) {
    if (!file_.is_open()) {
        throw std::runtime_error("Write failed: file not open");
    }
    // ensure open for output
    if ((mode_ & std::ios::out) == 0 && (mode_ & std::ios::app) == 0) {
        throw std::runtime_error("Write failed: stream not opened for output");
    }

    // write all bytes
    file_.write(data.data(), static_cast<std::streamsize>(data.size()));
    file_.flush();
    if (!file_) {
        throw std::runtime_error("Write failed");
    }
}

std::vector<char> SafeFileBuffer::read_bytes() {
    if (!file_.is_open()) {
        throw std::runtime_error("Read failed: file not open");
    }
    if ((mode_ & std::ios::in) == 0) {
        throw std::runtime_error("Read failed: stream not opened for input");
    }

    // seek to end to get size
    file_.seekg(0, std::ios::end);
    if (!file_) {
        throw std::runtime_error("Read failed");
    }
    std::streamoff size = file_.tellg();
    if (size < 0) size = 0;
    file_.seekg(0, std::ios::beg);
    if (!file_) {
        throw std::runtime_error("Read failed");
    }

    std::vector<char> buffer;
    buffer.resize(static_cast<std::size_t>(size));
    if (size > 0) {
        file_.read(buffer.data(), static_cast<std::streamsize>(size));
        if (!file_) {
            throw std::runtime_error("Read failed");
        }
    }
    return buffer;
}

bool SafeFileBuffer::is_open() const noexcept {
    return file_.is_open();
}
