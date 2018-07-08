#include "Color.h"

cgl::Color::Color(float r, float g, float b, float a) : VectorContainer(r, g, b, a) {
	fixValues();
}

cgl::Color::Color(const glm::vec3 v, float a) : VectorContainer(v, a) {
	fixValues();
}

cgl::Color::Color(const glm::vec4 v) : VectorContainer(v) {
	fixValues();
}

void cgl::Color::fixValues() {
	fixRed();
	fixGreen();
	fixBlue();
	fixAlpha();
}

void cgl::Color::fixRed() {
	if (getX() > 1.0f) {
		setX(1.0f);
	} else if (getX() < 0.0f) {
		setX(0.0f);
	}
}

void cgl::Color::fixGreen() {
	if (getY() > 1.0f) {
		setY(1.0f);
	} else if (getY() < 0.0f) {
		setY(0.0f);
	}
}

void cgl::Color::fixBlue() {
	if (getZ() > 1.0f) {
		setZ(1.0f);
	} else if (getZ() < 0.0f) {
		setZ(0.0f);
	}
}

void cgl::Color::fixAlpha() {
	if (getW() > 1.0f) {
		setW(1.0f);
	} else if (getW() < 0.0f) {
		setW(0.0f);
	}
}

float cgl::Color::getRed() const {
	return getX();
}

float cgl::Color::getGreen() const {
	return getY();
}

float cgl::Color::getBlue() const {
	return getZ();
}

float cgl::Color::getAlpha() const {
	return getW();
}

void cgl::Color::setRed(float red) {
	setX(red);
	fixRed();
}

void cgl::Color::setGreen(float green) {
	setY(green);
	fixGreen();
}

void cgl::Color::setBlue(float blue) {
	setZ(blue);
	fixBlue();
}

void cgl::Color::setAlpha(float alpha) {
	setW(alpha);
	fixAlpha();
}