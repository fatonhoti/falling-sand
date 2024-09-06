#pragma once

// vendor
#include "glad/glad.h"
#include "GLFW/glfw3.h"

// std
#include <memory>

class AppInstance {
public:
    virtual ~AppInstance() = default;
    virtual int Init() = 0;
    virtual int Tick() = 0;
    virtual void Destroy() = 0;
    void SetWindowHandle(GLFWwindow* handle) {
        this->window_handle = handle;
    }
protected:
    GLFWwindow* window_handle;
};