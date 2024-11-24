//
// Created by ivang on 26.10.2024.
//

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera {
public:
    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);
    void processKeyboard(CameraMovement direction, float deltaTime);
    void processMouseMovement(float xOffset, float yOffset);
    void updateCameraVectors();

    glm::mat4 getViewMatrix() const;

private:
    glm::vec3 position{};
    glm::vec3 front;
    glm::vec3 up{};
    glm::vec3 right{};
    glm::vec3 worldUp;

    float yaw;
    float pitch;
    float movementSpeed;
    float mouseSensitivity;
};

#endif //CAMERA_H
