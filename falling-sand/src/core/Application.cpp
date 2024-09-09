#include "Application.hpp"

// std
#include <iostream>

// src
#include "InputHandler.hpp"

void Application::Start()
{
    if (!app_initialized) {
        std::cout << "[Application][Start][ERROR] Application has not been initialized, call 'Init()' before calling 'Run()'";
        return;
    }

    if (app_instance->Init()) {
        std::cout << "[Application][Start][ERROR] Failed to initialize application instance.\n";
        return;
    }

    static double currentTime = 0.0;
    static double lastTime = 0.0;
    static double dt = 0.0;

    while (!glfwWindowShouldClose(this->window.handle))
    {
        currentTime = glfwGetTime();
        dt = currentTime - lastTime;
        lastTime = currentTime;

        if (app_instance->Tick(dt)) [[unlikely]] {
            std::cout << "[Application][Start][NOTICE] Instanced application returned.\n";
            break;
        }

        glfwSwapBuffers(this->window.handle);
        glfwPollEvents();
    }

}

int Application::Init(const int window_width, const int window_height, const std::string title)
{

    if (app_initialized) {
        std::cout << "[APPLICATION][INIT][WARNING] Application has already been initialized.\n";
        return 0;
    }

    if (InitGLFW(window_width, window_height, title))
        return -1;

    glfw_initialized = true;

    window.width = window_width;
    window.height = window_height;
    window.title = title;

    if (InitGLAD())
        return -1;

    glad_initialized = true;

    glViewport(0, 0, window.width, window.height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    app_initialized = true;

    return 0;
}

int Application::AttachAppInstance(std::unique_ptr<AppInstance> ai)
{
    if (app_instance) {
        std::cout << "[APPLICATION][AttachAppInstance][ERROR] There's already an application instance attached.\n";
        std::cout << "[APPLICATION][AttachAppInstance][ERROR] If you want to attach a new instance, call 'DetachAppInstance()' first. \n";
        return -1;
    }

    app_instance = std::move(ai);
    app_instance->SetWindowHandle(this->window.handle);
    return 0;
}

std::optional<std::unique_ptr<AppInstance>> Application::DetachAppInstance()
{
    
    if (app_instance == nullptr) {
        std::cout << "[APPLICATION][DetachAppInstance][ERROR] There is no application instance to detach.\n";
        return std::nullopt;
    }

    std::unique_ptr<AppInstance> i = std::move(app_instance);
    app_instance = nullptr;
    return std::move(i);
}

int Application::InitGLFW(const int window_width, const int window_height, const std::string title)
{
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    this->window.handle = glfwCreateWindow(window_width, window_height, title.c_str(), NULL, NULL);
    if (!this->window.handle)
    {
        glfwTerminate();
        std::cout << "[APPLICATION][InitializeGLFW][ERROR][glfwCreateWindow] Failed to create window using GLFW.\n";
        return -1;
    }
    glfwMakeContextCurrent(this->window.handle);

    // Disable vsync (we want our transistors to go brrrrrrrrrrrrrrrrrrrrrr)
    glfwSwapInterval(0);

    // From documentation: https://www.glfw.org/docs/latest/input_guide.html
    glfwSetFramebufferSizeCallback(this->window.handle, framebuffer_size_callback);
    glfwSetKeyCallback(this->window.handle, key_callback);
    glfwSetCursorPosCallback(this->window.handle, cursor_position_callback);
    glfwSetCursorEnterCallback(this->window.handle, cursor_enter_callback);
    glfwSetMouseButtonCallback(this->window.handle, mouse_button_callback);
    glfwSetScrollCallback(this->window.handle, scroll_callback);

    return 0;

}

int Application::InitGLAD()
{
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
        std::cout << "[APPLICATION][InitializeGLAD][ERROR][gladLoadGLLoader] Failed to initialize GLAD.\n";
        return -1;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    return 0;

}

void Application::Destroy()
{

    if (this->app_instance != nullptr)
        this->app_instance->Destroy();

    std::cout << "[APPLICATION][DESTROY][NOTICE] Destroying application.\n";
    glfwDestroyWindow(this->window.handle);
    glfwTerminate();
}

namespace {

    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_UNKNOWN)
            return;

        InputHandler::UpdateKeyInput(key, action);
    }

    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
    {
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);

        // Mouse is outside the window
        if (xpos < 0.0 || xpos > static_cast<double>(w) - 1.0 || ypos < 0.0 || ypos > static_cast<double>(h) - 1.0)
            return;

        InputHandler::UpdateCursorPosition(xpos, ypos);
    }

    void cursor_enter_callback(GLFWwindow* window, int entered)
    {
        if (entered)
        {
            // The cursor entered the content area of the window
        }
        else
        {
            // The cursor left the content area of the window
        }
    }

    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        InputHandler::UpdateButtonInput(button, action);
    }

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
    }

    static void APIENTRY glDebugOutput(GLenum source,
        GLenum type,
        unsigned int id,
        GLenum severity,
        GLsizei length,
        const char* message,
        const void* userParam)
    {
        // ignore non-significant error/warning codes
        if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

        std::cout << "---------------" << std::endl;
        std::cout << "Debug message (" << id << "): " << message << std::endl;

        switch (source)
        {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
        } std::cout << std::endl;

        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
        } std::cout << std::endl;

        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
        } std::cout << std::endl;
        std::cout << std::endl;
    }
}
