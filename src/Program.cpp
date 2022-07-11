#include "Program.hpp"

void Program::link() {
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