#ifndef CUBEMAP_HPP
#define CUBEMAP_HPP

#include <glad/glad.h>

#include "Image.hpp"

class Cubemap {
public:
    Cubemap(int width, int height, int channels);

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
    void uploadTexture(GLint face, const Image& image);

private:
    GLuint m_texture;
};

#endif