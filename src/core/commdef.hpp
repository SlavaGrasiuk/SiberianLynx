#pragma once

#define MANGOFF		extern "C"

#ifdef DEBUG
static constexpr bool g_debug = true;
#elif defined(NDEBUG)
static constexpr bool g_debug = false;
#endif

