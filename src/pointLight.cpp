#include "pointLight.h"

PointLight::PointLight(const glm::vec3& pos, const glm::vec3& color, float ambientStrength, float diffuseStrength, float specularStrength, float con, float lin, float quad) : Light(color, ambientStrength, diffuseStrength, specularStrength), position(pos), constant(con), linear(lin), quadratic(quad) {}

glm::vec3 PointLight::getPosition() const {
	return position;
}

float PointLight::getConstant() const {
	return constant;
}

float PointLight::getLinear() const {
	return linear;
}

float PointLight::getQuadratic() const {
	return quadratic;
}

void PointLight::setPosition(const glm::vec3& pos) {
	position = pos;
}
