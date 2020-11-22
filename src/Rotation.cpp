#include "Rotation.h"

cgl::Rotation::Rotation(float x, float y, float z) : VectorContainer(x, y, z) {}

void cgl::Rotation::rotate(float x, float y, float z) {
	*this += glm::vec3(x, y, z);
}
