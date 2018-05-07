#include "rectangle.h"

Rectangle::Rectangle(float xpos, float ypos, float w, float h) {
	x = xpos;
	y = ypos;
	width = w;
	height = h;
}

float Rectangle::getX() const {
	return x;
}

float Rectangle::getY() const {
	return y;
}

float Rectangle::getWidth() const {
	return width;
}

float Rectangle::getHeight() const {
	return height;
}

void Rectangle::setX(float xpos) {
	x = xpos;
}

void Rectangle::setY(float ypos) {
	y = ypos;
}

void Rectangle::setWidth(float w) {
	width = w;
}

void Rectangle::setHeight(float h) {
	height = h;
}
