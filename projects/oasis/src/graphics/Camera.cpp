#include "../Precompiled.h"

Camera::Camera(std::shared_ptr<Renderer> renderer) {
    mRenderer = renderer;
}

Camera::~Camera() {
}

void Camera::onMessage(IMessage *message) {
    if (message->getKeyPressed() == "arrow_left") {
        moveLeft();
    }
    if (message->getKeyPressed() == "arrow_right") {
        moveRight();
    }
    if (message->getKeyPressed() == "arrow_up") {
        moveForward();
    }
    if (message->getKeyPressed() == "arrow_down") {
        moveBackward();
    }
}

glm::mat4 Camera::getViewMatrix() {
    return glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
}

glm::mat4 Camera::getCamMatrix() {
    return glm::lookAt(position, lookTarget, upVector);
}

void Camera::moveForward() {
    position.y += moveSpeed;
    lookTarget.y += moveSpeed;
}

void Camera::moveBackward() {
    position.y -= moveSpeed;
    lookTarget.y -= moveSpeed;
}

void Camera::moveLeft() {
    position.x -= moveSpeed;
    lookTarget.x -= moveSpeed;
}

void Camera::moveRight() {
    position.x += moveSpeed;
    lookTarget.x += moveSpeed;
}

void Camera::init(Configuration* cfg) {
    position = cfg->getParameter("Camera", "position").toFvec3();
    aspectRatio = cfg->getScreenAspectRatio();
    upVector = cfg->getParameter("Camera", "upVector").toFvec3();
    lookTarget = cfg->getParameter("Camera", "lookAt").toFvec3();
}
