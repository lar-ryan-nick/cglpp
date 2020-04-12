#include "../include/Camera.h"

cgl::Camera::Camera(const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& up) : position(pos), direction(dir), worldUp(up) {}

glm::mat4 cgl::Camera::getViewMatrix() const {
	return glm::lookAt(position, position + direction, worldUp);
}

glm::vec3 cgl::Camera::getPosition() const {
	return position;
}

glm::vec3 cgl::Camera::getDirection() const {
	return direction;
}

void cgl::Camera::setPosition(const glm::vec3& pos) {
	position = pos;
}

void cgl::Camera::setRotation(float pitch, float yaw) {
	direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	direction.y = sin(glm::radians(pitch));
	direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	direction = glm::normalize(direction);
}

void cgl::Camera::moveForward(float amount) {
	position += amount * direction;
}

void cgl::Camera::moveRight(float amount) {
	position += amount * glm::normalize(glm::cross(direction, worldUp));
}
