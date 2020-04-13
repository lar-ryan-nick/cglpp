#include "TextView.h"

cgl::Shader* cgl::TextView::textViewShader = NULL;

void cgl::TextView::init() {
	if (textViewShader == NULL) {
		textViewShader = new Shader("res/glsl/TextViewVertexShader.glsl", "res/glsl/TextViewFragmentShader.glsl");
	}
}

cgl::TextView::TextView(const std::string& t, float x, float y, float width, float height) : View(x, y, width, height), text(t), font("res/fonts/Arial.ttf") {
	init();
	glGenVertexArrays(1, &textVAO);
	glGenBuffers(1, &textVBO);
	glBindVertexArray(textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0); 
	glBindVertexArray(0);
	setScalar(glm::vec3(1.0f, -1.0f, 1.0f));
}

cgl::TextView::~TextView() {
	glDeleteBuffers(1, &textVBO);
	glDeleteVertexArrays(1, &textVAO);
}

void cgl::TextView::setText(const std::string& t) {
	text = t;
}

void cgl::TextView::setFont(const std::string& fontName) {
	font = Font(fontName);
}

void cgl::TextView::setFont(const Font& f) {
	font = f;
}

void cgl::TextView::render(const cgl::Polygon& bounds, const glm::mat4& model) {
	View::render(bounds, model);
	float viewport[4];
	glGetFloatv(GL_VIEWPORT, viewport);
	glm::mat4 projection = glm::ortho(0.0f, viewport[2], viewport[3], 0.0f);
	glm::mat4 mvp = projection * model;
	Rectangle bnds = getBounds();
	float x = bnds.getX();
	float y = bnds.getY() + bnds.getHeight() - font.getMaxHeight();
	Font::Glyph* bitmap = font.getBitmap();
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	textViewShader->use();
	textViewShader->setUniform("text", 0);
	textViewShader->setUniform("textColor", 0.0f, 0.0f, 0.0f, 1.0f);
	// Iterate over characters
	for (std::string::const_iterator c = text.begin(); c != text.end(); c++) {
		Font::Glyph g = bitmap[static_cast<unsigned char>(*c)];
		float xpos = x + g.bearing.x;
		float ypos = y - g.size.y + g.bearing.y;
		float w = g.size.x;
		float h = g.size.y;
		// Update VBO for each character
		float verticies[6][4] = {
		    { xpos,     ypos + h,   0.0, 0.0 },            
		    { xpos,     ypos,       0.0, 1.0 },
		    { xpos + w, ypos,       1.0, 1.0 },
		    { xpos,     ypos + h,   0.0, 0.0 },
		    { xpos + w, ypos,       1.0, 1.0 },
		    { xpos + w, ypos + h,   1.0, 0.0 }           
		};
		Polygon p;
		for (int i = 0; i < 6; i++) {
			glm::vec4 vert(verticies[i][0], verticies[i][1], 0.0f, 1.0f);
			glm::vec4 transformed = mvp * vert;
			verticies[i][0] = transformed.x;
			verticies[i][1] = transformed.y;
			p.addVertex(glm::vec2(transformed.x, transformed.y));
		}
		/*
		std::cout << 3 << std::endl;
		std::list<Polygon> clippedPolygons = p.clipTo(bounds);
		std::cout << 4 << std::endl;
		*/
		// Render glyph texture over quad
		g.texture.bind();
		// Update content of VBO memory
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verticies), verticies); 
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (g.advance >> 6) * getScalar().x; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	textViewShader->finish();
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
