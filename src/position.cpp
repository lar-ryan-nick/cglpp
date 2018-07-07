#include "position.h"

cgl::Position::Position(float x, float y, float z, float w) : VectorContainer(x, y, z, w) {}

void cgl::Position::move(float x, float y, float z) {
	*this += Position(x, y, z);
}
