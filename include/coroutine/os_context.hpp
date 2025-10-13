#pragma once

#ifdef __unix__
#include "linux_context.hpp"
using OsContext = LinuxContext;
#elif _WIN32
#include "windows_context.hpp"
using OsContext = WindowsContext;
#else
#error "Unsupported platform"
#endif
