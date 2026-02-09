#pragma once

#include "Core.h"
#include <string>

struct GLFWwindow;

namespace Atometa {
    struct WindowProperties {
        std::string Title;
        uint32_t Width;
        uint32_t Height;
        bool VSync;
        std::string IconPath;

        WindowProperties(const std::string& title = "Atometa",
                        uint32_t width = 1600,
                        uint32_t height = 900,
                        bool vsync = true,
                        const std::string& iconPath = "")
            : Title(title), Width(width), Height(height), VSync(vsync), IconPath(iconPath) {}
    };

    class Window {
    public:
        Window(const WindowProperties& props = WindowProperties());
        ~Window();

        void OnUpdate();
        
        uint32_t GetWidth() const { return m_Data.Width; }
        uint32_t GetHeight() const { return m_Data.Height; }
        float GetAspectRatio() const { return (float)m_Data.Width / (float)m_Data.Height; }
        
        void SetVSync(bool enabled);
        bool IsVSync() const { return m_Data.VSync; }

        GLFWwindow* GetNativeWindow() const { return m_Window; }
        bool ShouldClose() const;

        void SetWindowIcon(const std::string& iconPath);

    private:
        void Init(const WindowProperties& props);
        void Shutdown();

        GLFWwindow* m_Window;
        struct WindowData {
            std::string Title;
            uint32_t Width, Height;
            bool VSync;
        };
        WindowData m_Data;
    };
}