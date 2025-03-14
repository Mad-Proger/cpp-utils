#pragma once

#ifndef __linux__
#error "Must be compiled only for linux"
#endif

#include <cstddef>
#include <cstdint>

namespace utils::detail {

class MapDeleter {
public:
    void operator()(uint8_t* mapped) noexcept;

public:
    MapDeleter() = default;
    MapDeleter(int fd, size_t fileSize);

    MapDeleter(const MapDeleter&) = delete;
    MapDeleter& operator=(const MapDeleter&) = delete;

    MapDeleter(MapDeleter&&) noexcept;
    MapDeleter& operator=(MapDeleter&&) noexcept;

    void swap(MapDeleter& other) noexcept;

private:
    int m_fileDescriptor{-1};
    size_t m_mapLength{0};
};

}// namespace utils::detail
