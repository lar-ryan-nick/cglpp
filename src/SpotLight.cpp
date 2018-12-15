#include "../include/SpotLight.h"

cgl::SpotLight::SpotLight(const glm::vec3& dir, const glm::vec3& pos, const glm::vec3& color, float ambientStrength, float diffuseStrength, float specularStrength, float cut, float outerCut, float con, float lin, float quad) : Light(color, ambientStrength, diffuseStrength, specularStrength), DirectionalLight(dir, color, ambientStrength, diffuseStrength, specularStrength), PointLight(pos, color, ambientStrength, diffuseStrength, specularStrength, con, lin, quad), cutOff(cut), outerCutOff(outerCut) {}

float cgl::SpotLight::getCutOff() const {
	return cutOff;
}

void cgl::SpotLight::setCutOff(float cut) {
	cutOff = cut;
}

float cgl::SpotLight::getOuterCutOff() const {
	return outerCutOff;
}

void cgl::SpotLight::setOuterCutOff(float outerCut) {
	outerCutOff = outerCut;
}
