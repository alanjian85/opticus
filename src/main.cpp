#include <iostream>
#include <stdexcept>
#include <optional>

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.hpp"
#include "Program.hpp"
#include "ScreenQuad.hpp"

class Application {
public:
    Application(GLFWwindow* window) {
        m_window = window;
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, resizeCallback);
    }

    ~Application() {
        glfwSetFramebufferSizeCallback(m_window, nullptr);
    }

    void init() {
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
            m_screenQuad.draw();

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
    std::optional<Program> m_program;
    ScreenQuad m_screenQuad;
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