#include "position.h"

cgl::Position::Position(float x, float y, float z, float w) : VectorContainer(x, y, z, w) {}

cgl::Position::Position(const glm::vec2& v, float z, float w) : VectorContainer(v, z, w) {}

cgl::Position::Position(const glm::vec3& v, float w) : VectorContainer(v, w) {}

cgl::Position::Position(const glm::vec4& v) : VectorContainer(v) {}

cgl::Position::Position(const VectorContainer& vc) : VectorContainer(vc) {}

void cgl::Position::move(float x, float y, float z) {
	*this += Position(x, y, z);
}
