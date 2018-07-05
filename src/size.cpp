#include "size.h"

cgl::Size::Size(float w, float h) {
	width = w;
	height = h;
}

float cgl::Size::getWidth() const {
	return width;
}

float cgl::Size::getHeight() const {
	return height;
}

void cgl::Size::setWidth(float w) {
	width = w;
}

void cgl::Size::setHeight(float h) {
	height = h;
}
