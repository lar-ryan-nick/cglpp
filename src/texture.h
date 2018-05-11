#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <string>
#include <iostream>

class Texture {
	unsigned int id;
	void init();
	public:
		Texture();
		Texture(const std::string& textureSource);
		void setTexture(const std::string& textureSource);
		void bind() const;
};

#endif
