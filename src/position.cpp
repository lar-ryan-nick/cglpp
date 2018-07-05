#include "position.h"

cgl::Position::Position(float xPos, float yPos) : x(xPos), y(yPos) {
}

float cgl::Position::getX() const {
	return x;
}

float cgl::Position::getY() const {
	return y;
}

void cgl::Position::setX(float xPos) {
	x = xPos;
}

void cgl::Position::setY(float yPos) {
	y = yPos;
}

void cgl::Position::move(float xOffset, float yOffset) {
	x += xOffset;
	y += yOffset;
}

bool cgl::Position::operator==(const Position& p) const {
	return x == p.x && y == p.y;
}

size_t std::hash<cgl::Position>::operator()(const cgl::Position& p) const {
	return hash<float>()(p.getX()) ^ hash<float>()(p.getY());
}
