#ifndef FONT_H
#define FONT_H

#include "ft2build.h"
#include FT_FREETYPE_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

#include "Texture.h"
#include "Shader.h"

#define NUM_CHAR 256

namespace cgl {
	class Font {
		public:
			struct Glyph {
				Texture texture;
				glm::ivec2 size;
				glm::ivec2 bearing;
				unsigned int advance;
			};
			Font(const std::string& fontName, int fontSize = 48);
			Glyph* getBitmap();
			int getMaxHeight();
		private:
			Glyph* bitmap;
			int maxHeight;
			static std::unordered_map<std::string, std::vector<Glyph> > bitmaps;
			static Glyph* getBitmap(const std::string& fontName, int fontSize = 48);
			int findMaxHeight();
	};
}

#endif
