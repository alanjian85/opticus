#ifndef SHADER_HPP
#define SHADER_HPP

#include <algorithm>

#include <glad/glad.h>

class Shader {
public:
    Shader() : m_shader(0) {

    }

    Shader(GLenum type) {
        m_shader = glCreateShader(type);
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
    VertexShader() : Shader(GL_VERTEX_SHADER) {

    }
};

class FragmentShader : public Shader {
public:
    FragmentShader() : Shader(GL_FRAGMENT_SHADER) {
    
    }
};

#endif // SHADER_HPP