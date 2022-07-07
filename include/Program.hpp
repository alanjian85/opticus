#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <glad/glad.h>

#include "Shader.hpp"

class Program {
public:
    Program() : m_program(0) {}

    Program(const Shader& vert, const Shader& frag) {
        m_program = glCreateProgram();
        glAttachShader(m_program, vert.getShader());
        glAttachShader(m_program, frag.getShader());
        link();
    }

    Program(Program&& other) noexcept {
        m_program = std::exchange(other.m_program, 0);
    }

    Program& operator=(Program&& other) noexcept {
        glDeleteProgram(m_program);
        m_program = std::exchange(other.m_program, 0);
        return *this;
    }

    ~Program() {
        glDeleteProgram(m_program);
    }

    void bind() const {
        glUseProgram(m_program);
    }

private:
    void link() {
        glLinkProgram(m_program);
        GLint linkStatus;
        glGetProgramiv(m_program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus == GL_FALSE) {
            GLint logLength;
            glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logLength);
            std::string log(logLength, ' ');
            glGetProgramInfoLog(m_program, logLength, nullptr, log.data());
            throw std::runtime_error("Failed to link program: \n" + log);
        }
    }

private:
    GLuint m_program;
};

#endif // PROGRAM_HPP