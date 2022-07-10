#ifndef CUBEMAP_HPP
#define CUBEMAP_HPP

#include <glad/glad.h>

#include "Image.hpp"

class Cubemap {
public:
    Cubemap(int width, int height, int channels) {
        glCreateTextures(GL_TEXTURE_CUBE_MAP_ARRAY, 1, &m_texture);
        if (m_texture == 0) {
            throw std::runtime_error("An error occurs while creating the cubemap texture");
        }

        GLenum format;
        switch (channels) {
            case 1:
                format = GL_R8;
                break;
            case 3:
                format = GL_SRGB8;
                break;
            case 4:
                format = GL_SRGB8_ALPHA8;
                break;
        }
        glTextureStorage3D(m_texture, 1, format, width, height, 6);
        glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    Cubemap(Cubemap&& other) noexcept {
        m_texture = std::exchange(other.m_texture, 0);
    }

    Cubemap& operator=(Cubemap&& other) noexcept {
        glDeleteTextures(1, &m_texture);
        m_texture = std::exchange(other.m_texture, 0);
        return *this;
    }

    ~Cubemap() {
        glDeleteTextures(1, &m_texture);
    }

    void setRightFace(const Image& image) { uploadTexture(0, image); }
    void setLeftFace(const Image& image) { uploadTexture(1, image); }
    void setTopFace(const Image& image) { uploadTexture(2, image); }
    void setBottomFace(const Image& image) { uploadTexture(3, image); }
    void setFrontFace(const Image& image) { uploadTexture(4, image); }
    void setBackFace(const Image& image) { uploadTexture(5, image); }

    void bindUnit(unsigned int unit) {
        glBindTextureUnit(unit, m_texture);
    }

private:
    void uploadTexture(GLint face, const Image& image) {
        GLenum format;
        switch (image.getChannels()) {
            case 1:
                format = GL_RED;
                break;
            case 3:
                format = GL_RGB;
                break;
            case 4:
                format = GL_RGBA;
                break;
        }
        glTextureSubImage3D(m_texture, 0, 0, 0, face, image.getWidth(), image.getHeight(), 1, format, GL_UNSIGNED_BYTE, image.getPixels());
    }

private:
    GLuint m_texture;
};

#endif