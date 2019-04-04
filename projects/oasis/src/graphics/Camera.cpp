#include "../Precompiled.h"

Camera::Camera()
{
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
    position = cfg->getParameter("Camera", "position").toFvec3();
    aspectRatio = cfg->getScreenAspectRatio();
    upVector = cfg->getParameter("Camera", "upVector").toFvec3();
    lookTarget = cfg->getParameter("Camera", "lookAt").toFvec3();
}
