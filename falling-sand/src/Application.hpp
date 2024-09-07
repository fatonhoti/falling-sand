#pragma once

// vendor
#include "glad/glad.h"
#include "GLFW/glfw3.h"

// std
#include <string>
#include <memory>
#include <optional>

// src
#include "AppInstance.hpp"

// Impl from: https://stackoverflow.com/a/1008289/26041804
class Application {

public:
	static Application& GetInstance() {
		static Application instance;
		return instance;
	}

	Application(const Application&) = delete;
	void operator=(const Application&) = delete;

	void Start();

	int Init(const int window_width, const int window_height, const std::string title);
	void Destroy();

	int AttachAppInstance(const std::unique_ptr<AppInstance> ai);
	std::optional<std::unique_ptr<AppInstance>> DetachAppInstance();

private:
	Application() {}; // Yes, leave it like this!

	struct Window {
		GLFWwindow* handle;
		int width;
		int height;
		std::string title;
	} window;
	std::unique_ptr<AppInstance> app_instance{ nullptr };

	bool glfw_initialized = false;
	bool glad_initialized = false;
	bool app_initialized = false;

	int InitGLFW(const int window_width, const int window_height, const std::string title);
	int InitGLAD();
	double GetDeltatime();
};

namespace {

	// From documentation: https://www.glfw.org/docs/latest/input_guide.html
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	void cursor_enter_callback(GLFWwindow* window, int entered);
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	static void APIENTRY glDebugOutput(GLenum source,
		GLenum type,
		unsigned int id,
		GLenum severity,
		GLsizei length,
		const char* message,
		const void* userParam);
}