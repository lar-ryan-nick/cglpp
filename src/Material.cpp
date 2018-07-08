#include "Material.h"

cgl::Material::Material(const Texture& diffuse, const Texture& specular, float shine) {
	diffuseMap = diffuse;
	specularMap = specular;
	shininess = shine;
}

void cgl::Material::bind() const {
	glActiveTexture(GL_TEXTURE0);
	diffuseMap.bind();
	glActiveTexture(GL_TEXTURE1);
	specularMap.bind();
}

float cgl::Material::getShininess() const {
	return shininess;
}
