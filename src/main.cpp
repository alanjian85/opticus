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
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }

    void init() {
        glCreateBuffers(1, &vbo);
        glNamedBufferData(vbo, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glCreateVertexArrays(1, &vao);
        glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex));
        glEnableVertexArrayAttrib(vao, 0);
        glEnableVertexArrayAttrib(vao, 1);
        glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
        glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texCoord));
        glVertexArrayAttribBinding(vao, 0, 0);
        glVertexArrayAttribBinding(vao, 1, 0);

        Shader::includeShader("/include/ray.glsl", readFile("shaders/ray.glsl").c_str());

        VertexShader vertexShader(readFile("shaders/vert.glsl").c_str());
        FragmentShader fragmentShader(readFile("shaders/frag.glsl").c_str());
        m_program = Program(vertexShader, fragmentShader);

        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);
        m_program->getUniform("aspectRatio") = static_cast<float>(width) / height;
    }

    void loop() {
        while (!glfwWindowShouldClose(m_window)) {
            glfwPollEvents();

            m_program->bind();
            glBindVertexArray(vao);
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
    GLuint vbo;
    GLuint vao;
    std::optional<Program> m_program;

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