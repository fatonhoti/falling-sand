#pragma once

// vendor
#include "glad/glad.h"
#include "glm.hpp"

// std
#include <string>
#include <unordered_map>

class Shader {
public:
	Shader() = delete;
	~Shader();
	Shader(std::string shader_name);

	void Bind() const;
	void Unbind() const;

	GLuint GetShaderProgram() const;

	void SetMat4x4(const glm::mat4x4& mat, const std::string& uniform_name);
	void SetVec4(const glm::vec4& vec, const std::string& uniform_name);
	void SetVec3(const glm::vec3& vec, const std::string& uniform_name);
	void SetVec2(const glm::vec2& vec, const std::string& uniform_name);
	void SetInteger1(const int& integer, const std::string& uniform_name);
	void SetFloat1(const float& float_, const std::string& uniform_name);

private:
	GLuint m_shader_program{ 0 };
	GLuint CreateShader(std::string& shader_name, unsigned int shader_type);
	GLuint CheckForErrors(unsigned int shader, unsigned int err_type);

	int GetUniformLoc(const std::string& uniform_name);
	std::unordered_map<std::string, int> m_UniformCache{};
};