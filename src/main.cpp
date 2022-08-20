#include <iostream>
#include <stdexcept>
#include <optional>

#ifdef _WIN32
#include <windows.h>
#endif
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.hpp"
#include "Program.hpp"
#include "ScreenMesh.hpp"
#include "Camera.hpp"
#include "Cubemap.hpp"
#include "Framebuffer.hpp"
#include "shader_list.h"

#ifdef _WIN32
extern "C" {
    _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
#endif

class Application {
public:
    Application(GLFWwindow* window) {
        m_window = window;
        m_camera.pos = glm::vec3(0.0f, 0.0f, 3.0f);
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
        m_samples = 0;
        m_maxSamples = INT_MAX;

        glEnable(GL_FRAMEBUFFER_SRGB);
        
        for (const char* header : shaderHeaders) {
            Shader::includeShader(
                (std::string("/include/") + header).c_str(),
                readFile((std::string("shaders/") + header).c_str()).c_str()
            );
        }

        VertexShader vertexShader(readFile("shaders/vert.glsl").c_str());
        FragmentShader fragmentShader(readFile("shaders/frag.glsl").c_str());
        FragmentShader accumShader(readFile("shaders/accum.glsl").c_str());
        m_tracerProgram = Program(vertexShader, fragmentShader);
        m_accumProgram = Program(vertexShader, accumShader);

        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);
        m_tracerProgram->getUniform("aspectRatio") = static_cast<float>(width) / height;
        m_tracerProgram->getUniform("screenWidth") = width;
        m_tracerProgram->getUniform("screenHeight") = height;
        m_tracerProgram->getUniform("fov") = 90.0f;
        m_framebuffer = Framebuffer(width, height);

        m_tracerProgram->getUniform("camRight") = m_camera.getRight();
        m_tracerProgram->getUniform("camUp") = m_camera.getUp();
        m_tracerProgram->getUniform("camFront") = m_camera.getFront();

        Image rightImage("textures/skybox/right.jpg");
        Image leftImage("textures/skybox/left.jpg");
        Image topImage("textures/skybox/top.jpg");
        Image bottomImage("textures/skybox/bottom.jpg");
        Image frontImage("textures/skybox/front.jpg");
        Image backImage("textures/skybox/back.jpg");
        m_cubemap = Cubemap(rightImage.getWidth(), rightImage.getHeight(), rightImage.getChannels());
        m_cubemap->setRightFace(rightImage);
        m_cubemap->setLeftFace(leftImage);
        m_cubemap->setTopFace(topImage);
        m_cubemap->setBottomFace(bottomImage);
        m_cubemap->setFrontFace(frontImage);
        m_cubemap->setBackFace(backImage);
    }

    void resetDivisor() {
        m_framebuffer->bind();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        m_samples = 0;
    }

    void loop() {
        float lastTime = static_cast<float>(glfwGetTime());
        unsigned int frame = 0;
        while (!glfwWindowShouldClose(m_window)) {
            glfwPollEvents();

            if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_TRUE) {
                glfwSetWindowShouldClose(m_window, GLFW_TRUE);
            }

            const float currTime = static_cast<float>(glfwGetTime());
            const float delta = currTime - lastTime;
            lastTime = currTime;

            updateCameraPosition(delta);

            if (m_samples < m_maxSamples) {
                glEnable(GL_BLEND);
                glBlendFunc(GL_ONE, GL_ONE);
                m_framebuffer->bind();
                m_tracerProgram->getUniform("frame") = frame++;
                m_cubemap->bindUnit(0);
                m_tracerProgram->getUniform("skybox") = 0;
                m_tracerProgram->bind();
                m_screenMesh.draw();
                ++m_samples;
            }

            glDisable(GL_BLEND);
            Framebuffer::bindDefault();
            m_framebuffer->bindTexture(0, 0);
            m_accumProgram->getUniform("accumBuffer") = 0;
            m_accumProgram->getUniform("samples") = m_samples;
            m_accumProgram->bind();
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

        m_tracerProgram->getUniform("camRight") = m_camera.getRight();
        m_tracerProgram->getUniform("camUp") = m_camera.getUp();
        m_tracerProgram->getUniform("camFront") = m_camera.getFront();
    }

    void updateCameraPosition(float delta) {
        const float speed = 2.5f;

        if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_TRUE) {
            m_camera.pos += m_camera.getFront() * speed * delta;
            resetDivisor();
        }

        if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_TRUE) {
            m_camera.pos -= m_camera.getFront() * speed * delta;
            resetDivisor();
        }

        if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_TRUE) {
            m_camera.pos += m_camera.getRight() * speed * delta;
            resetDivisor();
        }

        if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_TRUE) {
            m_camera.pos -= m_camera.getRight() * speed * delta;
            resetDivisor();
        }

        m_tracerProgram->getUniform("camPos") = m_camera.pos;
    }

    static void resizeCallback(GLFWwindow* window, int width, int height) {
        Application* self = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
        glViewport(0, 0, width, height);
        self->resetDivisor();
        self->m_framebuffer->resize(width, height);
        self->m_tracerProgram->getUniform("aspectRatio") = static_cast<float>(width) / height;
        self->m_tracerProgram->getUniform("screenWidth") = width;
        self->m_tracerProgram->getUniform("screenHeight") = height;
    }

    static void cursorCallback(GLFWwindow* window, double xpos, double ypos) {
        static double lastx = xpos, lasty = ypos;
        
        Application* self = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
        float xoffset = static_cast<float>(xpos - lastx), yoffset = static_cast<float>(lasty - ypos);
        
        self->resetDivisor();
        self->updateCameraDirection(xoffset, yoffset);

        lastx = xpos;
        lasty = ypos;
    }
private:
    GLFWwindow* m_window;
    ScreenMesh m_screenMesh;
    std::optional<Program> m_tracerProgram;
    std::optional<Program> m_accumProgram;
    std::optional<Cubemap> m_cubemap;
    std::optional<Framebuffer> m_framebuffer;

    Camera m_camera;
    int m_samples;
    int m_maxSamples;
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
    GLFWwindow* window = glfwCreateWindow(1024, 720, "Ray Tracing Now!", nullptr, nullptr);
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