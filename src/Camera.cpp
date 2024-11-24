//
// Created by ivang on 26.10.2024.
//

#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : front(glm::vec3(0.0f, 0.0f, -1.0f)), worldUp(up), yaw(yaw), pitch(pitch) {
    this->position = position;
    this->movementSpeed = 2.5f;
    this->mouseSensitivity = 0.1f;
    updateCameraVectors();
}

void Camera::processKeyboard(const CameraMovement direction, const float deltaTime) {
    const float velocity = movementSpeed * deltaTime;
    if (direction == FORWARD)
        position += front * velocity;
    if (direction == BACKWARD)
        position -= front * velocity;
    if (direction == LEFT)
        position -= right * velocity;
    if (direction == RIGHT)
        position += right * velocity;
}

void Camera::processMouseMovement(float xOffset, float yOffset) {
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    yaw += xOffset;
    pitch += yOffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->front = glm::normalize(front);
    right = glm::normalize(glm::cross(this->front, worldUp)); // Нормалізуємо вектор праворуч
    up = glm::normalize(glm::cross(right, this->front));
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}
