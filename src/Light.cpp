#include "Light.h"

cgl::Light::Light(const glm::vec3& col, float ambient, float diffuse, float specular) {
	color = col;
	ambientStrength = ambient;
	diffuseStrength = diffuse;
	specularStrength = specular;
}

glm::vec3 cgl::Light::getColor() const {
	return color;
}

float cgl::Light::getAmbientStrength() const {
	return ambientStrength;
}

float cgl::Light::getDiffuseStrength() const {
	return diffuseStrength;
}

float cgl::Light::getSpecularStrength() const {
	return specularStrength;
}

void cgl::Light::setColor(const glm::vec3& col) {
	color = col;
}

void cgl::Light::setAmbientStrength(float ambient) {
	ambientStrength = ambient;
}

void cgl::Light::setDiffuseStrength(float diffuse) {
	diffuseStrength = diffuse;
}

void cgl::Light::setSpecularStrength(float specular) {
	specularStrength = specular;
}
