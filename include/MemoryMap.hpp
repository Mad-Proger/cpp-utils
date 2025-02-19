#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <span>

namespace utils {

class FileMapping {
public:
    FileMapping();
    explicit FileMapping(const std::filesystem::path& path);

    FileMapping(FileMapping&&) noexcept;
    FileMapping& operator=(FileMapping&&) noexcept;

    class MapDeleter {
    public:
        void operator()(uint8_t* mapped) noexcept;

    private:
        MapDeleter() = default;
        MapDeleter(int fd, size_t fileSize);

        int m_fileDescriptor = -1;
        size_t m_mapLength = 0;

        friend class FileMapping;
    };

    std::span<uint8_t> getBytes() const noexcept;
    void swap(FileMapping& other) noexcept;

private:
    size_t m_fileSize;
    std::unique_ptr<uint8_t[], MapDeleter> m_mapping;
};

}// namespace utils
