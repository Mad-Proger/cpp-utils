#pragma once

#ifdef __unix__
#include "linux_context.hpp"
using OsContext = LinuxContext;
#else
#error "Unsupported platform"
#endif
