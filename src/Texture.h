#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <unordered_map>
#include <string>
#include <iostream>

namespace cgl {
	class Texture {
		private:
			unsigned int id;
			static std::unordered_map<std::string, unsigned int> loadedTextures;
			void init();
		public:
			Texture();
			Texture(const std::string& textureSource);
			void setTexture(const std::string& textureSource);
			void bind() const;
	};
}

#endif
