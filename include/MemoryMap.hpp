#pragma once

#include <filesystem>

namespace utils {

class MapDeleter {
public:
    void operator()(std::byte* mapped);

private:
    MapDeleter() = default;
    MapDeleter(int fd, size_t fileSize);

    int m_fileDescriptor;
    size_t m_mapLength;

    friend std::unique_ptr<std::byte[], MapDeleter> MapFile(const std::filesystem::path& path);
};

std::unique_ptr<std::byte[], MapDeleter> MapFile(const std::filesystem::path& path);

}// namespace utils
