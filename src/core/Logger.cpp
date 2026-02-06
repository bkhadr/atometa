#include "Atometa/Core/Logger.h"

namespace Atometa {
    void Logger::Init() {
        Info("Atometa Logger initialized");
    }

    void Logger::Shutdown() {
        Info("Atometa Logger shutdown");
    }
}