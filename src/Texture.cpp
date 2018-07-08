#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "Texture.h"

cgl::Texture::Texture() {
	init();
}

cgl::Texture::Texture(const std::string& filename) {
	init();
	setTexture(filename);
}

void cgl::Texture::init() {
	stbi_set_flip_vertically_on_load(true);
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void cgl::Texture::setTexture(const std::string& filename) {
	glBindTexture(GL_TEXTURE_2D, id);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
	GLenum imageType = GL_RGB;
	if (nrChannels > 3) {
		imageType = GL_RGBA;
	}
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, imageType, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cerr << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void cgl::Texture::bind() const {
	glBindTexture(GL_TEXTURE_2D, id);
}
