#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

struct Camera {
    glm::vec3 getRight() {
        return glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), getFront()));
    }
    
    glm::vec3 getFront() {
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        return front;
    }

    glm::vec3 getUp() {
        return glm::normalize(glm::cross(getFront(), getRight()));
    }

    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    float yaw = 90.0f;
    float pitch = 0.0f;
};

#endif // CAMERA_HPP