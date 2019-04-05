#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "../include/Texture.h"

std::unordered_map<std::string, unsigned int> cgl::Texture::loadedTextures;
cgl::Texture cgl::Texture::whiteTexture;
cgl::Texture cgl::Texture::blackTexture;

cgl::Texture::Texture(unsigned int tid) : id(tid) {}

cgl::Texture::Texture(const std::string& filename) : id(0) {
	setTexture(filename);
}

void cgl::Texture::setTexture(const std::string& filename) {
	std::unordered_map<std::string, unsigned int>::iterator it = loadedTextures.find(filename);
	if (it != loadedTextures.end()) {
		id = it->second;
		return;
	}
	// load and generate the texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
	GLenum imageType = GL_RED;
	if (nrChannels == 2) {
		imageType = GL_RG;
	} else if (nrChannels == 3) {
		imageType = GL_RGB;
	} else {
		imageType = GL_RGBA;
	}
	if (data != NULL) {
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, imageType, width, height, 0, imageType, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		loadedTextures.insert(std::pair<std::string, unsigned int>(filename, id));
	} else {
		std::cerr << "Failed to load texture at path: " << filename << std::endl;
		std::cerr << "Loaded a white texture instead" << std::endl;
		setTexture("res/img/white.jpg");
	}
	stbi_image_free(data);
}

void cgl::Texture::setID(unsigned int tid) {
	id = tid;
}

void cgl::Texture::bind() const {
	glBindTexture(GL_TEXTURE_2D, id);
}

cgl::Texture cgl::Texture::getWhiteTexture() {
	if (whiteTexture.id == 0) {
		whiteTexture.setTexture("res/img/white.jpg");
	}
	return whiteTexture;
}

cgl::Texture cgl::Texture::getBlackTexture() {
	if (blackTexture.id == 0) {
		blackTexture.setTexture("res/img/black.png");
	}
	return blackTexture;
}
