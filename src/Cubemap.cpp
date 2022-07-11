#include "Cubemap.hpp"

#include <stdexcept>

Cubemap::Cubemap(int width, int height, int channels) {
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

void Cubemap::uploadTexture(GLint face, const Image& image) {
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