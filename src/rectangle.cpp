#include "rectangle.h"

Rectangle::Rectangle(float xpos, float ypos, float w, float h) : Position(xpos, ypos), Size(w, h) {
}

bool Rectangle::isInbetween(float min, float max, float val) const {
	return (min < val && val < max) || (max < val && val < min);
}

bool Rectangle::contains(Position p) const {
	return isInbetween(getX(), getX() + getWidth(), p.getX()) && isInbetween(getY(), getY() + getHeight(), p.getY());
}
