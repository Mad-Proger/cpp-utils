#pragma once

#include <filesystem>

namespace utils {

class MapDeleter {
public:
    void operator()(std::byte* mapped) noexcept;

private:
    MapDeleter() = default;
    MapDeleter(int fd, size_t fileSize);

    int m_fileDescriptor = -1;
    size_t m_mapLength = 0;

    friend std::unique_ptr<std::byte[], MapDeleter> MapFile(const std::filesystem::path& path);
};

std::unique_ptr<std::byte[], MapDeleter> MapFile(const std::filesystem::path& path);

}// namespace utils
