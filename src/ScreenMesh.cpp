#include "ScreenMesh.hpp"

#include <algorithm>

ScreenMesh::ScreenMesh() {
    glCreateBuffers(1, &m_vbo);
    glNamedBufferData(m_vbo, sizeof(s_vertices), s_vertices, GL_STATIC_DRAW);

    glCreateVertexArrays(1, &m_vao);
    glVertexArrayVertexBuffer(m_vao, 0, m_vbo, 0, sizeof(Vertex));
    glEnableVertexArrayAttrib(m_vao, 0);
    glEnableVertexArrayAttrib(m_vao, 1);
    glVertexArrayAttribFormat(m_vao, 0, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
    glVertexArrayAttribFormat(m_vao, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texCoord));
    glVertexArrayAttribBinding(m_vao, 0, 0);
    glVertexArrayAttribBinding(m_vao, 1, 0);
}

ScreenMesh::ScreenMesh(ScreenMesh&&) noexcept {
    m_vao = std::exchange(m_vao, 0);
    m_vbo = std::exchange(m_vbo, 0);
}

ScreenMesh& ScreenMesh::operator=(ScreenMesh&& other) noexcept {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    m_vao = std::exchange(m_vao, 0);
    m_vbo = std::exchange(m_vbo, 0);
    return *this;
}

ScreenMesh::~ScreenMesh() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
}

void ScreenMesh::draw() {
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

Vertex ScreenMesh::s_vertices[3] = {
    { { -1,  3 }, { 0, 2 } },
    { { -1, -1 }, { 0, 0 } },
    { {  3, -1 }, { 2, 0 } }
};