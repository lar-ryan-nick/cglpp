#include "Actor.h"

cgl::Actor::Actor(const std::string& path) : model(path) {}

cgl::Actor::~Actor() {}

void cgl::Actor::draw(Shader& shader, const glm::mat4& parentModel) {
	model.draw(shader, parentModel);
}
