#include "Atometa/Core/Input.h"
#include "Atometa/Core/Application.h"
#include <GLFW/glfw3.h>

namespace Atometa {
    bool Input::IsKeyPressed(int keycode) {
        auto window = Application::Get().GetWindow().GetNativeWindow();
        return glfwGetKey(window, keycode) == GLFW_PRESS;
    }

    bool Input::IsMouseButtonPressed(int button) {
        auto window = Application::Get().GetWindow().GetNativeWindow();
        return glfwGetMouseButton(window, button) == GLFW_PRESS;
    }

    glm::vec2 Input::GetMousePosition() {
        auto window = Application::Get().GetWindow().GetNativeWindow();
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        return {(float)x, (float)y};
    }

    float Input::GetMouseX() { return GetMousePosition().x; }
    float Input::GetMouseY() { return GetMousePosition().y; }
}