#include "position.h"

Position::Position(float xPos, float yPos) : x(xPos), y(yPos) {
}

float Position::getX() const {
	return x;
}

float Position::getY() const {
	return y;
}

void Position::setX(float xPos) {
	x = xPos;
}

void Position::setY(float yPos) {
	y = yPos;
}

void Position::move(float xOffset, float yOffset) {
	x += xOffset;
	y += yOffset;
}

bool Position::operator==(const Position& p) const {
	return x == p.x && y == p.y;
}

size_t std::hash<Position>::operator()(const Position& p) const {
	return hash<float>()(p.getX()) ^ hash<float>()(p.getY());
}
