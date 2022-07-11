#ifndef TEXTURE_2D_HPP
#define TEXTURE_2D_HPP

#include <glad/glad.h>

class Texture2D {
public:
    Texture2D(int width, int height) {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);
        glTextureStorage2D(m_texture, 1, GL_RGB32F, width, height);
        glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    Texture2D(Texture2D&& other) noexcept {
        m_texture = std::exchange(other.m_texture, 0);
    }

    Texture2D& operator=(Texture2D&& other) noexcept {
        glDeleteTextures(1, &m_texture);
        m_texture = std::exchange(other.m_texture, 0);
        return *this;
    }

    ~Texture2D() {
        glDeleteTextures(1, &m_texture);
    }

    GLuint getTexture() const {
        return m_texture;
    }

    void bindUnit(unsigned int unit) {
        glBindTextureUnit(unit, m_texture);
    }

private:
    GLuint m_texture;
};

#endif // TEXTURE_2D_HPP