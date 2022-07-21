#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include <algorithm>
#include <vector>
#include <stdexcept>

#include <glad/glad.h>

class Framebuffer {
public:
    Framebuffer(int width, int height) {
        m_textures.resize(1);
        glCreateFramebuffers(1, &m_framebuffer);
        glCreateTextures(GL_TEXTURE_2D, static_cast<GLsizei>(m_textures.size()), m_textures.data());
        glTextureStorage2D(m_textures[0], 1, GL_RGB32F, width, height);
        glNamedFramebufferTexture(m_framebuffer, GL_COLOR_ATTACHMENT0, m_textures[0], 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error("The framebuffer is not completed");
        }
    }

    Framebuffer(Framebuffer&& other) noexcept {
        m_framebuffer = std::exchange(other.m_framebuffer, 0);
        m_textures = std::move(other.m_textures);
    }

    Framebuffer& operator=(Framebuffer&& other) noexcept {
        glDeleteTextures(static_cast<GLsizei>(m_textures.size()), m_textures.data());
        glDeleteFramebuffers(1, &m_framebuffer);
        m_framebuffer = std::exchange(other.m_framebuffer, 0);
        m_textures = std::move(other.m_textures);
        return *this;
    }

    ~Framebuffer() {
        glDeleteTextures(static_cast<GLsizei>(m_textures.size()), m_textures.data());
        glDeleteFramebuffers(1, &m_framebuffer);
    }

    void resize(int width, int height) {
        glDeleteTextures(static_cast<GLsizei>(m_textures.size()), m_textures.data());
        glCreateTextures(GL_TEXTURE_2D, static_cast<GLsizei>(m_textures.size()), m_textures.data());
        glTextureStorage2D(m_textures[0], 1, GL_RGB32F, width, height);
        glNamedFramebufferTexture(m_framebuffer, GL_COLOR_ATTACHMENT0, m_textures[0], 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error("The framebuffer is not completed");
        }
    }

    void bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    }

    void bindTexture(unsigned int unit, unsigned int attachment) {
        glBindTextureUnit(unit, m_textures[attachment]);
    }

    static void bindDefault() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

private:
    GLuint m_framebuffer;
    std::vector<GLuint> m_textures;
};

#endif // FRAMEBUFFER_HPP