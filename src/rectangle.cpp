#include "rectangle.h"

Rectangle::Rectangle(float xpos, float ypos, float w, float h) : Position(xpos, ypos), Size(w, h) {
}
/*
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
*/

bool Rectangle::isInbetween(float min, float max, float val) const {
	return (min < val && val < max) || (max < val && val < min);
}

bool Rectangle::contains(Position p) const {
	return isInbetween(getX(), getX() + getWidth(), p.getX()) && isInbetween(getY(), getY() + getHeight(), p.getY());
}
