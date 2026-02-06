#pragma once

#include <memory>

// Platform detection
#ifdef _WIN32
    #define ATOMETA_PLATFORM_WINDOWS
#elif defined(__linux__)
    #define ATOMETA_PLATFORM_LINUX
#elif defined(__APPLE__)
    #define ATOMETA_PLATFORM_MACOS
#endif

// Debug configuration
#ifdef _DEBUG
    #define ATOMETA_DEBUG
    #define ATOMETA_ENABLE_ASSERTS
    #define ATOMETA_ENABLE_LOGGING
#endif

// Assertions
#ifdef ATOMETA_ENABLE_ASSERTS
    #include <cassert>
    #define ATOMETA_ASSERT(x, msg) assert((x) && msg)
    #define ATOMETA_CORE_ASSERT(x, msg) assert((x) && msg)
#else
    #define ATOMETA_ASSERT(x, msg)
    #define ATOMETA_CORE_ASSERT(x, msg)
#endif

#define ATOMETA_API

namespace Atometa {
    template<typename T>
    using Scope = std::unique_ptr<T>;
    
    template<typename T, typename... Args>
    constexpr Scope<T> CreateScope(Args&&... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
    
    template<typename T>
    using Ref = std::shared_ptr<T>;
    
    template<typename T, typename... Args>
    constexpr Ref<T> CreateRef(Args&&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}