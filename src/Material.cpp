#include "../include/Material.h"

cgl::Material cgl::Material::materialFromAssimp(aiMaterial* m, const aiScene* scene, const std::string& directory) {
	Material material;
	aiColor3D ambientColor;
	m->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
	material.ambientColor.setColor(ambientColor.r, ambientColor.g, ambientColor.b);
	aiColor3D diffuseColor;
	m->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
	material.diffuseColor.setColor(diffuseColor.r, diffuseColor.g, diffuseColor.b);
	aiColor3D specularColor;
	m->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
	material.specularColor.setColor(specularColor.r, specularColor.g, specularColor.b);
	loadMaterialTextureMaps(m, aiTextureType_AMBIENT, scene, directory, material.textureMaps);
	loadMaterialTextureMaps(m, aiTextureType_DIFFUSE, scene, directory, material.textureMaps);
	loadMaterialTextureMaps(m, aiTextureType_SPECULAR, scene, directory, material.textureMaps);
	m->Get(AI_MATKEY_SHININESS, material.shininess);
	m->Get(AI_MATKEY_OPACITY, material.opacity);
	return material;
}

void cgl::Material::loadMaterialTextureMaps(aiMaterial* material, aiTextureType type, const aiScene* scene, const std::string& directory, std::list<TextureMap>& textureMaps) {
	for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
		textureMaps.push_back(TextureMap::textureMapFromAssimp(material, scene, directory, type, i));
	}
}

cgl::Material::Material() : shininess(0.0f), opacity(1.0f) {}

cgl::Color cgl::Material::getAmbientColor() const {
	return ambientColor;
}

cgl::Color cgl::Material::getDiffuseColor() const {
	return diffuseColor;
}

cgl::Color cgl::Material::getSpecularColor() const {
	return specularColor;
}

std::list<cgl::TextureMap> cgl::Material::getTextureMaps() const {
	return textureMaps;
}

float cgl::Material::getShininess() const {
	return shininess;
}

float cgl::Material::getOpacity() const {
	return opacity;
}
