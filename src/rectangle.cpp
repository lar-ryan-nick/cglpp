#include "rectangle.h"

cgl::Rectangle::Rectangle(float xpos, float ypos, float w, float h) : Position(xpos, ypos), Size(w, h) {
}

bool cgl::Rectangle::isInbetween(float min, float max, float val) const {
	return (min < val && val < max) || (max < val && val < min);
}

bool cgl::Rectangle::contains(const Position& p) const {
	return isInbetween(getX(), getX() + getWidth(), p.getX()) && isInbetween(getY(), getY() + getHeight(), p.getY());
}
