#include "light.h"

Light::Light(const glm::vec3& col, float ambient, float diffuse, float specular) {
	color = col;
	ambientStrength = ambient;
	diffuseStrength = diffuse;
	specularStrength = specular;
}

glm::vec3 Light::getColor() const {
	return color;
}

float Light::getAmbientStrength() const {
	return ambientStrength;
}

float Light::getDiffuseStrength() const {
	return diffuseStrength;
}

float Light::getSpecularStrength() const {
	return specularStrength;
}

void Light::setColor(const glm::vec3& col) {
	color = col;
}

void Light::setAmbientStrength(float ambient) {
	ambientStrength = ambient;
}

void Light::setDiffuseStrength(float diffuse) {
	diffuseStrength = diffuse;
}

void Light::setSpecularStrength(float specular) {
	specularStrength = specular;
}
