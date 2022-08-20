#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

class Camera {
public:
    Camera() : m_yaw(-90.0f), m_pitch(0.0f), m_fov(90.0f) { update(); }

    float getYaw() const { return m_yaw; }
    float getPitch() const { return m_pitch; }
    float getFov() const { return m_fov; }

    void setYaw(float yaw) { m_yaw = yaw; update(); }
    void setPitch(float pitch) { m_pitch = pitch; update(); }
    void setFov(float fov) { m_fov = fov; }

    glm::vec3 getRight() const { return m_right; }
    glm::vec3 getUp() const { return m_up; }
    glm::vec3 getFront() const { return m_front; }

    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);

private:
    void update();

private:
    float m_yaw, m_pitch;
    float m_fov;

    glm::vec3 m_right, m_up, m_front;
};

#endif // CAMERA_HPP