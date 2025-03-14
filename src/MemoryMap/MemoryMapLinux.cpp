#include "MemoryMapLinux.hpp"
#include "Defer.hpp"
#include "MemoryMap.hpp"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <fcntl.h>
#include <filesystem>
#include <sys/mman.h>
#include <unistd.h>

#include <cerrno>
#include <system_error>

namespace {

inline void errnoThrow(const char* what) {
    throw std::system_error(errno, std::system_category(), what);
}

uint8_t* mapSized(int fd, size_t fileSize) {
    auto mapped = static_cast<uint8_t*>(mmap(nullptr, fileSize, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0));
    if (mapped == MAP_FAILED) errnoThrow("could not map file to memory");
    return mapped;
}

}// namespace

namespace utils {

namespace detail {

    void MapDeleter::operator()(uint8_t* mapped) noexcept {
        int errCode = munmap(mapped, m_mapLength);
        if (errCode == -1) {
            perror("could not unmap memory");
        }
        close(m_fileDescriptor);
    }

    MapDeleter::MapDeleter(int fd, size_t fileSize): m_fileDescriptor{fd}, m_mapLength{fileSize} {}

    MapDeleter::MapDeleter(MapDeleter&& other) noexcept
        : m_fileDescriptor{other.m_fileDescriptor}
        , m_mapLength{other.m_mapLength} {
        other.m_fileDescriptor = -1;
        other.m_mapLength = 0;
    }

    MapDeleter& MapDeleter::operator=(MapDeleter&& other) noexcept {
        MapDeleter moved = std::move(other);
        swap(moved);
        return *this;
    }

}// namespace detail

FileMap FileMap::openExisting(const std::filesystem::path& path) {
    int fd = open(std::filesystem::absolute(path).c_str(), 0, O_RDWR);
    if (fd == -1) errnoThrow("could not open file to map");
    Defer closeFd{[fd]() { close(fd); }};

    off64_t end = lseek64(fd, 0, SEEK_END);
    if (end == -1) errnoThrow("could not find file end");
    off64_t start = lseek64(fd, 0, SEEK_SET);
    if (start == -1) errnoThrow("could not find file start");

    auto fileSize = static_cast<size_t>(end - start);
    uint8_t* mapped = mapSized(fd, fileSize);
    closeFd.cancel();

    return {mapped, detail::MapDeleter{fd, fileSize}, fileSize};
}

FileMap FileMap::createNew(const std::filesystem::path& path, size_t fileSize) {
    int fd = open(path.c_str(), O_CREAT | O_EXCL, O_RDWR);
    if (fd == -1) errnoThrow("could not create file");
    Defer closeFd{[fd]() { close(fd); }};
    int err = fallocate64(fd, 0, 0, static_cast<off64_t>(fileSize));
    if (err == -1) errnoThrow("could not allocate memory for file on disk");

    uint8_t* mapped = mapSized(fd, fileSize);
    closeFd.cancel();

    return {mapped, detail::MapDeleter{fd, fileSize}, fileSize};
}

}// namespace utils
