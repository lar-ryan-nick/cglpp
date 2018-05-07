#include "color.h"

Color::Color(float r, float g, float b, float a) : red(r), green(g), blue(b), alpha(a) {
	fixValues();
}

Color::Color(int r, int g, int b, int a) : red(r / 255.0f), green(g / 255.0f), blue(b / 255.0f), alpha(a / 255.0f) {
	fixValues();
}

void Color::fixValues() {
	if (red > 1.0f) {
		red = 1.0f;
	}
	if (red < 0.0f) {
		red = 0.0f;
	}
	if (green > 1.0f) {
		green = 1.0f;
	}
	if (green < 0.0f) {
		green = 0.0f;
	}
	if (blue > 1.0f) {
		blue = 1.0f;
	}
	if (blue < 0.0f) {
		blue = 0.0f;
	}
	if (alpha > 1.0f) {
		alpha = 1.0f;
	}
	if (alpha < 0.0f) {
		alpha = 0.0f;
	}
}

float Color::getRed() const {
	return red;
}

float Color::getGreen() const {
	return green;
}

float Color::getBlue() const {
	return blue;
}

float Color::getAlpha() const {
	return alpha;
}
