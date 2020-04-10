#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <unordered_map>
#include <string>
#include <iostream>

namespace cgl {
	class Texture {
		public:
			Texture(unsigned int tid = 0);
			Texture(const std::string& textureSource);
			Texture(unsigned char* data, int width, int height);
			unsigned int getID() const;
			void setTexture(unsigned char* data, int width, int height);
			void setTexture(const std::string& textureSource);
			void setID(unsigned int tid);
			unsigned int bind() const;
			static void unbindAll();
			static Texture getBlackTexture();
			static Texture getWhiteTexture();
		private:
			static Texture whiteTexture;
			static Texture blackTexture;
			static std::unordered_map<std::string, unsigned int> loadedTextures;
			static unsigned int activeTextures;
			unsigned int id;
			void init();
	};
}

#endif
