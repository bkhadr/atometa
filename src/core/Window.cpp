#include "Atometa/Core/Window.h"
#include "Atometa/Core/Logger.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_SAMPLES, 4); // 4x MSAA

        m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, 
                                   m_Data.Title.c_str(), nullptr, nullptr);
        ATOMETA_CORE_ASSERT(m_Window, "Failed to create GLFW window!");

        glfwMakeContextCurrent(m_Window);

        int gladStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        ATOMETA_CORE_ASSERT(gladStatus, "Failed to initialize GLAD!");

        if (!props.IconPath.empty()) {
            SetWindowIcon(props.IconPath);
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

        // Load the icon using stb_image
        int width, height, channels;
        stbi_set_flip_vertically_on_load(0); // Don't flip for icons
        
        // Try to load the image
        unsigned char* data = stbi_load(iconPath.c_str(), &width, &height, &channels, 4); // Force RGBA
        
        if (!data) {
            ATOMETA_ERROR("Failed to load window icon: ", iconPath, " - ", stbi_failure_reason());
            return;
        }

        // Create GLFW image
        GLFWimage image;
        image.width = width;
        image.height = height;
        image.pixels = data;

        // Set the window icon
        glfwSetWindowIcon(m_Window, 1, &image);
        
        ATOMETA_INFO("Window icon set: ", iconPath, " (", width, "x", height, ")");

        // Free the image data
        stbi_image_free(data);
    }

    void Window::SetWindowIconFromResources() {
        // This is a placeholder for loading icons from Windows resources
        // You would implement this differently for actual resource loading
        
        #ifdef ATOMETA_PLATFORM_WINDOWS
            ATOMETA_INFO("Resource icon loading would be implemented here for Windows");
        #else
            ATOMETA_INFO("Resource icon loading not implemented for this platform");
        #endif
    }
}