#include "ComputeShader.hpp"

// vendor
#include "glad/glad.h"
#include "gtc/type_ptr.hpp"

// std
#include <iostream>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

ComputeShader::~ComputeShader()
{
	glDeleteShader(m_shader_program);
}

ComputeShader::ComputeShader(std::string shader_name)
{
	auto base_dir = fs::current_path().append("src").append("shaders").string();
	auto path_comp = base_dir + "\\" + shader_name + ".comp";

	GLuint c_shader = CreateShader(path_comp, GL_COMPUTE_SHADER);
	if (c_shader == -1) {
		std::cout << "[SHADER][Constructor][ERROR] COULD_NOT_CREATE_VERTEX_SHADER" << std::endl;
		exit(1);
	}

	GLuint shader_program;
	shader_program = glCreateProgram();

	glAttachShader(shader_program, c_shader);
	glLinkProgram(shader_program);

	GLuint err = CheckForErrors(shader_program, GL_LINK_STATUS);
	if (err == -1) {
		std::cout << "[SHADER][Constructor][ERROR] COULD_NOT_CREATE_SHADER: " + shader_name << std::endl;
		exit(1);
	}

	m_shader_program = shader_program;

	glDeleteShader(c_shader);
}

void ComputeShader::Bind() const
{
	glUseProgram(m_shader_program);
}

void ComputeShader::Unbind() const
{
	glUseProgram(0);
}

GLuint ComputeShader::GetShaderProgram() const
{
	return m_shader_program;
}

void ComputeShader::SetMat4x4(const glm::mat4x4& mat, const std::string& uniform_name) {
	auto uniform_location = GetUniformLoc(uniform_name);
	if (uniform_location == -1)
		return;
	glUniformMatrix4fv(uniform_location, 1, GL_FALSE, glm::value_ptr(mat));
}

void ComputeShader::SetVec4(const glm::vec4& vec, const std::string& uniform_name) {
	auto uniform_location = GetUniformLoc(uniform_name);
	if (uniform_location == -1)
		return;
	glUniform4fv(uniform_location, 1, glm::value_ptr(vec));
}

void ComputeShader::SetVec3(const glm::vec3& vec, const std::string& uniform_name)
{
	auto uniform_location = GetUniformLoc(uniform_name);
	if (uniform_location == -1)
		return;
	glUniform3fv(uniform_location, 1, glm::value_ptr(vec));
}

void ComputeShader::SetVec2(const glm::vec2& vec, const std::string& uniform_name)
{
	auto uniform_location = GetUniformLoc(uniform_name);
	if (uniform_location == -1)
		return;
	glUniform2fv(uniform_location, 1, glm::value_ptr(vec));
}

void ComputeShader::SetInteger1(const int& integer, const std::string& uniform_name)
{
	auto uniform_location = GetUniformLoc(uniform_name);
	if (uniform_location == -1)
		return;
	glUniform1i(uniform_location, integer);
}

void ComputeShader::SetFloat1(const float& float_, const std::string& uniform_name)
{
	auto uniform_location = GetUniformLoc(uniform_name);
	if (uniform_location == -1)
		return;
	glUniform1f(uniform_location, float_);
}

GLint ComputeShader::GetUniformLoc(const std::string& uniform_name)
{
	auto it = m_UniformCache.find(uniform_name);
	if (it == m_UniformCache.end()) {
		GLint loc = glGetUniformLocation(m_shader_program, uniform_name.c_str());
		if (loc == -1) {
			std::cout << m_shader_program << "\n";
			std::cout << "[SHADER][GetUniformLoc][ERROR] Could not find location for uniform '" + uniform_name + "'.\n";
			return -1;
		}
		m_UniformCache[uniform_name] = loc;
		return loc;
	}

	return it->second;
}

GLuint ComputeShader::CreateShader(std::string& path, GLuint shader_type)
{

	std::ifstream in(path);
	if (in.fail()) {
		std::cout << "[SHADER][CreateShader][ERROR] Failed opening file: " + path << std::endl;
		return -1;
	}

	const std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	const char* shader_source = contents.c_str();

	GLuint shader;
	shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &shader_source, NULL);
	glCompileShader(shader);

	GLuint err = CheckForErrors(shader, GL_COMPILE_STATUS);
	if (err)
		return -1;

	return shader;
}

GLuint ComputeShader::CheckForErrors(GLuint shader, GLuint err_type)
{

	int success;
	char info_log[512];

	if (err_type == GL_LINK_STATUS) {
		glGetProgramiv(shader, err_type, &success);
		glGetProgramInfoLog(shader, 512, NULL, info_log);
	}
	else {
		glGetShaderiv(shader, err_type, &success);
		glGetShaderInfoLog(shader, 512, NULL, info_log);
	}

	if (!success) {
		switch (err_type) {
		case GL_COMPILE_STATUS:
			std::cout << "[Shader][CheckForErrors][ERROR] Failed compiling shader program.\n" << info_log << std::endl;
			break;
		case GL_LINK_STATUS:
			std::cout << "[Shader][CheckForErrors][ERROR] Failed linking shader program.\n" << info_log << std::endl;
			break;
		default:
			std::cout << "[Shader][CheckForErrors][ERROR] Whatever happened was not supposed to happen.\n" << std::endl;
			break;
		}
		return -1;
	}

	return 0;

}