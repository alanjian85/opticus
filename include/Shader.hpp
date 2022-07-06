#ifndef SHADER_HPP
#define SHADER_HPP

#include <algorithm>
#include <stdexcept>
#include <string>

#include <glad/glad.h>

class Shader {
public:
    Shader() : m_shader(0) {

    }

    Shader(GLenum type, const char* source) {
        m_shader = glCreateShader(type);
        glShaderSource(m_shader, 1, &source, nullptr);
        glCompileShader(m_shader);
        GLint compileStatus;
        glGetShaderiv(m_shader, GL_COMPILE_STATUS, &compileStatus);
        if (compileStatus == GL_FALSE) {
            GLint logLength;
            glGetShaderiv(m_shader, GL_INFO_LOG_LENGTH, &logLength);
            std::string log(logLength, ' ');
            glGetShaderInfoLog(m_shader, logLength, nullptr, log.data());
            throw std::runtime_error("Failed to compile shader: " + log);
        }
    }

    Shader(Shader&& other) noexcept {
        m_shader = std::exchange(other.m_shader, 0);
    }

    Shader& operator=(Shader&& other) noexcept {
        glDeleteShader(m_shader);
        m_shader = std::exchange(other.m_shader, 0);
    }

    ~Shader() {
        glDeleteShader(m_shader);
    }

    GLuint getShader() {
        return m_shader;
    }

private:
    GLuint m_shader;
};

class VertexShader : public Shader {
public:
    VertexShader() = default;

    VertexShader(const char* source) : Shader(GL_VERTEX_SHADER, source) {}
};

class FragmentShader : public Shader {
public:
    FragmentShader() = default;

    FragmentShader(const char* source) : Shader(GL_FRAGMENT_SHADER, source) {}
};

#endif // SHADER_HPP