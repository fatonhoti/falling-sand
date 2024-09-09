#include "Shader.hpp"

Shader::~Shader() {
    glDeleteShader(m_shader_program);
}

Shader::Shader(std::string shader_name, ShaderType type) : m_shader_type(type) {
    auto base_dir = fs::current_path().append("src").append("shaders").string();
    GLuint shader_program = glCreateProgram();

    if (type == ShaderType::GRAPHICS) {
        auto path_vert = base_dir + "\\" + shader_name + ".vert";
        auto path_frag = base_dir + "\\" + shader_name + ".frag";

        GLuint v_shader = CreateShader(path_vert, GL_VERTEX_SHADER);
        if (v_shader == -1) {
            std::cout << "[Shader][Constructor][ERROR] COULD_NOT_CREATE_VERTEX_SHADER\n";
            exit(1); // just bail
        }

        GLuint f_shader = CreateShader(path_frag, GL_FRAGMENT_SHADER);
        if (f_shader == -1) {
            std::cout << "[Shader][Constructor][ERROR] COULD_NOT_CREATE_FRAGMENT_SHADER\n";
            exit(1); // just bail
        }

        glAttachShader(shader_program, v_shader);
        glAttachShader(shader_program, f_shader);

        glDeleteShader(v_shader);
        glDeleteShader(f_shader);
    }
    else if (type == ShaderType::COMPUTE) {
        auto path_comp = base_dir + "\\" + shader_name + ".comp";

        GLuint c_shader = CreateShader(path_comp, GL_COMPUTE_SHADER);
        if (c_shader == -1) {
            std::cout << "[Shader][Constructor][ERROR] COULD_NOT_CREATE_COMPUTE_SHADER\n";
            exit(1); // just bail
        }

        glAttachShader(shader_program, c_shader);

        glDeleteShader(c_shader);
    }

    glLinkProgram(shader_program);

    if (CheckForErrors(shader_program, GL_LINK_STATUS) == -1) {
        std::cout << "[Shader][Constructor][ERROR] COULD_NOT_LINK_SHADER: " + shader_name << "\n";
        exit(1); // just bail
    }

    m_shader_program = shader_program;
}

void Shader::Bind() const {
    glUseProgram(m_shader_program);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

GLuint Shader::GetShaderProgram() const {
    return m_shader_program;
}

void Shader::SetMat4x4(const glm::mat4x4& mat, const std::string& uniform_name) {
    auto uniform_location = GetUniformLoc(uniform_name);
    if (uniform_location != -1) {
        glUniformMatrix4fv(uniform_location, 1, GL_FALSE, glm::value_ptr(mat));
    }
}

void Shader::SetVec4(const glm::vec4& vec, const std::string& uniform_name) {
    auto uniform_location = GetUniformLoc(uniform_name);
    if (uniform_location != -1) {
        glUniform4fv(uniform_location, 1, glm::value_ptr(vec));
    }
}

void Shader::SetVec3(const glm::vec3& vec, const std::string& uniform_name) {
    auto uniform_location = GetUniformLoc(uniform_name);
    if (uniform_location != -1) {
        glUniform3fv(uniform_location, 1, glm::value_ptr(vec));
    }
}

void Shader::SetVec2(const glm::vec2& vec, const std::string& uniform_name) {
    auto uniform_location = GetUniformLoc(uniform_name);
    if (uniform_location != -1) {
        glUniform2fv(uniform_location, 1, glm::value_ptr(vec));
    }
}

void Shader::SetInteger1(const int& integer, const std::string& uniform_name) {
    auto uniform_location = GetUniformLoc(uniform_name);
    if (uniform_location != -1) {
        glUniform1i(uniform_location, integer);
    }
}

void Shader::SetFloat1(const float& float_, const std::string& uniform_name) {
    auto uniform_location = GetUniformLoc(uniform_name);
    if (uniform_location != -1) {
        glUniform1f(uniform_location, float_);
    }
}

GLint Shader::GetUniformLoc(const std::string& uniform_name) {
    auto it = m_UniformCache.find(uniform_name);
    if (it == m_UniformCache.end()) {
        GLint loc = glGetUniformLocation(m_shader_program, uniform_name.c_str());
        if (loc == -1) {
            std::cout << "[Shader][GetUniformLoc][ERROR] Could not find location for uniform '" + uniform_name + "'.\n";
            return -1;
        }
        m_UniformCache[uniform_name] = loc;
        return loc;
    }

    return it->second;
}

GLuint Shader::CreateShader(std::string& path, GLuint shader_type) {
    std::ifstream in(path);
    if (in.fail()) {
        std::cout << "[Shader][CreateShader][ERROR] Failed opening file: " + path << "\n";
        return -1;
    }

    const std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    const char* shader_source = contents.c_str();

    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_source, NULL);
    glCompileShader(shader);

    if (CheckForErrors(shader, GL_COMPILE_STATUS))
        return -1;

    return shader;
}

GLuint Shader::CheckForErrors(GLuint shader, GLuint err_type) {
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
            std::cout << "[Shader][CheckForErrors][ERROR] Failed compiling shader.\n" << info_log << "\n";
            break;
        case GL_LINK_STATUS:
            std::cout << "[Shader][CheckForErrors][ERROR] Failed linking shader.\n" << info_log << "\n";
            break;
        default:
            std::cout << "[Shader][CheckForErrors][ERROR] Unexpected error.\n" << "\n";
            break;
        }
        return -1;
    }

    return 0;
}
