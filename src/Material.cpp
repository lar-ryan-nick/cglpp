#include "Material.h"

cgl::Material::Material(const std::list<Texture>& diffuses, const std::list<Texture>& speculars, float shine) : diffuseMaps(diffueses), specularMaps(speculars), shininess(shine) {
}

std::list<cgl::Texture> cgl::Material::getDiffuseMaps() const {
	return diffuseMaps;
}

std::list<cgl::Texture> cgl::Material::getSpecularMaps() const {
	return specularMaps;
}

float cgl::Material::getShininess() const {
	return shininess;
}
