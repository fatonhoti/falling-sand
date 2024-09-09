#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

class AppInstance {
public:
    virtual ~AppInstance() = default;
    [[nodiscard]] virtual int Init() = 0;
    [[nodiscard]] virtual int Tick(const double dt) = 0;
    virtual void Destroy() = 0;
    void SetWindowHandle(GLFWwindow* handle) {
        this->window_handle = handle;
    }
protected:
    GLFWwindow* window_handle;
};