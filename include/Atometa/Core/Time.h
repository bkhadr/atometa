#pragma once

#include "Core.h"

namespace Atometa {
    class Time {
    public:
        static float GetTime();
        static float GetDeltaTime() { return s_DeltaTime; }
        static void SetDeltaTime(float dt) { s_DeltaTime = dt; }
    private:
        static float s_DeltaTime;
    };
}