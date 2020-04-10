#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "../include/Texture.h"

std::unordered_map<std::string, unsigned int> cgl::Texture::loadedTextures;
cgl::Texture cgl::Texture::whiteTexture;
cgl::Texture cgl::Texture::blackTexture;
unsigned int cgl::Texture::activeTextures(0);

cgl::Texture::Texture(unsigned int tid) : id(tid) {}

cgl::Texture::Texture(const std::string& filename) : id(0) {
	setTexture(filename);
}

void cgl::Texture::setTexture(unsigned char* data, int width, int height) {
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
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
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 4);
	if (data != NULL) {
		setTexture(data, width, height);
		loadedTextures.insert(std::pair<std::string, unsigned int>(filename, id));
		stbi_image_free(data);
	} else {
		std::cerr << "Failed to load texture at path: " << filename << std::endl;
		std::cerr << "Loaded a white texture instead" << std::endl;
		setID(getWhiteTexture().id);
	}
}

unsigned int cgl::Texture::getID() const {
	return id;
}

void cgl::Texture::setID(unsigned int tid) {
	id = tid;
}

unsigned int cgl::Texture::bind() const {
	glActiveTexture(GL_TEXTURE0 + activeTextures);
	glBindTexture(GL_TEXTURE_2D, id);
	return activeTextures++;
}

void cgl::Texture::unbindAll() {
	while (activeTextures >= 0) {
		glActiveTexture(GL_TEXTURE0 + activeTextures);
		glBindTexture(GL_TEXTURE_2D, 0);
		--activeTextures;
	}
	activeTextures = 0;
}

cgl::Texture cgl::Texture::getWhiteTexture() {
	if (whiteTexture.id == 0) {
		unsigned char data[4] = {1, 1, 1, 1};
		whiteTexture.setTexture(data, 1, 1);
	}
	return whiteTexture;
}

cgl::Texture cgl::Texture::getBlackTexture() {
	if (blackTexture.id == 0) {
		unsigned char data[4] = {0, 0, 0, 1};
		blackTexture.setTexture(data, 1, 1);
	}
	return blackTexture;
}
