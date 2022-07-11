#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include <algorithm>
#include <vector>
#include <stdexcept>

#include <glad/glad.h>

#include "Texture2D.hpp"

class Framebuffer {
public:
    Framebuffer(int width, int height) {
        glCreateFramebuffers(1, &m_framebuffer);

        m_textures.emplace_back(width, height);
        glNamedFramebufferTexture(m_framebuffer, GL_COLOR_ATTACHMENT0, m_textures[0].getTexture(), 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error("The framebuffer is not completed");
        }
    }

    Framebuffer(Framebuffer&& other) noexcept {
        m_framebuffer = std::exchange(other.m_framebuffer, 0);
        m_textures = std::move(other.m_textures);
    }

    Framebuffer& operator=(Framebuffer&& other) noexcept {
        glDeleteFramebuffers(1, &m_framebuffer);
        m_framebuffer = std::exchange(other.m_framebuffer, 0);
        m_textures = std::move(other.m_textures);
        return *this;
    }

    ~Framebuffer() {
        glDeleteFramebuffers(1, &m_framebuffer);
    }

    void bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    }

    Texture2D& getTexture(unsigned i) {
        return m_textures[i];
    }

    static void bindDefault() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

private:
    GLuint m_framebuffer;
    std::vector<Texture2D> m_textures;
};

#endif // FRAMEBUFFER_HPP