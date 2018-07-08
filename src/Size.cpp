#include "Size.h"

cgl::Size::Size(float x, float y, float z, float w) : VectorContainer(x, y, z, w) {}

cgl::Size::Size(const glm::vec2& v, float z, float w) : VectorContainer(v, z, w) {}

cgl::Size::Size(const glm::vec3& v, float w) : VectorContainer(v, w) {}

cgl::Size::Size(const glm::vec4& v) : VectorContainer(v) {}

cgl::Size::Size(const VectorContainer& vc) : VectorContainer(vc) {}

void cgl::Size::scale(float x, float y, float z) {
	scale(glm::vec3(x, y, z));
}

void cgl::Size::scale(const glm::vec2& scalar) {
	scale(glm::vec3(scalar, 1.0f));
}

void cgl::Size::scale(const glm::vec3& scalar) {
	*this *= scalar;
}

float cgl::Size::getWidth() const {
	return getX();
}

float cgl::Size::getHeight() const {
	return getY();
}

float cgl::Size::getDepth() const {
	return getZ();
}

void cgl::Size::setWidth(float width) {
	setX(width);
}

void cgl::Size::setHeight(float height) {
	setY(height);
}

void cgl::Size::setDepth(float depth) {
	setZ(depth);
}