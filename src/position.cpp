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
