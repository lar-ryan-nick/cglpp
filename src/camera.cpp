#include "camera.h"

Camera::Camera(const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& up) : position(pos), direction(dir), worldUp(up) {}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(position, position + direction, worldUp);
}

glm::vec3 Camera::getPosition() {
	return position;
}

glm::vec3 Camera::getDirection() {
	return direction;
}

void Camera::setPosition(const glm::vec3& pos) {
	position = pos;
}

void Camera::setRotation(float pitch, float yaw) {
	direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	direction.y = sin(glm::radians(pitch));
	direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	direction = glm::normalize(direction);
}

void Camera::moveForward(float amount) {
	position += amount * direction;
}

void Camera::moveRight(float amount) {
	position += amount * glm::normalize(glm::cross(direction, worldUp));
}
