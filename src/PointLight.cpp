#include "../include/PointLight.h"

cgl::PointLight::PointLight(const glm::vec3& pos, const glm::vec3& color, float ambientStrength, float diffuseStrength, float specularStrength, float con, float lin, float quad) : Light(color, ambientStrength, diffuseStrength, specularStrength), position(pos), constant(con), linear(lin), quadratic(quad) {}

glm::vec3 cgl::PointLight::getPosition() const {
	return position;
}

float cgl::PointLight::getConstant() const {
	return constant;
}

float cgl::PointLight::getLinear() const {
	return linear;
}

float cgl::PointLight::getQuadratic() const {
	return quadratic;
}

void cgl::PointLight::setPosition(const glm::vec3& pos) {
	position = pos;
}
