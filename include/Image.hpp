#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <algorithm>
#include <stdexcept>

#include <stb_image.h>

class Image {
public:
    Image(const char* path, int channels = 0) {
        m_pixels = stbi_load(path, &m_width, &m_height, &m_channels, channels);
        if (!m_pixels) {
            throw std::runtime_error(std::string("Failed to load image file: ") + path);
        }
    }

    Image(Image&& other) noexcept {
        m_width = other.m_width;
        m_height = other.m_height;
        m_channels = other.m_channels;
        m_pixels = std::exchange(other.m_pixels, nullptr);
    }

    Image& operator=(Image&& other) noexcept {
        stbi_image_free(m_pixels);
        m_width = other.m_width;
        m_height = other.m_height;
        m_channels = other.m_channels;
        m_pixels = std::exchange(other.m_pixels, nullptr);
        return *this;
    }

    ~Image() {
        stbi_image_free(m_pixels);
    }

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    int getChannels() const { return m_channels; }
    unsigned char* getPixels() const { return m_pixels; }

private:
    int m_width, m_height, m_channels;
    unsigned char* m_pixels;
};

#endif // IMAGE_HPP
