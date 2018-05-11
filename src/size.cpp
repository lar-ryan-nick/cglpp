#include "size.h"

Size::Size(float w, float h) {
	width = w;
	height = h;
}

float Size::getWidth() const {
	return width;
}

float Size::getHeight() const {
	return height;
}

void Size::setWidth(float w) {
	width = w;
}

void Size::setHeight(float h) {
	height = h;
}
