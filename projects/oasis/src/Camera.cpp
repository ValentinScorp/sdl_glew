#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(float ar, glm::vec3 pos, glm::vec3 upVec, glm::vec3 lookAt) {
    aspectRatio = ar;
    position = pos;
    upVector = upVec;
    lookTarget = lookAt;
}

Camera::~Camera() {
}

glm::mat4 Camera::getViewMatrix() {
    return glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
}

glm::mat4 Camera::getCamMatrix() {
    return glm::lookAt(position, lookTarget, upVector);
}

void Camera::moveUp() {
    position.y += moveSpeed;
    lookTarget.y += moveSpeed;
}

void Camera::moveDown() {
    position.y -= moveSpeed;
    lookTarget.y -= moveSpeed;
}

void Camera::init(Configuration* cfg) {
    position = cfg->camPosition;
}
