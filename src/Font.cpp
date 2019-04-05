#include "Font.h"
#include <iostream>

cgl::Shader* cgl::Font::shader = NULL;
unsigned int cgl::Font::vao = 0;
unsigned int cgl::Font::vbo = 0;
std::unordered_map<std::string, std::vector<cgl::Font::Glyph> > cgl::Font::bitmaps;

void cgl::Font::init() {
	if (shader == NULL) {
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		shader = new cgl::Shader("res/glsl/TextViewVertexShader.glsl", "res/glsl/TextViewFragmentShader.glsl");
	}
}

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
	init();
	bitmap = getBitmap(fontName, fontSize);
}

void cgl::Font::renderString(const std::string& s, int x, int y, int scale) {
	float viewport[4];
	glGetFloatv(GL_VIEWPORT, viewport);
	glm::mat4 projection = glm::ortho(0.0f, viewport[2], 0.0f, viewport[3], -0.1f, 0.1f);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(vao);
	shader->use();
	shader->setUniform("projection", projection);
	// Iterate through all characters
	for (std::string::const_iterator c = s.begin(); c != s.end(); c++) {
		Glyph g = bitmap[static_cast<unsigned char>(*c)];
	
		float xpos = x + g.bearing.x * scale;
		float ypos = y - (g.size.y - g.bearing.y) * scale;
	
		float w = g.size.x * scale;
		float h = g.size.y * scale;
		// Update VBO for each character
		float vertices[6][4] = {
		    { xpos,     ypos + h,   0.0, 0.0 },            
		    { xpos,     ypos,       0.0, 1.0 },
		    { xpos + w, ypos,       1.0, 1.0 },
	
		    { xpos,     ypos + h,   0.0, 0.0 },
		    { xpos + w, ypos,       1.0, 1.0 },
		    { xpos + w, ypos + h,   1.0, 0.0 }           
		};
		// Render glyph texture over quad
		g.texture.bind();
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (g.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	shader->finish();
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
