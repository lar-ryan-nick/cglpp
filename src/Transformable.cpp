#include "Transformable.h"

cgl::Transformable::Transformable() : translation(0.0f, 0.0f, 0.0f), scalar(1.0f, 1.0f, 1.0f), rotation(0.0f), center(0.0f, 0.0f, 0.0f) {}

glm::vec3 cgl::Transformable::getTranslation() const {
	return translation;
}

void cgl::Transformable::translate(float x, float y, float z) {
	translation += glm::vec3(x, y, z);
}

void cgl::Transformable::setTranslation(const glm::vec3& t) {
	translation = t;
}

float cgl::Transformable::getRotation() const {
	return rotation;
}

void cgl::Transformable::rotate(float radians) {
	rotation += radians;
}

void cgl::Transformable::setRotation(float radians) {
	rotation = radians;
}

glm::vec3 cgl::Transformable::getScalar() const {
	return scalar;
}

void cgl::Transformable::scale(float x, float y, float z) {
	scalar *= glm::vec3(x, y, z);
}

void cgl::Transformable::setScalar(const glm::vec3& s) {
	scalar = s;
}

void cgl::Transformable::setCenter(const glm::vec3& c) {
	center = c;
}

glm::mat4 cgl::Transformable::getTransformationMatrix() const {
	glm::mat4 model(1.0f);
	model = glm::translate(model, static_cast<glm::vec3>(translation));
	model = glm::translate(model, glm::vec3(center.x, center.y, center.z));
	model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, static_cast<glm::vec3>(scalar));
	model = glm::translate(model, glm::vec3(-center.x, -center.y, -center.z));
	return model;
}

glm::mat4 cgl::Transformable::getInverseTransformationMatrix() const {
	return glm::inverse(getTransformationMatrix());
}
