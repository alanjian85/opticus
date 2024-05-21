#include "Shader.hpp"

Shader::Shader(GLenum type, const char* source) {
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
