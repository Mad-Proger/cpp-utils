#pragma once

#ifdef __linux__
#include "MemoryMapLinux.hpp"
#else
#error "OS is not supported"
#endif

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <span>

namespace utils {

class FileMap {
public:
    static FileMap openExisting(const std::filesystem::path& path);
    static FileMap createNew(const std::filesystem::path& path, size_t fileSize);

    FileMap(const FileMap&) = delete;
    FileMap& operator=(const FileMap&) = delete;

    FileMap(FileMap&&) noexcept;
    FileMap& operator=(FileMap&&) noexcept;

public:
    std::span<uint8_t> getBytes() const noexcept;
    void swap(FileMap& other) noexcept;

private:
    size_t m_mapSize;
    std::unique_ptr<uint8_t[], detail::MapDeleter> m_mapping;

    FileMap(uint8_t*, detail::MapDeleter&&, size_t) noexcept;
};

}// namespace utils
