#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <cassert>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"

class Uniform {
    friend class Program;
public:
    Uniform(GLuint program, GLuint location) 
        : m_program(program), m_location(location) {
        assert(program != 0);
        assert(location != -1);
    }

    const Uniform& operator=(int value) const {
        glProgramUniform1i(m_program, m_location, value);
        return *this;
    }

    const Uniform& operator=(float value) const {
        glProgramUniform1f(m_program, m_location, value);
        return *this;
    }

    const Uniform& operator=(glm::vec3 value) const {
        glProgramUniform3fv(m_program, m_location, 1, glm::value_ptr(value));
        return *this;
    }
private:
    GLuint m_program;
    GLuint m_location;
};

class Program {
public:
    Program(const Shader& vert, const Shader& frag) {
        create();
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

    Uniform getUniform(const char* name) {
        return Uniform(m_program, glGetUniformLocation(m_program, name));
    }

private:
    void create() {
        m_program = glCreateProgram();
        if (m_program == 0) {
            throw std::runtime_error("An error occurs while creating the program object");
        }
    }

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