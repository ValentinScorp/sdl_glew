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
    return glm::perspective(glm::radians(45.0f), aspectRatio, nearPlane, farPlane);
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
    nearPlane = cfg->getParameter("Camera", "nearPlane").toFloat();
    farPlane = cfg->getParameter("Camera", "farPlane").toFloat();
    screenWidth = cfg->getParameter("Window", "width").toFloat();
    screenHeight = cfg->getParameter("Window", "height").toFloat();
}

RayVector Camera::getVectorRay(int x, int y) {
	glm::fvec3 pts[4];
	getPlanePoints(farPlane, pts);
	float dx = (float)x / screenWidth;
	float dz = (float)y / screenHeight;
	glm::fvec3 vdx = pts[1] - pts[0];
	glm::fvec3 vdz = pts[3] - pts[0];
	glm::fvec3 end = pts[0] + (vdx * dx) + (vdz * dz);
    
	glm::mat4 orientation = getCamMatrix();
    glm::fvec4 end1 = orientation * glm::fvec4(end, 1.0);
	return RayVector(position, glm::fvec3(end1.x, end1.y, end1.z));
}

void Camera::getPlanePoints(float dist, glm::fvec3 *pts)
{
	float fov = glm::radians(45.0);

	float y = dist * float(glm::tan(fov * 0.5));
	float x = y * aspectRatio;

	pts[0].x = -x;  pts[0].y = y;   pts[0].z = dist;
	pts[1].x = x;   pts[1].y = y;   pts[1].z = dist;
	pts[2].x = x;   pts[2].y = -y;  pts[2].z = dist;
	pts[3].x = -x;  pts[3].y = -y;  pts[3].z = dist;
}

glm::mat4 Camera::makeOrientationMatrix()
{
	glm::fvec3 cam_pos = position;
	glm::fvec3 cam_rot = glm::fvec3(45.0, 0, 0);

	glm::mat4 mat_rot_x, mat_rot_y, mat_rot_z, mat_trans;
    
    mat_trans = glm::translate(glm::mat4(1.0f), cam_pos);
    mat_rot_x = glm::rotate(glm::mat4(1.0f), cam_rot.x, glm::fvec3(1, 0, 0));
    mat_rot_y = glm::rotate(glm::mat4(1.0f), cam_rot.y, glm::fvec3(0, 1, 0));
    mat_rot_z = glm::rotate(glm::mat4(1.0f), cam_rot.z, glm::fvec3(0, 0, 1));

	glm::mat4 orientation = mat_rot_x * mat_rot_y * mat_rot_z * mat_trans;

	return orientation;
}