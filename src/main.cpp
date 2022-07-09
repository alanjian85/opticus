#include <iostream>
#include <stdexcept>
#include <optional>

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.hpp"
#include "Program.hpp"
#include "ScreenMesh.hpp"
#include "Camera.hpp"

class Application {
public:
    Application(GLFWwindow* window) {
        m_window = window;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, resizeCallback);
        glfwSetCursorPosCallback(window, cursorCallback);
    }

    ~Application() {
        glfwSetFramebufferSizeCallback(m_window, nullptr);
        glfwSetCursorPosCallback(m_window, nullptr);
    }

    void init() {
        glEnable(GL_SAMPLE_SHADING);
        glMinSampleShading(1.0f);

        Shader::includeShader("/include/ray.glsl", readFile("shaders/ray.glsl").c_str());
        Shader::includeShader("/include/sphere.glsl", readFile("shaders/sphere.glsl").c_str());

        VertexShader vertexShader(readFile("shaders/vert.glsl").c_str());
        FragmentShader fragmentShader(readFile("shaders/frag.glsl").c_str());
        m_program = Program(vertexShader, fragmentShader);

        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);
        m_program->getUniform("aspectRatio") = static_cast<float>(width) / height;
        m_program->getUniform("fov") = glm::radians(45.0f);

        m_program->getUniform("camRight") = m_camera.getRight();
        m_program->getUniform("camUp") = m_camera.getUp();
        m_program->getUniform("camFront") = m_camera.getFront();
    }

    void loop() {
        float lastTime = static_cast<float>(glfwGetTime());
        while (!glfwWindowShouldClose(m_window)) {
            glfwPollEvents();

            if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_TRUE) {
                glfwSetWindowShouldClose(m_window, GLFW_TRUE);
            }

            const float currTime = static_cast<float>(glfwGetTime());
            const float delta = currTime - lastTime;
            lastTime = currTime;

            updateCameraPosition(delta);

            m_program->bind();
            m_screenMesh.draw();

            glfwSwapBuffers(m_window);
        }
    }

    void updateCameraDirection(float xoffset, float yoffset) {
        const float sensitivity = 0.1f;

        xoffset *= sensitivity;
        yoffset *= sensitivity;

        m_camera.setYaw(m_camera.getYaw() + xoffset);
        m_camera.setPitch(m_camera.getPitch() + yoffset);

        m_camera.setPitch(std::clamp(m_camera.getPitch(), -89.0f, 89.0f));

        m_program->getUniform("camRight") = m_camera.getRight();
        m_program->getUniform("camUp") = m_camera.getUp();
        m_program->getUniform("camFront") = m_camera.getFront();
    }

    void updateCameraPosition(float delta) {
        const float speed = 2.5f;

        if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_TRUE) {
            m_camera.pos += m_camera.getFront() * speed * delta;

        }

        if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_TRUE) {
            m_camera.pos -= m_camera.getFront() * speed * delta;
        }

        if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_TRUE) {
            m_camera.pos += m_camera.getRight() * speed * delta;
        }

        if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_TRUE) {
            m_camera.pos -= m_camera.getRight() * speed * delta;
        }

        m_program->getUniform("camPos") = m_camera.pos;
    }

    static void resizeCallback(GLFWwindow* window, int width, int height) {
        Application* self = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
        glViewport(0, 0, width, height);
        self->m_program->getUniform("aspectRatio") = static_cast<float>(width) / height;
    }

    static void cursorCallback(GLFWwindow* window, double xpos, double ypos) {
        static double lastx = xpos, lasty = ypos;
        
        Application* self = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
        float xoffset = static_cast<float>(xpos - lastx), yoffset = static_cast<float>(lasty - ypos);

        self->updateCameraDirection(xoffset, yoffset);

        lastx = xpos;
        lasty = ypos;
    }
private:
    GLFWwindow* m_window;
    std::optional<Program> m_program;
    ScreenMesh m_screenMesh;

    Camera m_camera;
};

int main() {
    if (glfwInit() == GL_FALSE) {
        std::cerr << "Failed to initialize GLFW\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 100);
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