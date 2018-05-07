#include "directionalLight.h"

DirectionalLight::DirectionalLight(const glm::vec3& dir, const glm::vec3& color, float ambientStrength, float diffuseStrength, float specularStrength) : Light(color, ambientStrength, diffuseStrength, specularStrength), direction(dir) {}

glm::vec3 DirectionalLight::getDirection() const {
	return direction;
}

void DirectionalLight::setDirection(const glm::vec3& dir) {
	direction = dir;
}
