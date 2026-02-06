#pragma once

#include "Core.h"
#include "Window.h"

namespace Atometa {
    class Application {
    public:
        Application(const std::string& name = "Atometa");
        virtual ~Application();

        void Run();
        void Close();

        Window& GetWindow() { return *m_Window; }
        static Application& Get() { return *s_Instance; }

    private:
        Scope<Window> m_Window;
        bool m_Running = true;
        float m_LastFrameTime = 0.0f;
        static Application* s_Instance;
    };

    Application* CreateApplication();
}