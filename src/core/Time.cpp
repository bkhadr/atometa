#include "Atometa/Core/Time.h"
#include <GLFW/glfw3.h>

namespace Atometa {
    float Time::s_DeltaTime = 0.0f;
    float Time::GetTime() { return (float)glfwGetTime(); }
}