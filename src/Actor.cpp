#include "Actor.h"

cgl::Actor::Actor(const std::string& path) : model(path) {}

cgl::Actor::~Actor() {}

void cgl::Actor::draw(Shader& shader, const glm::mat4 mvp, const Polygon& clippingRegion) {
	model.draw(shader, mvp, clippingRegion);
}
