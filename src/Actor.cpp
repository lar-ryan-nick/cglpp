#include "Actor.h"

cgl::Actor::Actor() {
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile("", aiProcess_Triangulate | aiProcess_FlipUVs);
}

void cgl::Actor::draw(const glm::mat4& parentModel, const Polygon& p) {
}