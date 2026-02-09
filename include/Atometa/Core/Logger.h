#pragma once

#include "Core.h"
#include <string>
#include <iostream>
#include <sstream>

// Forward declare Windows API
extern "C" __declspec(dllimport) void __stdcall OutputDebugStringA(const char* lpOutputString);

namespace Atometa {
    enum class LogLevel { Trace, Info, Warn, Error, Fatal };

    class Logger {
    public:
        static void Init();
        static void Shutdown();

        template<typename... Args>
        static void Trace(Args&&... args) { Log(LogLevel::Trace, std::forward<Args>(args)...); }
        
        template<typename... Args>
        static void Info(Args&&... args) { Log(LogLevel::Info, std::forward<Args>(args)...); }
        
        template<typename... Args>
        static void Warn(Args&&... args) { Log(LogLevel::Warn, std::forward<Args>(args)...); }
        
        template<typename... Args>
        static void Error(Args&&... args) { Log(LogLevel::Error, std::forward<Args>(args)...); }

    private:
        template<typename... Args>
        static void Log(LogLevel level, Args&&... args) {
            std::ostringstream oss;
            ((oss << std::forward<Args>(args)), ...);
            
            const char* levelStr = "";
            switch (level) {
                case LogLevel::Trace: levelStr = "[TRACE]"; break;
                case LogLevel::Info:  levelStr = "[INFO] "; break;
                case LogLevel::Warn:  levelStr = "[WARN] "; break;
                case LogLevel::Error: levelStr = "[ERROR]"; break;
                case LogLevel::Fatal: levelStr = "[FATAL]"; break;
            }
            
            std::string fullMsg = std::string(levelStr) + " " + oss.str() + "\n";
            OutputDebugStringA(fullMsg.c_str());
        }
    };
}

#ifdef ATOMETA_ENABLE_LOGGING
    #define ATOMETA_TRACE(...) ::Atometa::Logger::Trace(__VA_ARGS__)
    #define ATOMETA_INFO(...)  ::Atometa::Logger::Info(__VA_ARGS__)
    #define ATOMETA_WARN(...)  ::Atometa::Logger::Warn(__VA_ARGS__)
    #define ATOMETA_ERROR(...) ::Atometa::Logger::Error(__VA_ARGS__)
#else
    #define ATOMETA_TRACE(...)
    #define ATOMETA_INFO(...)
    #define ATOMETA_WARN(...)
    #define ATOMETA_ERROR(...)
#endif