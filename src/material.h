#ifndef MATERIAL_H
#define MATERIAL_H

#include <glad/glad.h>
#include "texture.h"

class Material {
	Texture diffuseMap;
	Texture specularMap;
	float shininess;
	public:
		Material(const Texture& diffuse, const Texture& specular, float shine);
		void bind() const;
		float getShininess() const;
};

#endif
