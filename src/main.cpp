#include <iostream>
#include <stdexcept>
#include <optional>

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.hpp"
#include "Program.hpp"

struct Vertex {
    glm::vec2 pos;
    glm::vec2 texCoord;
};

class Application {
public:
    Application(GLFWwindow* window) {
        m_window = window;
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, resizeCallback);
    }

    ~Application() {
        glfwSetFramebufferSizeCallback(m_window, nullptr);
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
    }

    void init() {
        glCreateBuffers(1, &m_vbo);
        glNamedBufferData(m_vbo, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glCreateVertexArrays(1, &m_vao);
        glVertexArrayVertexBuffer(m_vao, 0, m_vbo, 0, sizeof(Vertex));
        glEnableVertexArrayAttrib(m_vao, 0);
        glEnableVertexArrayAttrib(m_vao, 1);
        glVertexArrayAttribFormat(m_vao, 0, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
        glVertexArrayAttribFormat(m_vao, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texCoord));
        glVertexArrayAttribBinding(m_vao, 0, 0);
        glVertexArrayAttribBinding(m_vao, 1, 0);

        Shader::includeShader("/include/ray.glsl", readFile("shaders/ray.glsl").c_str());

        VertexShader vertexShader(readFile("shaders/vert.glsl").c_str());
        FragmentShader fragmentShader(readFile("shaders/frag.glsl").c_str());
        m_program = Program(vertexShader, fragmentShader);

        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);
        m_program->getUniform("aspectRatio") = static_cast<float>(width) / height;

        m_program->getUniform("camRight") = glm::vec3(1.0f, 0.0f, 0.0f);
        m_program->getUniform("camUp") = glm::vec3(0.0f, 1.0f, 0.0f);
        m_program->getUniform("camFront") = glm::vec3(0.0f, 0.0f, 1.0f);
    }

    void loop() {
        while (!glfwWindowShouldClose(m_window)) {
            glfwPollEvents();

            m_program->bind();
            glBindVertexArray(m_vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            glfwSwapBuffers(m_window);
        }
    }

    static void resizeCallback(GLFWwindow* window, int width, int height) {
        Application* self = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
        glViewport(0, 0, width, height);
        self->m_program->getUniform("aspectRatio") = static_cast<float>(width) / height;
    }
private:
    GLFWwindow* m_window;
    GLuint m_vbo;
    GLuint m_vao;
    std::optional<Program> m_program;
    float m_aspectRatio;

    static Vertex vertices[3];
};

Vertex Application::vertices[] = {
    { { -1,  1 }, { 0, 0 } },
    { { -1, -3 }, { 0, 2 } },
    { {  3,  1 }, { 2, 0 } }
};

int main() {
    if (glfwInit() == GL_FALSE) {
        std::cerr << "Failed to initialize GLFW\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
    GLFWwindow* window = glfwCreateWindow(800, 600, "Ray Tracing Now!", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window\n";
        return 1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to load OpenGL\n";
        return 1;
    }

    try {
        Application app(window);
        app.init();
        app.loop();
    } catch (std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}