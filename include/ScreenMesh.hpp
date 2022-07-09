#ifndef SCREEN_QUAD
#define SCREEN_QUAD

#include <glad/glad.h>
#include <glm/glm.hpp>

struct Vertex {
    glm::vec2 pos;
    glm::vec2 texCoord;
};

class ScreenMesh {
public:
    ScreenMesh();

    ScreenMesh(ScreenMesh&&) noexcept;

    ScreenMesh& operator=(ScreenMesh&& other) noexcept;

    ~ScreenMesh();

    void draw();

private:
    GLuint m_vbo;
    GLuint m_vao;

    static Vertex s_vertices[3];
};

#endif // SCREEN_QUAD