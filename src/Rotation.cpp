#include "Rotation.h"

cgl::Rotation::Rotation(float x = 0.0f, float y = 0.0f, float z = 0.0f) : VectorContainer(x, y, z) {}

void cgl::Rotation::rotate(float x, float y = 0.0f, float z = 0.0f) {
	*this += glm::vec3(x, y, z);
}