#pragma once

#ifdef __unix__
#include "unix_context.hpp"
using OsContext = UnixContext;
#elif _WIN32
#include "windows_context.hpp"
using OsContext = WindowsContext;
#else
#error "Unsupported platform"
#endif
