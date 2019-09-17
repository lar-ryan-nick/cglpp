#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <unordered_map>
#include <string>
#include <iostream>

namespace cgl {
	class Texture {
		private:
			static Texture whiteTexture;
			static Texture blackTexture;
			unsigned int id;
			static std::unordered_map<std::string, unsigned int> loadedTextures;
			void init();
		public:
			Texture(unsigned int tid = 0);
			Texture(const std::string& textureSource);
			Texture(unsigned char* data, int width, int height);
			void setTexture(unsigned char* data, int width, int height);
			void setTexture(const std::string& textureSource);
			void setID(unsigned int tid);
			void bind() const;
			static Texture getBlackTexture();
			static Texture getWhiteTexture();
	};
}

#endif
