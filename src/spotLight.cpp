#include "spotLight.h"

SpotLight::SpotLight(const glm::vec3& dir, const glm::vec3& pos, const glm::vec3& color, float ambientStrength, float diffuseStrength, float specularStrength, float cut, float outerCut, float con, float lin, float quad) : DirectionalLight(dir, color, ambientStrength, diffuseStrength, specularStrength), PointLight(pos, color, ambientStrength, diffuseStrength, specularStrength, con, lin, quad), cutOff(cut), outerCutOff(outerCut) {}

float SpotLight::getCutOff() const {
	return cutOff;
}

void SpotLight::setCutOff(float cut) {
	cutOff = cut;
}

float SpotLight::getOuterCutOff() const {
	return outerCutOff;
}

void SpotLight::setOuterCutOff(float outerCut) {
	outerCutOff = outerCut;
}
