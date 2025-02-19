#include <cstdint>
#include <fcntl.h>
#include <filesystem>
#include <sys/mman.h>
#include <unistd.h>

#include <cerrno>
#include <system_error>

#include "Defer.hpp"
#include "MemoryMap.hpp"

namespace {

inline void errnoThrow(const char* what) {
    throw std::system_error(errno, std::system_category(), what);
}

}// namespace

namespace utils {

void FileMapping::MapDeleter::operator()(uint8_t* mapped) noexcept {
    munmap(mapped, m_mapLength);
    close(m_fileDescriptor);
}

FileMapping::MapDeleter::MapDeleter(int fd, size_t fileSize): m_fileDescriptor(fd), m_mapLength(fileSize) {}

FileMapping::FileMapping(): m_fileSize(0), m_mapping{nullptr, MapDeleter()} {}

FileMapping::FileMapping(const std::filesystem::path& path): FileMapping() {
    int fd = open(std::filesystem::absolute(path).c_str(), 0, O_RDWR);
    if (fd == -1) errnoThrow("could not open file to map");
    Defer closeFd([fd]() { close(fd); });

    off64_t end = lseek64(fd, 0, SEEK_END);
    if (end == -1) errnoThrow("could not find file end");
    off64_t start = lseek64(fd, 0, SEEK_SET);
    if (start == -1) errnoThrow("could not find file start");

    m_fileSize = static_cast<size_t>(end - start);
    auto mapped = static_cast<uint8_t*>(mmap(nullptr, m_fileSize, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, start));
    if (mapped == MAP_FAILED) errnoThrow("could not map file to memory");

    closeFd.cancel();
    m_mapping = std::unique_ptr<uint8_t[], MapDeleter>{mapped, MapDeleter(fd, m_fileSize)};
}

FileMapping::FileMapping(FileMapping&& other) noexcept
    : m_fileSize(other.m_fileSize)
    , m_mapping(std::move(other.m_mapping)) {
    other.m_fileSize = 0;
}

FileMapping& FileMapping::operator=(FileMapping&& other) noexcept {
    FileMapping moved = std::move(other);
    swap(moved);
    return *this;
}

std::span<uint8_t> FileMapping::getBytes() const noexcept {
    return {m_mapping.get(), m_fileSize};
}

void FileMapping::swap(FileMapping& other) noexcept {
    std::swap(m_fileSize, other.m_fileSize);
    std::swap(m_mapping, other.m_mapping);
}

}// namespace utils
