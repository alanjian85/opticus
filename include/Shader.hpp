#ifndef SHADER_HPP
#define SHADER_HPP

#include <algorithm>
#include <stdexcept>
#include <string>
#include <fstream>

#include <glad/glad.h>

inline std::string readFile(const char* path) {
    std::ifstream file(path, std::ios::ate);
    if (!file.is_open()) {
        throw std::runtime_error(std::string("Failed to open file: ") + path);
    }
    size_t size = file.tellg();
    std::string buffer(size, ' ');
    file.seekg(0);
    file.read(buffer.data(), size);
    return buffer;
}

class Shader {
public:
    Shader() : m_shader(0) { }

    Shader(GLenum type, const char* source) {
        m_shader = glCreateShader(type);
        if (m_shader == 0) {
            throw std::runtime_error("An error occurs while creating the shader object");
        }
        glShaderSource(m_shader, 1, &source, nullptr);
        glCompileShader(m_shader);
        GLint compileStatus;
        glGetShaderiv(m_shader, GL_COMPILE_STATUS, &compileStatus);
        if (compileStatus == GL_FALSE) {
            GLint logLength;
            glGetShaderiv(m_shader, GL_INFO_LOG_LENGTH, &logLength);
            std::string log(logLength, ' ');
            glGetShaderInfoLog(m_shader, logLength, nullptr, log.data());
            throw std::runtime_error("Failed to compile shader: \n" + log);
        }
    }

    Shader(Shader&& other) noexcept {
        m_shader = std::exchange(other.m_shader, 0);
    }

    Shader& operator=(Shader&& other) noexcept {
        glDeleteShader(m_shader);
        m_shader = std::exchange(other.m_shader, 0);
        return *this;
    }

    ~Shader() {
        glDeleteShader(m_shader);
    }

    GLuint getShader() const {
        return m_shader;
    }

    static void includeShader(const char* name, const char* source) {
        glNamedStringARB(GL_SHADER_INCLUDE_ARB, -1, name, -1, source);
    }

private:
    GLuint m_shader;
};

class VertexShader : public Shader {
public:
    VertexShader() = default;

    explicit VertexShader(const char* source) : Shader(GL_VERTEX_SHADER, source) {}
};

class FragmentShader : public Shader {
public:
    FragmentShader() = default;

    explicit FragmentShader(const char* source) : Shader(GL_FRAGMENT_SHADER, source) {}
};

#endif // SHADER_HPP