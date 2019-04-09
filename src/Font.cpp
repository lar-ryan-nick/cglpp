#include "Font.h"
#include <iostream>

std::unordered_map<std::string, std::vector<cgl::Font::Glyph> > cgl::Font::bitmaps;

cgl::Font::Glyph* cgl::Font::getBitmap(const std::string& fontName, int fontSize) {
	// don't generate bitmap if it has already been generated
	std::unordered_map<std::string, std::vector<Glyph> >::iterator it = bitmaps.find(fontName);
	if (it != bitmaps.end()) {
		return &(it->second[0]);
	}
	// init freetype libraries
	FT_Library ft;
	if (FT_Init_FreeType(&ft) != 0) {
		std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}
	FT_Face face;
	if (FT_New_Face(ft, fontName.c_str(), 0, &face) != 0) {
		std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
	}
	FT_Set_Pixel_Sizes(face, 0, fontSize);
	// load bitmap for all chars for given font
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	std::vector<Glyph> bitmap(NUM_CHAR);
	for (int c = 0; c < NUM_CHAR; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cerr << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
		}
		unsigned int tid;
		glGenTextures(1, &tid);
		glBindTexture(GL_TEXTURE_2D, tid);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		bitmap[c].texture.setID(tid);
		bitmap[c].size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
		bitmap[c].bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
		bitmap[c].advance = face->glyph->advance.x;
	}
	bitmaps.insert(std::pair<const std::string, std::vector<Glyph> >(fontName, bitmap));
	// release freetype resources
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
	it = bitmaps.find(fontName);
	return &(it->second[0]);
}

cgl::Font::Font(const std::string& fontName, int fontSize) {
	bitmap = getBitmap(fontName, fontSize);
	maxHeight = findMaxHeight();
}

int cgl::Font::getMaxHeight() {
	return maxHeight;
}

cgl::Font::Glyph* cgl::Font::getBitmap() {
	return bitmap;
}

int cgl::Font::findMaxHeight() {
	int max = 0;
	for (int c = 0; c < NUM_CHAR; c++) {
		if (bitmap[c].size.y > max) {
			max = bitmap[c].size.y;
		}
	}
	return max;
}
