#include "TextView.h"

cgl::Shader* cgl::TextView::textViewShader = NULL;

void cgl::TextView::init() {
	if (textViewShader == NULL) {
		textViewShader = new Shader("TextViewVertexShader.glsl", "TextViewFragmentShader.glsl");
	}
}

cgl::TextView::TextView(const std::string& t, float x, float y, float width, float height) : View(x, y, width, height), text(t), font("res/fonts/Arial.ttf") {
	init();
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

