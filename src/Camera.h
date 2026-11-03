#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

constexpr float SPEED = 3.0f;

enum class CameraMove { Forward, Backward, Left, Right, Up, Down };

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front{0.0f, 0.0f, -SPEED};
    glm::vec3 up{0.0f, SPEED, 0.0f};
    glm::vec3 right{SPEED, 0.0f, 0.0f};
    float yaw = -90.0f;
    float pitch = 0.0f;
    float speed = SPEED;
    float sensitivity = 0.1f;

    explicit Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f)) : position(pos) {
        updateVectors();
    }

    glm::mat4 viewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }

    void move(CameraMove dir, float dt) {
        float v = speed * dt;
        switch (dir) {
            case CameraMove::Forward:  position += front * v; break;
            case CameraMove::Backward: position -= front * v; break;
            case CameraMove::Left:     position -= right * v; break;
            case CameraMove::Right:    position += right * v; break;
            case CameraMove::Up:       position += glm::vec3(0, 1, 0) * v; break;
            case CameraMove::Down:     position -= glm::vec3(0, 1, 0) * v; break;
        }
    }

    void rotate(float dx, float dy) {
        yaw += dx * sensitivity;
        pitch += dy * sensitivity;
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
        updateVectors();
    }

private:
    void updateVectors() {
        glm::vec3 f;
        f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        f.y = sin(glm::radians(pitch));
        f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(f);
        right = glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
        up = glm::normalize(glm::cross(right, front));
    }
};