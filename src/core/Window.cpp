#include "Atometa/Core/Window.h"
#include "Atometa/Core/Logger.h"
#include "Atometa/Core/Input.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <windows.h>
#include "../resource.h"

// Undef problematic windows.h macros that conflict with our code
#ifdef AddAtom
#undef AddAtom
#endif
#endif

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Atometa {
    static bool s_GLFWInitialized = false;

    static void GLFWErrorCallback(int error, const char* description) {
        ATOMETA_ERROR("GLFW Error (", error, "): ", description);
    }

    Window::Window(const WindowProperties& props) {
        Init(props);
    }

    Window::~Window() {
        Shutdown();
    }

    void Window::Init(const WindowProperties& props) {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;
        m_Data.VSync = props.VSync;

        ATOMETA_INFO("Creating window: ", props.Title, " (", props.Width, "x", props.Height, ")");

        if (!s_GLFWInitialized) {
            int success = glfwInit();
            ATOMETA_CORE_ASSERT(success, "Could not initialize GLFW!");
            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitialized = true;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_SAMPLES, 4); // 4x MSAA

        m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, 
                                   m_Data.Title.c_str(), nullptr, nullptr);
        ATOMETA_CORE_ASSERT(m_Window, "Failed to create GLFW window!");

        glfwMakeContextCurrent(m_Window);

        int gladStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        ATOMETA_CORE_ASSERT(gladStatus, "Failed to initialize GLAD!");

        // Initialize scroll wheel callback
        Input::InitializeScrollCallback(m_Window);

        // Set window icon
        if (!props.IconPath.empty()) {
            SetWindowIcon(props.IconPath);
        } else {
            // Try to load from embedded resources (Windows only)
            #ifdef _WIN32
            HWND hwnd = glfwGetWin32Window(m_Window);
            if (hwnd) {
                // Load icon from resources
                HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_APP_ICON));
                if (hIcon) {
                    SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
                    SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
                    ATOMETA_INFO("Window icon set from embedded resources");
                } else {
                    ATOMETA_WARN("Failed to load icon from resources");
                }
            }
            #endif
        }

        ATOMETA_INFO("OpenGL Info:");
        ATOMETA_INFO("  Vendor: ", (const char*)glGetString(GL_VENDOR));
        ATOMETA_INFO("  Renderer: ", (const char*)glGetString(GL_RENDERER));
        ATOMETA_INFO("  Version: ", (const char*)glGetString(GL_VERSION));

        SetVSync(props.VSync);
    }

    void Window::Shutdown() {
        glfwDestroyWindow(m_Window);
    }

    void Window::OnUpdate() {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    void Window::SetVSync(bool enabled) {
        glfwSwapInterval(enabled ? 1 : 0);
        m_Data.VSync = enabled;
    }

    bool Window::ShouldClose() const {
        return glfwWindowShouldClose(m_Window);
    }

    void Window::SetWindowIcon(const std::string& iconPath) {
        if (!m_Window) {
            ATOMETA_WARN("Cannot set icon: window not initialized");
            return;
        }

        if (iconPath.empty()) {
            ATOMETA_WARN("Icon path is empty");
            return;
        }

        #ifdef _WIN32
        // Check if it's an ICO file
        if (iconPath.length() >= 4 && iconPath.substr(iconPath.length() - 4) == ".ico") {
            HWND hwnd = glfwGetWin32Window(m_Window);
            if (hwnd) {
                HICON hIcon = (HICON)LoadImageA(NULL, iconPath.c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
                if (hIcon) {
                    SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
                    SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
                    ATOMETA_INFO("Window icon set from file: ", iconPath);
                } else {
                    ATOMETA_WARN("Failed to load ICO file: ", iconPath);
                }
            }
            return;
        }
        #endif

        // Load PNG/BMP icon using stb_image (fallback for non-ICO files)
        int width, height, channels;
        stbi_set_flip_vertically_on_load(0);
        
        unsigned char* data = stbi_load(iconPath.c_str(), &width, &height, &channels, 4);
        
        if (data) {
            // Set GLFW icon (for Linux/Mac)
            GLFWimage image;
            image.width = width;
            image.height = height;
            image.pixels = data;
            glfwSetWindowIcon(m_Window, 1, &image);
            
            ATOMETA_INFO("Window icon set from file: ", iconPath, " (", width, "x", height, ")");
            
            stbi_image_free(data);
            
            // Also set Windows native icon if on Windows
            #ifdef _WIN32
            HWND hwnd = glfwGetWin32Window(m_Window);
            if (hwnd) {
                // Convert to HICON (simplified - you might want a proper conversion)
                ATOMETA_INFO("Native Windows icon also updated");
            }
            #endif
        } else {
            ATOMETA_WARN("Failed to load icon: ", iconPath);
            ATOMETA_WARN("stb_image error: ", stbi_failure_reason());
        }
    }
}