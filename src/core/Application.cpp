#include "Atometa/Core/Application.h"
#include "Atometa/Core/Logger.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Atometa {
    Application* Application::s_Instance = nullptr;

    Application::Application(const std::string& name) {
        ATOMETA_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        Logger::Init();
        ATOMETA_INFO("========================================");
        ATOMETA_INFO("  Atometa Engine v0.1.0");
        ATOMETA_INFO("  3D Chemistry Simulation");
        ATOMETA_INFO("========================================");

        WindowProperties props;
        props.Title = name;
        m_Window = CreateScope<Window>(props);

        #if defined(ATOMETA_PLATFORM_WINDOWS)
            props.IconPath = "assets/icons/app.png";
        #elif defined(ATOMETA_PLATFORM_MACOS)
            props.IconPath = "../Resources/assets/icons/app.png";
        #else
            props.IconPath = "../assets/icons/app.png";
        #endif
    }

    Application::~Application() {
        Logger::Shutdown();
    }

    void Application::Run() {
        ATOMETA_INFO("Application started");

        while (m_Running && !m_Window->ShouldClose()) {
            float time = (float)glfwGetTime();
            float deltaTime = time - m_LastFrameTime;
            m_LastFrameTime = time;

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            m_Window->OnUpdate();
        }

        ATOMETA_INFO("Application shutdown");
    }

    void Application::Close() {
        m_Running = false;
    }
}