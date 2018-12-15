#include "../include/Rectangle.h"

cgl::Rectangle::Rectangle(float x, float y, float width, float height) : origin(x, y), size(width, height) {}

cgl::Rectangle::Rectangle(const Position& p, const Size& s) : origin(p), size(s) {}

bool cgl::Rectangle::isInbetween(float min, float max, float val) const {
	return (min < val && val < max) || (max < val && val < min);
}

cgl::Position cgl::Rectangle::getOrigin() const {
	return origin;
}

cgl::Size cgl::Rectangle::getSize() const {
	return size;
}

float cgl::Rectangle::getX() const {
	return origin.getX();
}

float cgl::Rectangle::getY() const {
	return origin.getY();
}

float cgl::Rectangle::getWidth() const {
	return size.getWidth();
}

float cgl::Rectangle::getHeight() const {
	return size.getHeight();
}

bool cgl::Rectangle::contains(const Position& p) const {
	return isInbetween(getX(), getX() + getWidth(), p.getX()) && isInbetween(getY(), getY() + getHeight(), p.getY());
}
