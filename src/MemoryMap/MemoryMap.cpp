#include "MemoryMap.hpp"
#include "MemoryMapLinux.hpp"
#include <cstdint>

namespace utils {

FileMap::FileMap(FileMap&& other) noexcept: m_mapSize{other.m_mapSize}, m_mapping{std::move(other.m_mapping)} {
    other.m_mapSize = 0;
}

FileMap& FileMap::operator=(FileMap&& other) noexcept {
    FileMap moved = std::move(other);
    swap(moved);
    return *this;
}

std::span<uint8_t> FileMap::getBytes() const noexcept {
    return {m_mapping.get(), m_mapSize};
}

void FileMap::swap(FileMap& other) noexcept {
    std::swap(m_mapSize, other.m_mapSize);
    m_mapping.swap(other.m_mapping);
}

FileMap::FileMap(uint8_t* data, detail::MapDeleter&& deleter, size_t mapSize) noexcept
    : m_mapSize{mapSize}
    , m_mapping{data, std::move(deleter)} {}

}// namespace utils
